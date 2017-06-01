#include <stdarg.h>
#include "endpoint.hpp"




//Trigger<4> t("TRIG1", new IntParam(10, 20), new FloatParam(3.14, 5.17), new StrParam(), new ValParam<3>(OPTS{"OPT1", "OPT2", "OPT3"}));
Trigger<0> t1("TRIG1");
Endpoint e = Endpoint(new Reader(&Serial));



void setup() {
  pinMode(PC13, OUTPUT);
  Serial.begin(115200);
}



void loop() {
  e.setPin("111");
  e.read();
  
  e.write();

  digitalWrite(PC13, LOW);
  delay(100);
  digitalWrite(PC13, HIGH);
  delay(100);



}
