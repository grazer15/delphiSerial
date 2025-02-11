// built with starting point of Paul Stoffregen's OneWire for bitbanging http://www.pjrc.com/teensy/td_libs_OneWire.html

#include <Arduino.h>
#include "delphiSerial.h"
#include <FreqMeasureMulti.h>

#define CRIT_TIMING 

void delphiSerial::begin(uint8_t read_pin, uint8_t write_pin)
{
	pinMode(write_pin, OUTPUT);
	bitmask = PIN_TO_BITMASK(write_pin);
	baseReg = PIN_TO_BASEREG(write_pin);
        frameValue = 0;
        frameToRead = 0;
	timeout = 0;
        freqReader.begin(read_pin, FREQMEASUREMULTI_SPACE_ONLY);
}

void CRIT_TIMING delphiSerial::write_bit(uint8_t v)
{
	IO_REG_TYPE mask IO_REG_MASK_ATTR = bitmask;
	__attribute__((unused)) volatile IO_REG_TYPE *reg IO_REG_BASE_ATTR = baseReg;
	
	//Defining short ground pulse as 1 (as defined in single wire serial)
	//if( v & 1) {
        //Defining short ground pulse as 0 (as SOF is always long pulse, so can send entire frame without leading "zeros"/pulses) 
	if (!(v & 1)) {
		noInterrupts();
		//Inverting logic levels to be able to connect switch normally open (avoid shorting bus if switch fails)
		//DIRECT_WRITE_LOW(reg, mask);
		//DIRECT_MODE_OUTPUT(reg, mask);	// drive output low
		DIRECT_WRITE_HIGH(reg,mask); // drive output high
		delayMicroseconds(100);
		//Inverting logic levels to be able to connect switch normally open (avoid shorting bus if switch fails)
		//DIRECT_WRITE_HIGH(reg, mask);	// drive output high
		DIRECT_WRITE_LOW(reg, mask);
		DIRECT_MODE_OUTPUT(reg, mask);	// drive output low
		interrupts();
		delayMicroseconds(875);
	} else {
		noInterrupts();
		//Inverting logic levels to be able to connect switch normally open (avoid shorting bus if switch fails)
		//DIRECT_WRITE_LOW(reg, mask);
		//DIRECT_MODE_OUTPUT(reg, mask);	// drive output low
		DIRECT_WRITE_HIGH(reg,mask); // drive output high
		delayMicroseconds(650);
		//Inverting logic levels to be able to connect switch normally open (avoid shorting bus if switch fails)
		//DIRECT_WRITE_HIGH(reg, mask);	// drive output high
		DIRECT_WRITE_LOW(reg, mask);
		DIRECT_MODE_OUTPUT(reg, mask);	// drive output low
		interrupts();
		delayMicroseconds(325);
	}
}

//write() OLD and not applicable
/*
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
*/

void delphiSerial::writeFrame(uint32_t v){
    uint32_t bitMask;
    
    //longest frame observed is 26 bits, so storing frame in unsigned 32bit integer
    //Need to detect first 1 in field, so that leading "zeros" are not sent on bus
    bool sof = false;
    
    for(bitMask = 0x80000000; bitMask; bitMask>>=1){
        if((!(bitMask & v)) && !sof){
            continue;
        }
        else{
            sof = true;
            delphiSerial::write_bit((bitMask &v)?1:0);  
        }
    }
    
    //clear measurements taken by reader during transmission (may be better to turn off reader during write using "end" function if seeing strange edge cases? problem is this 
    //function cannot currently see what pin freqreader is defined for..)
    while(freqReader.available()){
      frameStarted = false;
      timeout = 0;
      freqReader.read();
    }
}

//read_bit() OLD and not functional
/*
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
}*/

//incomingFrame() OLD and not functional
/*
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
*/

//readFrame() OLD and not functional
/*
uint32_t delphiSerial::readFrame(){
    return frameToRead;
}
*/

bool delphiSerial::available_freq(){
    if(freqReader.available()){
    	frameStarted = true;
	timeout = 0;
	uint32_t r = 0;
	r = freqReader.read();
	if (r < 50000){
	        //Defining short ground pulse as 1 (as defined in single wire serial)
		//r = 1;
		//Defining short ground pulse as 0 (as SOF is always long pulse, so can capture entire frame)
		r = 0;
	}
	else if (r >50000 && r <200000){
		//Defining long ground pulse as 0 (as defined in single wire serial)
		//r = 0;
		//Defining long ground pulse as 1 (as SOF is always long pulse, so can capture entire frame) 
		r = 1;
	}	
	else{
		return false;
	}
	frameValue = (frameValue<<1)  +  r;

	return false;
  
    }
    else if(timeout > 2000 && frameStarted == true){
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




