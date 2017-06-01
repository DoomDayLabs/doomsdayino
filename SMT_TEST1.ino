
#include <stdarg.h>
#include "sensor.hpp"


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



template<int paramsCount>
class Trigger {
  private:
    const char* name;
    char* def;
    Param* params[paramsCount];
  public:
    Trigger(const char* name, ...) {
      va_list ap;
      va_start(ap, name);

      this->name = name;
      char buff[1024];
      sprintf(buff, "TRIGGER %s ", name);
      
      for (int i = 0; i < paramsCount; i++) {       
        Param* p = va_arg(ap,Param*);
        params[i] = p;        
        strcat(buff,p->def());
      }

      va_end(ap);
      def = (char*)(malloc(strlen(buff)));
      strcpy(def, buff);
    }

    const char* getDef() {
      return def;
    }

};






IntSensor s("SENS1", 9, 192);
FloatSensor fs("SENS2", 3.14, 5.18);
ValSensor<3>  vs("SENS3", "OPT1", "OPT2", "OPT3");
FlagSensor<3> fls("SENS4", "FLAG1", "FLAG2", "FLAG3");


/*
IntParam ip1 = IntParam(10, 20);
FloatParam fp1 = FloatParam(3.14, 5.17);
StrParam sp1 = StrParam();
ValParam<3> vp1 (OPTS{"OPT1", "OPT2", "OPT3"});
FlagParam<3> flp1(OPTS{"FLG1", "FLG2", "FLG3"});
*/    
Trigger<4> t("TRIG1", new IntParam(10, 20), new FloatParam(3.14, 5.17),new StrParam(), new ValParam<3>(OPTS{"OPT1","OPT2","OPT3"}));




void setup() {
  pinMode(PC13, OUTPUT);
  Serial.begin(115200);
}



void loop() {
  digitalWrite(PC13, LOW);
  delay(100);
  digitalWrite(PC13, HIGH);
  delay(100);
/*
  Serial.println(ip1.def());
  Serial.println(fp1.def());
  Serial.println(sp1.def());
  /*
    Serial.println(vp1.def());
    Serial.println(flp1.def());
  */
  Serial.println(t.getDef());



}
