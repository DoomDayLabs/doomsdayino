#include "endpoint.hpp"

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

class Protocol {
  private:
    Endpoint* endpoint;
    Reader* reader;

    void callTrigger(){
      char* triggerName = strtok(NULL," ");
      Serial.print("Trigger=");
      Serial.println(triggerName);
      
      AbstractTrigger* trigger = endpoint->findTrigger(triggerName);
      if (trigger==NULL) return;
      Serial.println("Trigger found");
      //trigger->call();
      int paramCount = trigger->getParamsCount();
      char* strParams[paramCount];

      Serial.println("PARAMS:");
      Serial.println(paramCount);
      for (int i=0;i<paramCount;i++){
        char* param = strtok(NULL," ");
        strParams[i] = param;
        Serial.print(i);
        Serial.print("=");
        Serial.println(param);  
      }
      Serial.println("VALIDATE");
      for (int i=0;i<paramCount;i++){
        Serial.println(i);
        
                 
        if (!trigger->validateParam(i,strParams[i])){
          Serial.print("INVALID PARAM ");
          Serial.println(i);
          return;
        }        
      }

      
      
      
      
    }
  public:
    Protocol(Endpoint* e, Reader* r) {
      this->endpoint = e;
      this->reader = r;
    }
  
    void read(){
      char* cmd = reader->read();
      if (cmd == NULL) return;
      Serial.println(cmd);
      char* command = strtok(cmd," ");
      
      if (strcmp(command,"CALL")==0){
        Serial.println("CALL CMD");
        callTrigger();
      } else {
        Serial.println("PROTO ERROR");
      }
      
    }

    void write(){
      
    }
};

