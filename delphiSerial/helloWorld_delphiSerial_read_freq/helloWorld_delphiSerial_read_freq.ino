
#include <delphiSerial.h>

// built with starting point of Paul Stoffregen's OneWire http://www.pjrc.com/teensy/td_libs_OneWire.html

delphiSerial ds(6,10);  // on pin 10

char byteReadstr[9];
uint32_t value;

void setup(void) {
  Serial.begin(57600);
  delay(1000);
  Serial.println("Listening");
}

void loop(void) {

if(ds.available_freq()){
  Serial.print("read: ");
  value = ds.readFrame_freq();
  Serial.println(value, HEX);
}
}

