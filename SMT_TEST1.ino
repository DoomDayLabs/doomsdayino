#include <stdarg.h>
#include "sensor.hpp"
#include "trigger.hpp"




Trigger<4> t("TRIG1", new IntParam(10, 20), new FloatParam(3.14, 5.17), new StrParam(), new ValParam<3>(OPTS{"OPT1", "OPT2", "OPT3"}));




void setup() {
  pinMode(PC13, OUTPUT);
  Serial.begin(115200);
}

class Buffer {
  private:
    Stream* stream;
    char buffer[1024];
    int pos = 0;
  public:
    Buffer(Stream* s) {
      this->stream = s;
    }
    
    char* read() {
      while(stream->available() >0){
        char c = stream->read();
        if (c == '\n'){                     
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

Buffer buff = Buffer(&Serial);

void process(char* line){
    char* tok = strtok(line," ");
    Serial.write(tok);    
    if (strcmp(tok,"CONNECT")==0){
      char* pin = strtok(NULL," ");
      Serial.write("CONNECT PIN=");
      Serial.write(pin);
    }

    if (strcmp(tok,"CALL")==0){
      
    }
}

void loop() {
  char* c = NULL;
  if ((c = buff.read()) != NULL){
    process(c);
    Serial.write(c);
    Serial.write("!");
  }
  /*
  if (int avail = Serial.available() > 0) {
    char buffer[1024];
    int i = 0;
    while ((Serial.available() > 0)) {
      char c = Serial.read();
      if (c == '\n')
        break;

      buffer[i] = c;
      i++;
    }
    buffer[i] = 0;
    Serial.write(buffer);
  }
  */

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
  //Serial.println(t.getDef());



}
