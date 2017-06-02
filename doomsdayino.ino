#include <stdarg.h>
#include "device.hpp"







Endpoint e = Endpoint();
Protocol proto = Protocol(&e, new Reader(&Serial));

void callback1(){
  Serial.println("!!CALLBACK1!!");
}

void setup() {
  pinMode(PC13, OUTPUT);
  Serial.begin(115200);
  setup(&e);
  e.setPin("111");
}



void loop() {
  
  
  proto.read();

  loop(&e);
  proto.write();

  digitalWrite(PC13, LOW);
  delay(100);
  digitalWrite(PC13, HIGH);
  delay(100);
}
