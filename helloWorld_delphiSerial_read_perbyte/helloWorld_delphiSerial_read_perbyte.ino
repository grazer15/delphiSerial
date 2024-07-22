
//temp
#define PIN_TO_BASEREG(pin)             (portOutputRegister(pin))
#define PIN_TO_BITMASK(pin)             (digitalPinToBitMask(pin))
#define IO_REG_TYPE uint32_t
#define IO_REG_BASE_ATTR
#define IO_REG_MASK_ATTR
#define DIRECT_READ(base, mask)         ((*((base)+2) & (mask)) ? 1 : 0)
#define DIRECT_MODE_INPUT(base, mask)   (*((base)+1) &= ~(mask))
#define DIRECT_MODE_OUTPUT(base, mask)  (*((base)+1) |= (mask))
#define DIRECT_WRITE_LOW(base, mask)    (*((base)+34) = (mask))
#define DIRECT_WRITE_HIGH(base, mask)   (*((base)+33) = (mask))
//temp


const byte listenPin = 10;
volatile byte state = 0;

//temp
uint32_t bitmask = PIN_TO_BITMASK(listenPin);
uint32_t baseReg = PIN_TO_BASEREG(listenPin);
//temp

uint8_t byteRead = 0;
uint8_t bitSpot = 0;
bool readableByte = false;

void setup() {
  // put your setup code here, to run once:

  //temp -> goes in begin?
  pinMode(listenPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(10),readBit,FALLING);
  //temp
  Serial.println("starting");

}

void loop() {
  // put your main code here, to run repeatedly:

  if(readableByte){
    Serial.print("read: ");
    Serial.println(byteRead,HEX);
    readableByte = false;
    byteRead = 0;
  }
}

//temp
void readBit (){
  IO_REG_TYPE mask IO_REG_MASK_ATTR = bitmask;
	__attribute__((unused)) volatile IO_REG_TYPE *reg IO_REG_BASE_ATTR = baseReg;


  uint32_t r;
  //uint32_t time;
  noInterrupts();
  //time = micros();
  //restart a timer from last bit received?
  delayMicroseconds(300);
  //read bit value and store in an array
  r = DIRECT_READ(reg, mask);
  //Serial.print(r);
  byteRead = byteRead +  (r<<bitSpot);
  if(bitSpot == 7){
    bitSpot = 0;
    //Serial.println("reset");
    readableByte = true;
  }
  else{
    bitSpot+=1;
    //Serial.print(bitSpot);
    //Serial.print(" ");
  }
 
  //if byte array is full, store within array of frame?
  interrupts();
  //Serial.print(r);
}
//temp

