#include <delphiSerial.h>

// built with starting point of Paul Stoffregen's OneWire http://www.pjrc.com/teensy/td_libs_OneWire.html

delphiSerial ds(10);  // on pin 10

char byteReadstr[9];

void setup(void) {
  attachInterrupt(digitalPinToInterrupt(10), isr, FALLING);
  Serial.begin(9600);
}

void loop(void) {

if(ds.incomingFrame()){
  Serial.print("read: ");
  sprintf(byteReadstr,"%08X",int(ds.readFrame()));
  Serial.println(byteReadstr);
  //ds.write(0x05);
}
}

void isr (){
  ds.read_bit();
}