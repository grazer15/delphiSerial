
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

uint64_t byteRead = 0;
char byteReadstr[9];
bool readableByte = false;

uint32_t timeAtLastBit = 0;

void setup() {
  // put your setup code here, to run once:

  //temp -> goes in begin?
  pinMode(listenPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(10),readBit,FALLING);
  //temp
  Serial.begin(9600);
  Serial.println("starting");

}

void loop() {
  // put your main code here, to run repeatedly:

  if( readableByte && (micros() - timeAtLastBit >= 1500)){
    Serial.print("read: ");
    sprintf(byteReadstr,"%08X",byteRead);
    Serial.println(byteReadstr);
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
  timeAtLastBit = micros();
  //Serial.println(timeAtLastBit);
  readableByte = true;
  //restart a timer from last bit received?
  delayMicroseconds(300);
  //read bit value and store in an array
  r = DIRECT_READ(reg, mask);
  //Serial.print(r);
  byteRead = (byteRead<<1)  +  r;
  //if byte array is full, store within array of frame?
  interrupts();
  //Serial.print(r);
}
//temp

