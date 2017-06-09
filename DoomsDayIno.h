#include "protocol.hpp"

#ifndef DOOMSDAYINO_USE_AS_LIB
void setup(Endpoint* e, Protocol* proto);
void loop(Endpoint* e, Protocol* proto);

Endpoint endpoint = Endpoint();
Protocol proto = Protocol(&endpoint, NULL);

void setup(){
  setup(&endpoint,&proto);
}

void loop(){
  proto.read();
  loop(&endpoint,&proto);
  proto.write();
}
#endif
