// built with starting point of Paul Stoffregen's OneWire http://www.pjrc.com/teensy/td_libs_OneWire.html

#include <Arduino.h>
#include "delphiSerial.h"
#include <FreqMeasureMulti.h>

#define CRIT_TIMING 

void delphiSerial::begin(uint8_t read_pin, uint8_t write_pin)
{
	pinMode(write_pin, INPUT);
	bitmask = PIN_TO_BITMASK(write_pin);
	baseReg = PIN_TO_BASEREG(write_pin);
        frameFinished = 0;
        frameValue = 0;
        frameToRead = 0;
	timeout = 0;
        freqReader.begin(read_pin, FREQMEASUREMULTI_SPACE_ONLY);
}

void CRIT_TIMING delphiSerial::write_bit(uint8_t v)
{
	IO_REG_TYPE mask IO_REG_MASK_ATTR = bitmask;
	__attribute__((unused)) volatile IO_REG_TYPE *reg IO_REG_BASE_ATTR = baseReg;

	if (v & 1) {
		noInterrupts();
		DIRECT_WRITE_LOW(reg, mask);
		DIRECT_MODE_OUTPUT(reg, mask);	// drive output low
		delayMicroseconds(100);
		DIRECT_WRITE_HIGH(reg, mask);	// drive output high
		interrupts();
		delayMicroseconds(875);
	} else {
		noInterrupts();
		DIRECT_WRITE_LOW(reg, mask);
		DIRECT_MODE_OUTPUT(reg, mask);	// drive output low
		delayMicroseconds(650);
		DIRECT_WRITE_HIGH(reg, mask);	// drive output high
		interrupts();
		delayMicroseconds(325);
	}
}

void delphiSerial::write(uint8_t v) {
    uint8_t bitMask;

    for (bitMask = 0x01; bitMask; bitMask <<= 1) {
	    delphiSerial::write_bit( (bitMask & v)?1:0);
    }
    
	  //noInterrupts();
	  //DIRECT_MODE_INPUT(baseReg, bitmask);
	  //DIRECT_WRITE_LOW(baseReg, bitmask);
	  //interrupts();
   
}

void CRIT_TIMING delphiSerial::read_bit(){
    
    IO_REG_TYPE mask IO_REG_MASK_ATTR = bitmask;
    __attribute__((unused)) volatile IO_REG_TYPE *reg IO_REG_BASE_ATTR = baseReg;    

    uint8_t r;

    noInterrupts();
    timeAtLastBit = micros();
    frameStarted = true;
 
    delayMicroseconds(300);
    
    //read bit value and store in an array
    r = DIRECT_READ(reg, mask);
    //Serial.print(r);
    frameValue = (frameValue<<1)  +  r;
    
    interrupts();
}

bool delphiSerial::incomingFrame(){
    if(frameStarted && (micros() - timeAtLastBit >= 2500)){
	frameToRead = frameValue;
        frameValue = 0;
        frameStarted = false;
	return true;
    }
    else{
	return false;
    }
}

uint32_t delphiSerial::readFrame(){
    return frameToRead;
}

bool delphiSerial::available_freq(){
    if(freqReader.available()){
    	frameStarted = true;
	timeout = 0;
	//TODO Add to frame
	uint32_t r = 0;
	r = freqReader.read();
	if (r < 50000){
		r = 1;
	}
	else if (r >50000 && r <200000){
		r = 0;
	}	
	else{
		return false;
	}
	frameValue = (frameValue<<1)  +  r;

	return false;
  
    }
    else if(timeout > 20 && frameStarted == true){
	frameToRead = frameValue;
        frameValue = 0;
        frameStarted = false;
	return true;
    }
    else{
	return false;
    }
}

uint32_t delphiSerial::readFrame_freq(){
	return frameToRead;
}




