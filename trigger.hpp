#include <stdarg.h>
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
    virtual bool validate(char* p) = 0;
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

    bool validate(char* p) {
      if (strspn(p, "0123456789") != strlen(p)) return false;
      int v = atoi(p);
      return v >= min && v <= max;
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

    bool validate(char* p) {
      if (strspn(p, "0123456789.,") == strlen(p)) return false;
      float v = atof(p);
      return v >= min && v <= max;
    }
};

class StrParam: public Param {
  private:
    //float min;
  public:
    StrParam(): Param() {
      setDef((char*)"STR ");
    }
    bool validate(char* p) {
      return true;
    }
};

class BoolParam: public Param {
  private:

  public:
    BoolParam(): Param() {
      setDef((char*)"BOOL ");
    }

    bool validate(char* p) {
      return strcmp(p, "FALSE") == 0 || strcmp(p, "TRUE") == 0;
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
    bool validate(char* p) {
      for (int i = 0; i < count; i++) {
        if (strcmp(opts[i], p) == 0)
          return true;
      }
      return false;
    }
};

template<int count>
class FlagParam: public Param {
  private:
    const char* flags[count];
  public:
    FlagParam(const char* f[count]) {
      char buff[1024];
      memset(buff, 0, 1024);
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
    bool validate(char* p) {
      return false;
    }
};

class TArg {
  private:
    char** strParams;
    Param** params;
    int paramCount;
  public:
    TArg(int paramCount, char** strParams, Param** params) {
      this->paramCount = paramCount;
      this->strParams = strParams;
      this->params = params;
    }
    int asInt(int i) {
      char* p = strParams[i];
      return atoi(p);
    }
    float asFloat(int i) {
      char* p = strParams[i];
      return atof(p);
    }
    const char* asStr(int i) {
      return strParams[i];
    }

    int asVal(int i) {
      Param* param = params[i];

      return 0;
    }

    int asFlag(int i) {
      return 0;
    }
};

typedef void (*TriggerCall)(TArg);



class AbstractTrigger {
  private:
    TriggerCall callback = NULL;
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

    void on(TriggerCall c) {
      this->callback = c;
    }

    void call(TArg arg) {
      if (this->callback != NULL) {
        this->callback(arg);
      }
    }


    virtual int getParamsCount() = 0;
    virtual bool validateParam(int i, char* p) = 0;
    virtual Param** getParams() = 0;
    virtual const char* getDef() = 0;

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

    int getParamsCount() {
      return paramsCount;
    }

    Param** getParams() {
      return params;
    }

    const char* getDef() {
      return def;
    }

    bool validateParam(int i, char* p) {
      Param* param = params[i];
      return param->validate(p);
    }

};


