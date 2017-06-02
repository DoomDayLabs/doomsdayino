
#include "protocol.hpp"


Endpoint e = Endpoint();

Protocol proto = Protocol(&e, new Reader(&Serial));

Trigger<0> t1("T1");
Trigger<1> t2("T2", new IntParam(10, 100));
Trigger<1> t3("T3", new ValParam<3>(OPTS{"OPT1","OPT2","OPT3"}));
 

void callback1(TArg args){
  Serial.println("CALLBACK");
  Serial.print(args.asInt(0));
}

IntSensor tempe("TEMP",10,100);
void setup() {
  pinMode(PC13, OUTPUT);
  Serial.begin(115200);
  e.setPin("111");
  t2.on(callback1);

  e.addSensor(&tempe);
  
  e.addTrigger(&t1);
  e.addTrigger(&t2);
  e.addTrigger(&t3);
  
  
}
int counter = 10;
int lastTime = millis();
void loop() {
  
  proto.read();

  int t = millis();
  if (t-lastTime>=2000){
    counter++;
    lastTime = t;
    //tempe.set(counter);
  }
  
  
  proto.write();
  
  digitalWrite(PC13, LOW);
  delay(100);
  digitalWrite(PC13, HIGH);
  delay(100);
}
