class Sensor {
  private:
    const char* name;
    char* def;
    bool changed;
  protected:
    void update() {
      changed = true;
    }
    virtual char* getValue() = 0;

  public:
    Sensor(const char* name) {
      this->name = name;
    };
    void setDef(char* def) {
      this->def = (char*)(malloc(strlen(def) + 1));
      strcpy(this->def, def);
    }
    char* getDef() {
      return this->def;
    }
};

class IntSensor: public Sensor {
  private:
    int min = 0;
    int max = 0;
    int val = 0;
  protected:
    char* getValue() {
      return NULL;
    }
  public:
    IntSensor(const char* name, int min, int max): Sensor(name) {
      this->min = min;
      this->max = max;
      this->val = min;
      char buf[strlen(name) * 2];
      sprintf(buf, "%s %d %d", name, min, max);
      this->setDef(buf);
    }
    void set(int val) {
      this->val = val;
      update();
    }
};

class FloatSensor: public Sensor {
  private:
    float min;
    float max;
    float val;
  protected:
    char* getValue() {
      return NULL;
    }
  public:
    FloatSensor(const char* name, float min, float max): Sensor(name) {
      this->min = min;
      this->max = max;
      char buf[1024];
      sprintf(buf, "%s %f %f", name, min, max);
      this->setDef(buf);
    }

    void set(float val) {
      this->val = val;
      update();
    }
};

template<int argCount>
class ValSensor: public Sensor {
  private:
    const char* options[argCount];
    int val = 0;
  protected:
    char* getValue() {
      return NULL;
    }
  public:
    ValSensor(const char* name, ...): Sensor(name) {
      char buf[1024];
      sprintf(buf, "%s ", name);


      va_list ap;
      va_start(ap, name);

      for (int i = 0; i < argCount; i++) {
        const char* opt = va_arg(ap, const char*);
        options[i] = opt;
        strcat(buf, opt);
        if (i < argCount - 1) {
          strcat(buf, ",");
        }
      }
      va_end(ap);

      setDef(buf);
    }

    void set(int val) {
      this->val = val;
      update();
    }
};

template<int argCount>
class FlagSensor: public Sensor {
  private:
    const char* options[argCount];
    int val = 0;
  protected:
    char* getValue() {
      return NULL;
    }
  public:
    FlagSensor(const char* name, ...): Sensor(name) {
      char buf[1024];
      sprintf(buf, "%s ", name);


      va_list ap;
      va_start(ap, name);

      for (int i = 0; i < argCount; i++) {
        const char* opt = va_arg(ap, const char*);
        options[i] = opt;
        strcat(buf, opt);
        if (i < argCount - 1) {
          strcat(buf, ",");
        }
      }
      va_end(ap);
      setDef(buf);
    }
    void set(int i) {
      this->val = val | (1 << i);
    }
    void unset(int i) {
      this->val = val & ~(1 << i);
    }
};

