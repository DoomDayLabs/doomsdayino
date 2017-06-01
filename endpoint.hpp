#include "sensor.hpp"
#include "trigger.hpp"

class Reader {
  private:
    Stream* stream;
    char buffer[1024];
    int pos = 0;
  public:
    Reader(Stream* s) {
      this->stream = s;
    }

    char* read() {
      while (stream->available() > 0) {
        char c = stream->read();
        if (c == '\n') {
          buffer[pos] = 0;
          pos = 0;
          return buffer;
        }

        buffer[pos] = c;
        pos++;
      }
      return NULL;
    }
};


typedef struct str_trigger {
  str_trigger* next;
  AbstractTrigger* t;
} str_trigger;


class Endpoint {
  private:
    char* pincode = "12345";
    Reader* reader;
    
    str_trigger* trigger;
    
    AbstractTrigger* findTrigger(const char* name) {
      str_trigger* t = trigger;

      while (1) {
        if (t->t != NULL && strcmp(name, t->t->getName()) == 0) {
          return t->t;
        }
        if (t->next != NULL) {
          t = t->next;
        }
        return NULL;
      }
    }

  void call(const char* name){
    AbstractTrigger* t = findTrigger(name);
    if (t==NULL) return;
    
    
  }
  public:
    Endpoint(Reader* r) {
      this->reader = r;
    }
    void setPin(char* pin){
      this->pincode = pin;
    }
    void read() {
      char* line = reader->read();
      if (line == NULL) return;

      char* cmd = strtok(line, " ");
      if (strcmp(cmd, "CONNECT") == 0) {
        char* pin = strtok(NULL, " ");
        if (strcmp(pin,this->pincode)==0){
          Serial.println("ACCESS OK");
        } else {
          Serial.println("ACCESS DENY");
        }
        
      } else

      if (strcmp(cmd, "CALL") == 0) {
        char* trigname = strtok(NULL, " ");
        
      } else {
        Serial.write("INCORRECT PROTO");
      }


    }
    void write() {

    }
    void process(char* cmd) {

    }
};

