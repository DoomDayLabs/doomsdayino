

//#define debug
#include "protocol.hpp"


Endpoint e = Endpoint();

Protocol proto = Protocol(&e, new Reader(&Serial));

Trigger<0> t1("T1");
Trigger<1> t2("T2", new IntParam(10, 100));
Trigger<1> t3("T3", new ValParam<3>(OPTS{"OPT1","OPT2","OPT3"}));
Trigger<1> t4("T4", new FlagParam<2>(OPTS{"FLAG1","FLAG2"}));
 

void callback1(TArg args){
    
}
void callback2(TArg args){
  Serial.println(args.asStr(0));
}

IntSensor tempe("TEMP",10,100);
StrSensor str("STR");
void setup() {
  pinMode(PC13, OUTPUT);
  Serial.begin(115200);
  e.setPin("111");
  t2.on(callback1);
  t3.on(callback2);

  e.addSensor(&tempe);
  e.addSensor(&str);
  
  e.addTrigger(&t1);
  e.addTrigger(&t2);
  
  e.addTrigger(&t4);
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
    tempe.set(counter);
    char buf[32];
    sprintf(buf,"%d",counter);
    str.set(buf);
  }
  
  
  proto.write();
  
  digitalWrite(PC13, LOW);
  
  digitalWrite(PC13, HIGH);
  
}
