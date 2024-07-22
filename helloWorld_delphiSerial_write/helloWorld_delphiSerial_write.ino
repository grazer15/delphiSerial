#include <delphiSerial.h>

// built with starting point of Paul Stoffregen's OneWire http://www.pjrc.com/teensy/td_libs_OneWire.html

delphiSerial ds(6,10);  // on pin 10

void setup(void) {
  Serial.begin(9600);
}

void loop(void) {

  ds.write(0b10000000);
  ds.write(0b01000000);
  delay(5000);
  ds.write(0b00001111);
  delay(5000);
  ds.write(0b11000000);
  delay(5000);
  ds.write(0b00100000);
  //ds.write(0xFE);
  
  delay(5000);     // maybe 750ms is enough, maybe not
  //Serial.println("Sending?");
           
}
