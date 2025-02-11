
#include <delphiSerial.h>

//attempt to masquerade as working CD player


delphiSerial ds(6,10);  // on pin 10
uint32_t value;
bool transport_flag;


void setup(void) {
  Serial.begin(57600);
  delay(1000);
  Serial.println("Replay script running");
}

void loop(void) {

if(ds.available_freq()){
  Serial.print("read: ");
  value = ds.readFrame_freq();
  Serial.println(value, HEX);  
}

if(value == 0x10BF84 || value == 0x30BF84){
  transport_flag = true;
}
if(value == 0x39D33 && transport_flag == false){
  ds.writeFrame(0x30D22);
  Serial.println("sent: 30D22");
}
if(value == 0xE6F && transport_flag == true){
  ds.writeFrame(0x30D22);
  Serial.println("sent: 30D22");
  transport_flag = false;
}
if(value == 0x39D12){
  ds.writeFrame(0x30D0005);
  Serial.println("sent: 30D0005");
}
if(value == 0x39B84){
  ds.writeFrame(0x30D0204);
  Serial.println("sent: 30D0204");
}
if(value == 0xE7472){
  ds.writeFrame(0x30D17);
  Serial.println("sent: 30D17");
  delay(2);
  ds.writeFrame(0x30D34B);
  Serial.println("sent: 30D34B");
  delay(2);
  ds.writeFrame(0xC34885);
  serial.println("sent: C34885");
  delay(350);
  ds.writeFrame(0xC340A);
  serial.println("sent: C340A");
  delay(2);
  ds.writeFrame(0x28D1B);
  serial.println("sent: 28D1B");
  delay(2);
  ds.writeFrame(0x28D05);
  serial.println("sent: 28D05");
  delay(2);
  ds.writeFrame(0x28D24);
  serial.println("sent: 28D24");
}
if(value == 0xE743C){
  ds.writeFrame(0x28D1B);
  serial.println("sent: 28D1B");
  delay(2);
  ds.writeFrame(0x28D05);
  serial.println("sent: 28D05");
  delay(2);
  ds.writeFrame(0x28D24);
  serial.println("sent: 28D24");
}



value = 0;

}

