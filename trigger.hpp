
class Param {
  private:
    char* d;
  protected:
    void setDef(char* def) {
      this->d = (char*)(malloc(strlen(def)));
      strcpy(this->d, def);
    }
  public:
    Param() {
    }
    const char* def() {
      return this->d;
    }
    bool validate(){
      return true;
    }
};


class IntParam: public Param {
  private:
    int min, max;
  public:
    IntParam(int min, int max) {
      this->min = min;
      this->max = max;
      char buf[512];
      sprintf(buf, "INT %d %d ", min, max);
      setDef(buf);
    }

};

class FloatParam: public Param {
  private:
    float min, max;
  public:
    FloatParam(float min, float max) {
      this->min = min;
      this->max = max;
      char buf[512];
      sprintf(buf, "FLOAT %f %f ", min, max);
      setDef(buf);
    }

};

class StrParam: public Param {
  private:
    float min;
  public:
    StrParam(): Param() {
      setDef((char*)"STR ");
    }
};

#define OPTS (const char*[])
template<int count>
class ValParam: public Param {
  private:
    const char* opts[count];
  public:
    ValParam(const char* s[count]) {
      char buf[1024];
      //memset(buf,0,1024);
      strcpy(buf, "VAL ");

      for (int i = 0; i < count; i++) {
        opts[i] = s[i];
        strcat(buf, s[i]);
        if (i < count - 1) {
          strcat(buf, ",");
        }
      }
      strcat(buf, " ");
      setDef(buf);
    }
};

template<int count>
class FlagParam: public Param {
  private:
    const char* flags[count];
  public:
    FlagParam(const char* f[count]) {
      char buff[1024];
      strcpy(buff, "FLAG ");

      for (int i = 0; i < count; i++) {
        flags[i] = f[i];
        strcat(buff, f[i]);
        if (i < count - 1) {
          strcat(buff, ",");
        }
      }
      strcat(buff, " ");
      setDef(buff);
    }
};


class AbstractTrigger {
  protected:
    const char* name;
    void setName(const char* n) {
      this->name = n;
    }
  public:
    void call(char* cmd) {
    }

    const char* getName() {
      return name;
    }

    virtual bool validateParam(int num) = 0;
};

template<int paramsCount>
class Trigger: public AbstractTrigger {
  private:
    char* def;
    Param* params[paramsCount];
  public:
    Trigger(const char* name, ...) {
      va_list ap;
      va_start(ap, name);

      setName(name);
      char buff[1024];
      sprintf(buff, "TRIGGER %s ", name);

      for (int i = 0; i < paramsCount; i++) {
        Param* p = va_arg(ap, Param*);
        params[i] = p;
        strcat(buff, p->def());
      }

      va_end(ap);
      def = (char*)(malloc(strlen(buff)));
      strcpy(def, buff);
    }


    bool validateParam(int n) {
      Param* p = params[n];
      
      return p->validate();
    }
    const char* getDef() {
      return def;
    }

};


