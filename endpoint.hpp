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
 * STATUS:
 */
class Endpoint {
  private:
    char* pincode = "12345";
    char* endpointClass = "";
    char* endpointSerial = "";
    char state = 0;
    str_trigger* trigger;
    str_sensor* sensor;
  public:
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

    str_sensor* getSensor(){
      return sensor;
    }

    str_trigger* getTrigger(){
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
    void setPin(char* pin) {
      this->pincode = pin;
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

