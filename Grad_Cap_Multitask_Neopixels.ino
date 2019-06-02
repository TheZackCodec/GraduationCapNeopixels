#include "NeoPatterns.h"

void LongBoiComplete();

// Define some NeoPatterns for the two rings and the stick
//  and pass the adderess of the associated completion routines
NeoPatterns LongBoi(60, 2, NEO_GRB + NEO_KHZ800, &LongBoiComplete);

// Initialize everything and prepare to start
void setup() {

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

  switch(random(0, 7)) {

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
      LongBoi.Fade(randColor(), randColor(), 200, random(5, 15));
      if (randBool()){
        LongBoi.Reverse();
      }
      break;

    case 3:
      LongBoi.TheaterChase(randColor(), LongBoi.Color(0,0,0), random(50, 100));
      if (randBool()){
        LongBoi.Reverse();
      }
      break;

    case 4:
      LongBoi.Scanner(randColor(), random(10, 50));
      if (randBool()){
        LongBoi.Reverse();
      }
      break;

    case 5:
      LongBoi.ColorWipe(randColor(), random(10, 50));
      if (randBool()){
        LongBoi.Reverse();
      }
      break;

    case 6:
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
