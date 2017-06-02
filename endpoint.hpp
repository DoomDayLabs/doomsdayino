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

class Endpoint {
  private:
    char* pincode = "12345";

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

    }
    Endpoint() {
      this->trigger = new (str_trigger);
      this->trigger->t = NULL;
      this->trigger->next = NULL;
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

