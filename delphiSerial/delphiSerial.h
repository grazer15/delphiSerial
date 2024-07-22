#ifndef delphiSerial_h
#define delphiSerial_h

#ifdef __cplusplus

#include <stdint.h>
#include <FreqMeasureMulti.h>

//Defines from OneWire_Direct_GPIO.h for teensy 4.0
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
//end defines from Onewire_direct_gpio


class delphiSerial
{
  private:
    IO_REG_TYPE bitmask;
    volatile IO_REG_TYPE *baseReg;

    uint32_t frameValue;
    uint32_t frameToRead;
    uint32_t timeAtLastBit;
    bool frameStarted;
    bool frameFinished;
    elapsedMillis timeout;
    FreqMeasureMulti freqReader;


    
    
  public:
    delphiSerial() { }
    delphiSerial(uint8_t read_pin, uint8_t write_pin) { begin(read_pin, write_pin); }
    void begin(uint8_t read_pin, uint8_t write_pin);
    void write_bit(uint8_t v);
    void write(uint8_t v);
    //void write_bytes(const uint8_t *buf, uint16_t count);
    
    bool incomingFrame();
    uint32_t readFrame();
    void read_bit();

    bool available_freq();
    uint32_t readFrame_freq();

};

#endif // __cplusplus
#endif //delphiSerial