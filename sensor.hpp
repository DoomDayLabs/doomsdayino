#include <stdarg.h>
class Sensor {
  private:
    const char* name;
    char* def;
    bool changed;
  protected:
    void update() {
      changed = true;
    }
  public:
    Sensor(const char* name) {
      this->name = name;
    };

    const char* getName() {
      return name;
    }
    void setDef(char* def) {
      this->def = (char*)(malloc(strlen(def) + 1));
      strcpy(this->def, def);
    }
    char* getDef() {
      return this->def;
    }

    void fresh() {
      changed = false;
    }

    bool isChanged() {
      return changed;
    }

    virtual void putValue(char* buf) {
      buf[0] = 0;
    }
};

class IntSensor: public Sensor {
  private:
    int min = 0;
    int max = 0;
    int val = 0;

  public:
    IntSensor(const char* name, int min, int max): Sensor(name) {
      this->min = min;
      this->max = max;
      this->val = min;

      char buf[1024];
      sprintf(buf, "SENSOR %s INT %d %d", name, min, max);
      this->setDef(buf);

    }
    void set(int val) {
      if (val < min) val = min;
      if (val > max) val = max;
      this->val = val;
      update();
    }


    void putValue(char* buffer) {
      sprintf(buffer, "%d", val);
    }
};

class FloatSensor: public Sensor {
  private:
    float min;
    float max;
    float val;
  protected:

  public:
    FloatSensor(const char* name, float min, float max): Sensor(name) {
      this->min = min;
      this->max = max;
      char buf[1024];
      sprintf(buf, "SENSOR %s FLOAT %f %f", name, min, max);
      this->setDef(buf);
    }

    void set(float val) {
      if (val < min) val = min;
      if (val > max) val = max;
      this->val = val;
      update();
    }

    void putValue(char* buff) {
      sprintf(buff, "%f", val);
    }
};

class StrSensor: public Sensor {
  private:
    char* val = NULL;
  public:
    StrSensor(const char* name): Sensor(name) {
      char buf[1024];
      sprintf(buf, "SENSOR %s STR", name);
      this->setDef(buf);
      this->val = (char*)malloc(8);
    }

    void set(const char* v) {
      free(val);
      val = (char*)(malloc(strlen(v)));
      strcpy(val, v);
      update();
    }

    void putValue(char* buf) {
      strcpy(buf, val);
    }
};

class BoolSensor: public Sensor {
  private:
    bool val = false;
  public:
    BoolSensor(const char* name): Sensor(name) {
      char buf[1024];
      sprintf(buf, "SENSOR %s BOOL", name);
      this->setDef(buf);
    }

    void set(const bool v) {
      val = v;
      update();
    }

    void putValue(char* buf) {
      if (this->val == true)
        strcpy(buf, "TRUE");
      else
        strcpy(buf, "FALSE");
    }
};
template<int argCount>
class ValSensor: public Sensor {
  private:
    const char* options[argCount];
    int val = 0;
  protected:

  public:
    ValSensor(const char* name, ...): Sensor(name) {
      char buf[1024];
      sprintf(buf, "SENSOR %s VAL ", name);


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

    void putValue(char* buf) {
      const char* option = options[val];
      strcpy(buf, option);
    }
};

template<int argCount>
class FlagSensor: public Sensor {
  private:
    const char* options[argCount];
    int val = 0;
  protected:

  public:
    FlagSensor(const char* name, ...): Sensor(name) {
      char buf[1024];
      sprintf(buf, "SENSOR %s FLAG ", name);


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
      update();
    }
    void unset(int i) {
      this->val = val & ~(1 << i);
      update();
    }

    char* getValue() {
      return NULL;
    }
};

