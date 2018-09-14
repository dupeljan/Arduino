/*
 * 
 * Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 * 
 * Pin layout for led strip
 * -----------------------------------------------------------------------------------------
 * DIN         DIN                                  D8
 */

#include <SPI.h>
#include <MFRC522.h>
#include <PololuLedStrip.h>
#include "cards_list.h"


#define RST_PIN         9          // Configurable, see typical pin layout above
#define SS_PIN          10         // Configurable, see typical pin layout above
#define STR_DIN_PIN     8          // Data input pin for led strip
#define LED_COUNT       60
#define DELAY           3000

//Program states
enum states{
  closing,
  waiting,
  reading,
  access,
  refusal,
} state = closing;

// Instances
MFRC522 mfrc522(SS_PIN, RST_PIN);     // Create MFRC522 instance
PololuLedStrip<STR_DIN_PIN> ledStrip; // Create ledStrip instance

// Buffer for holding collor
rgb_color colors[LED_COUNT];

// Functions
void set_strip_color(states state);
bool card_in_list(byte* card);

void setup() {
  Serial.begin(9600);   // Initialize serial communications with the PC
  SPI.begin();          // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522
  set_strip_color(waiting);
  
}

void loop() {

  switch ( state ) {

    case waiting:
      //Serial.println("waiting");
      if ( mfrc522.PICC_IsNewCardPresent() )
         state = reading;
      
       break;

    case reading:
      Serial.println("reading");
      mfrc522.PICC_ReadCardSerial();
      for (byte i = 0; i < mfrc522.uid.size; i++){
        //Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
        Serial.println(mfrc522.uid.uidByte[i]/*, HEX*/);
      }
      Serial.println();
      // if mfrc.uid in list
      if ( card_in_list( mfrc522.uid.uidByte ) )
          state = access;
      else
         state = refusal; 
      break;

    case access:
      Serial.println("access");
      //led to access mode
      set_strip_color(access);
      
      //Open lock
      
      //take a brake
      delay(DELAY);
      
      state = closing;
     break;

    case refusal:
      Serial.println("refusal");
      
      //led to refusal
      set_strip_color(refusal);
      
      //take a break
      delay(DELAY / 3);
      
      state = closing;
    break;

    case closing:
      Serial.println("closing");
      
      //lock on
      
      //led waiting mode
      set_strip_color(waiting);
      
      state = waiting;
    break;
  }
 
 /* // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }
*/
  // Dump debug info about the card; PICC_HaltA() is automatically called
  //mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
  
 /*
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     //Serial.print(" ");
    // content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     //content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
 */
}

// Led strip handling
void set_strip_color(states state){
  rgb_color color;
  switch ( state ) {
     
    case access:
      color.red = 0;
      color.green = 255;
      color.blue = 0;
     break;
     
     case refusal:
      color.red = 255;
      color.green = 0;
      color.blue = 0;
     break;

     case waiting:
      color.red = 0;
      color.green = 0;
      color.blue = 255;
     break;
  }

  // Update the colors buffer.
  for(uint16_t i = 0; i < LED_COUNT; i++){
       colors[i] = color;
  }

  // Write to the LED strip.
  ledStrip.write(colors, LED_COUNT);
  
}

// Search card in list
bool card_in_list(byte* card){
  bool result = false;
  for ( byte i = 0; i < CARD_NUMB && !result; i++){
      result = true;
      for ( byte j = 0; j < KEY_LENGTH && result; j++)
        result = card[j] == cards_list[i][j];
  }
  return result;
}
