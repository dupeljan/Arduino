#ifndef EEPROMproc_H
#define EEPROMproc_H

#include <EEPROM.h>

#define STORAGE_SIZE	5 
#define ZERO_ADDR	    0
#define SHIFT         1


class EEPROMproc{
	byte curr_addr; 			// Current adress for writing
public:
	EEPROMproc(){
		curr_addr = 0;
		EEPROM.write(ZERO_ADDR, 0);
	}
	
	void write(byte *v){			// Write key in memory
		EEPROM.put(SHIFT + curr_addr * KEY_LENGTH , v);
		curr_addr = ( curr_addr == STORAGE_SIZE - 1 )? 0 : curr_addr++;
		EEPROM.write(ZERO_ADDR, curr_addr);
	}
	
}
#endif
