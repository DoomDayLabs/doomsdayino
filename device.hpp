#include "protocol.hpp"

IntSensor temp = IntSensor("TEMP",10,100);
Trigger<0> t1("T1");
Trigger<1> t2("T2",new IntParam(10,100));
Trigger<0> t3("T3");

void setup(Endpoint* endpoint){
  endpoint->addSensor(&temp);
  endpoint->addTrigger(&t1);
  endpoint->addTrigger(&t2);  
}

void loop(Endpoint* endpoint){
    
}

