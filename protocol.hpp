#include "endpoint.hpp"

#ifdef debug
#define PRINT(a) Serial.print(a)
#define PRINTLN(a) Serial.println(a)
#else
#define PRINT(a)
#define PRINTLN(a)
#endif

class Reader {
  private:
    Stream* stream = NULL;
    char buffer[1024];
    int pos = 0;
  public:

    Reader() {

    }
    void setStream(Stream* s) {
      this->stream = s;
      pos = 0;
    }

    char* read() {
      if (stream == NULL)
        return NULL;
      PRINT("READ:");
      while (stream->available() > 0) {
        char c = stream->read();
        PRINT(c);
        if (c == '\r'){
          continue;
        }
        
        if (c == '\n') {
          buffer[pos] = 0;
          if (pos>0){
    	    PRINTLN("Readed ");
    	    PRINTLN(buffer);
    	  }
          pos = 0;

          return buffer;
        }
        buffer[pos] = c;
        pos++;
      }      
      if (pos>0){
        PRINTLN("Readed ");
        PRINTLN(buffer);
      }
      return NULL;
      
    }

};

class Protocol {
  private:
    Endpoint* endpoint;
    Reader reader;
    Stream* stream;

    void callTrigger() {
      char* triggerName = strtok(NULL, " ");
      PRINT("Trigger=");
      PRINTLN(triggerName);

      AbstractTrigger* trigger = endpoint->findTrigger(triggerName);
      if (trigger == NULL) return;
      PRINTLN("Trigger found");

      int paramCount = trigger->getParamsCount();
      char* strParams[paramCount];

      PRINTLN("PARAMS:");
      PRINTLN(paramCount);
      for (int i = 0; i < paramCount; i++) {
        char* param = strtok(NULL, " ");
        if (param == NULL){
    	    return;
        }
        strParams[i] = param;
        PRINT(i);
        PRINT("=");
        PRINTLN(param);
      }

      PRINTLN("VALIDATE");
      for (int i = 0; i < paramCount; i++) {
        PRINTLN(i);


        if (!trigger->validateParam(i, strParams[i])) {
          PRINT("INVALID PARAM ");
          PRINTLN(i);
          return;
        }
      }

      PRINTLN("VALID PARAMS");
      TArg args = TArg(0, strParams, trigger->getParams());
      trigger->call(args);





    }

    void writeSensorDefs() {

      str_sensor* ss = endpoint->getSensor();

      while (ss->s != NULL) {
        Sensor* sensor = ss->s;
        //reader->str()->println(
        stream->println(sensor->getDef());

        if (ss->next == NULL)
          break;
        ss = ss->next;
      }

    }

    void writeTriggerDefs() {

      str_trigger* ss = endpoint->getTrigger();

      while (ss->t != NULL) {
        AbstractTrigger* t = ss->t;

        stream->println(t->getDef());

        if (ss->next == NULL)
          break;
        ss = ss->next;
      }

    }
  public:
    Protocol(Endpoint* e, Stream* s) {
      this->endpoint = e;
      setStream(s);
    }

    void setStream(Stream* s) {
      this->reader.setStream(s);
      this->stream = s;
      if (s!=NULL){
          this->stream->println("HELO");
      }
    }

    void read() {
      if (stream == NULL)
        return;
      char* cmd = reader.read();      
      if (cmd == NULL) return;      
      PRINT("COMMAND ");
      PRINTLN(cmd);
      char* command = strtok(cmd, " ");

      if (strcmp(command, "CONNECT") == 0) {
        if (endpoint->state == 1) {
          char* pin = strtok(NULL, " ");
          if (strcmp(endpoint->getPin(), pin) == 0) {
            stream->println("ACCEPT");
            endpoint->state = 2;
            /*
            
            */
          } else {
            stream->println("REJECT");
          }
        } else {
          stream->println("STATE ERROR");
        }
        
      } else if (strcmp(command, "PROFILE") == 0){
        if (endpoint->state == 2){
          writeSensorDefs();
          writeTriggerDefs();
          stream->println("READY");
          endpoint->state = 3;
          writeSensors(true);          
        } else {
          stream->println("STATE ERROR");
        }
        
        
      } else if (strcmp(command, "READY") == 0){
        if (endpoint->state == 2){
          stream->println("READY");
          endpoint->state = 3;
          writeSensors(true);       
        } else {
          stream->println("STATE ERROR");
        }
                
      } else if (strcmp(command, "CALL") == 0) {
        if (endpoint->state == 3) {
          callTrigger();
        } else {
          stream->println("STATE ERROR");
        }

      } else {
        stream->println(cmd);
        stream->println("PROTO ERROR");
      }
    }


    void writeSensors(bool always=false) {

      str_sensor* ss = endpoint->getSensor();

      while (ss->s != NULL) {
        Sensor* sensor = ss->s;

        if (sensor->isChanged()||always) {
          char buffer[512];
          sensor->putValue(buffer);
          sensor->fresh();
          stream->print("SET ");
          stream->print(sensor->getName());
          stream->print(" ");
          stream->println(buffer);
        }

        if (ss->next == NULL)
          break;
        ss = ss->next;

      }

    }

    void write() {
      if (stream == NULL)
        return;
      if (endpoint->state == 3) {
        writeSensors();
      }
    }
};

