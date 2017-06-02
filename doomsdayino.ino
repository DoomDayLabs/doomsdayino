#include <stdarg.h>
#include "protocol.hpp"




Endpoint e = Endpoint();
Protocol proto = Protocol(&e, new Reader(&Serial));


//IntSensor temp("TEMP",10,100);
Trigger<0> t1("T1");
Trigger<1> t2("T2", new IntParam(10, 100));



void setup() {
  pinMode(PC13, OUTPUT);
  Serial.begin(115200);
  e.addTrigger(&t1);
  e.addTrigger(&t2);
  e.setPin("111");

}



void loop() {


  proto.read();


  proto.write();

  digitalWrite(PC13, LOW);
  delay(100);
  digitalWrite(PC13, HIGH);
  delay(100);
}
