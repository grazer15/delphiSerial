#include <delphiSerial.h>

// built with starting point of Paul Stoffregen's OneWire http://www.pjrc.com/teensy/td_libs_OneWire.html

delphiSerial ds(6,10);  // on pin 10

void setup(void) {
  Serial.begin(9600);
}

void loop(void) {

  
  delay(500);
  //ds.write(0b00100000);
  ds.writeFrame(1);
  //ds.write(0xFE);
  delay(500);
  ds.writeFrame(2);
  delay(500);
  ds.writeFrame(3);
  delay(100);
  ds.writeFrame(0x30d204);
  delay(100);
  ds.writeFrame(0xa34905);
 
  delay(5000);     // maybe 750ms is enough, maybe not
  Serial.println("Sending?");
           
}
