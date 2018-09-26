/***
  Writing card keys in EEPROM memory
  Presentation of data:

   Byte 0       Byte 1        Byte 2 .. card_count * key_lenght + 2
  |------------|------------|------------|..
  | Card count | Key length | Key_value  |...          
  |------------|------------|------------|...
***/

#include <EEPROM.h>
#include "cards_list.h"

#define RESERVED_BYTES 2
 

void setup() {

  Serial.begin(9600);

  if ( EEPROM.length() < CARD_COUNT * KEY_LENGTH + RESERVED_BYTES )
     Serial.print( "Error: EEPROM overflow" );
  else{
   
    //EEPROM.clear();
    EEPROM.write(0,CARD_COUNT);
    EEPROM.write(1,KEY_LENGTH);
    for ( int i = RESERVED_BYTES; i < EEPROM.length(); i++ ){
      EEPROM.write(i,0);
    }
  
    EEPROM.put(RESERVED_BYTES, cards_list);
  
    Serial.print( "Success writing " );
    Serial.print( sizeof(cards_list) / KEY_LENGTH );
    Serial.println( "card keys" );

    for ( int i = 0; i < CARD_COUNT * KEY_LENGTH + RESERVED_BYTES; i++ ){
      int x = EEPROM.read(i);
      Serial.println(x);
    
    }
  }
}

void loop() {
  /* Empty loop */
  
}
