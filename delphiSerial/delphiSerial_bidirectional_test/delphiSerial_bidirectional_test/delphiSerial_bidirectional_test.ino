
#include <delphiSerial.h>

//Test for checking read and write capability of hardware
//Meant for two teensys connected to each other
//Correct test counts up continously


delphiSerial ds(6,10);  // on pin 10
uint32_t value;

void setup(void) {
  Serial.begin(57600);
  delay(1000);
  Serial.println("Bidirectional Test. Writing Frame 0x1, then listening");
  ds.writeFrame(1);
}

void loop(void) {

if(ds.available_freq()){
  Serial.print("read: ");
  value = ds.readFrame_freq();
  Serial.println(value, HEX);
 
  delay(1000);

  ds.writeFrame(value +1);
  Serial.print("sent: ");
  Serial.println(value+1,HEX);
  
}
}

