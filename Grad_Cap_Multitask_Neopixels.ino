#include "NeoPatterns.h"
#include <EEPROM.h>

void LongBoiComplete();

// Define some NeoPatterns for the two rings and the stick
//  and pass the adderess of the associated completion routines
NeoPatterns LongBoi(60, 2, NEO_GRB + NEO_KHZ800, &LongBoiComplete);

// Initialize everything and prepare to start
void setup() {

  // Read current EEPROM value and seed current show
  randomSeed(EEPROMReadlong(0));

  // Generate and Write New Long to EEPROM
  EEPROMWritelong(0, random());

  Serial.begin(9600);      // open the serial port at 9600 bps:


  // Initialize all the pixelStrips
  LongBoi.begin();

  // Enable internal pullups on the switch inputs
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);

  // Kick off a pattern
  LongBoiComplete();

}

// Main loop
void loop() {
  // Update the rings.
  LongBoi.Update();
}

//------------------------------------------------------------
//Completion Routines - get called on completion of a pattern
//------------------------------------------------------------

// Ring 2 Completion Callback
void LongBoiComplete() {

  switch(random(0, 8)) {

    case 0:
      LongBoi.RainbowCycle(random(0, 20));
      if (randBool()){
        LongBoi.Reverse();
      }
      break;

    case 1:
      LongBoi.SingleRandom(randColor(), 20, random(50, 300), randBool());
      break;

    case 2:
      LongBoi.PersistentRandom(randColor(), 20, random(50, 300), randBool());
      break;

    case 3:
      LongBoi.Fade(randColor(), randColor(), 200, random(5, 15));
      if (randBool()){
        LongBoi.Reverse();
      }
      break;

    case 4:
      LongBoi.TheaterChase(randColor(), LongBoi.Color(0,0,0), random(50, 100));
      if (randBool()){
        LongBoi.Reverse();
      }
      break;

    case 5:
      LongBoi.Scanner(randColor(), random(10, 50));
      if (randBool()){
        LongBoi.Reverse();
      }
      break;

    case 6:
      LongBoi.ColorWipe(randColor(), random(10, 50));
      if (randBool()){
        LongBoi.Reverse();
      }
      break;

    case 7:
      LongBoi.AllRandom(random(50, 100), random(20, 50));
      break;

    default:
    break;

  }

}

bool randBool(){

  if (random(0, 2) == 1){
    return true;
  }
  return false;
}

uint32_t randColor(){

  switch(random(0, 4)) {

    case 0:
      return LongBoi.Color(255,0,0);
      break;

    case 1:
      return LongBoi.Color(0,255,0);
      break;

    case 2:
      return LongBoi.Color(0,0,255);
      break;

    case 3:
      return LongBoi.Color(random(0, 255),random(0, 255),random(0, 255));
      break;

    default:
      break;
  }

}

//This function will write a 4 byte (32bit) long to the eeprom at
//the specified address to adress + 3.
void EEPROMWritelong(int address, long value) {
  //Decomposition from a long to 4 bytes by using bitshift.
  //One = Most significant -> Four = Least significant byte
  byte four = (value & 0xFF);
  byte three = ((value >> 8) & 0xFF);
  byte two = ((value >> 16) & 0xFF);
  byte one = ((value >> 24) & 0xFF);

  //Write the 4 bytes into the eeprom memory.
  EEPROM.write(address, four);
  EEPROM.write(address + 1, three);
  EEPROM.write(address + 2, two);
  EEPROM.write(address + 3, one);
}

//This function will return a 4 byte (32bit) long from the eeprom
//at the specified address to adress + 3.
long EEPROMReadlong(long address) {
  //Read the 4 bytes from the eeprom memory.
  long four = EEPROM.read(address);
  long three = EEPROM.read(address + 1);
  long two = EEPROM.read(address + 2);
  long one = EEPROM.read(address + 3);

  //Return the recomposed long by using bitshift.
  return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
}
