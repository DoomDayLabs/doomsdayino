#include "sensor.hpp"
#include "trigger.hpp"


typedef struct str_trigger {
  str_trigger* next;
  AbstractTrigger* t;
} str_trigger;

typedef struct str_sensor {
  str_sensor* next;
  Sensor* s;
} str_sensor;

/*
   STATUS:
   OFFLINE = 0
   ONLINE = 1
   ACTIVE = 2
*/
class Endpoint {
  private:
    const char* pincode = (char*)("12345");
    const char* endpointClass = (char*)("DEVICECLASS");
    const char* endpointSerial = (char*)("DEVICESERIAL");

    str_trigger* trigger;
    str_sensor* sensor;
  public:
    char state = 1;
    void addTrigger(AbstractTrigger* t) {
      str_trigger* st = trigger;
      while (st->t != NULL) {
        if (st->next == NULL) {
          str_trigger* new_st = new(str_trigger);
          new_st->t = NULL;
          new_st->next = NULL;
          st->next = new_st;
        }
        st = st->next;
      }
      st->t = t;
    }

    void addSensor(Sensor* s) {
      str_sensor* st = sensor;
      while (st->s != NULL) {
        if (st->next == NULL) {
          str_sensor* new_st = new(str_sensor);
          new_st->s = NULL;
          new_st->next = NULL;
          st->next = new_st;
        }
        st = st->next;
      }
      st->s = s;
    }

    str_sensor* getSensor() {
      return sensor;
    }

    str_trigger* getTrigger() {
      return trigger;
    }
    Endpoint() {
      this->trigger = new (str_trigger);
      this->trigger->t = NULL;
      this->trigger->next = NULL;
      this->sensor = new (str_sensor);
      this->sensor->s = NULL;
      this->sensor->next = NULL;
      
    }
    void setPin(const char* pin) {      
      this->pincode = pin;
    }
    
    const char* getPin() {
      return this->pincode;
    }

    const char* getDevSerial(){
      return endpointSerial;
    }

    const char* getDevClass(){
      return endpointClass;
    }

    void setDevClass(const char* c){
      endpointClass = c;
    }
    void setDevSerial(const char* s){
      endpointSerial = s;
    }

    AbstractTrigger* findTrigger(const char* name) {
      str_trigger* t = trigger;

      while (1) {
        if (t->t != NULL) {
          AbstractTrigger* trig = t->t;

        }
        if (t->t != NULL && strcmp(name, t->t->getName()) == 0) {
          return t->t;
        }
        if (t->next == NULL) {
          return NULL;

        }
        t = t->next;
      }
    }

};

