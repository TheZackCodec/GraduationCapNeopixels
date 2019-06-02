#include <Adafruit_NeoPixel.h>

// Pattern types supported:
enum pattern { NONE, RAINBOW_CYCLE, THEATER_CHASE, COLOR_WIPE, SCANNER, FADE, SINGLERANDOM, ALLRANDOM };
// Patern directions supported:
enum direction { FORWARD, REVERSE };

// NeoPattern Class - derived from the Adafruit_NeoPixel class
class NeoPatterns : public Adafruit_NeoPixel {
  public:

  // Member Variables:
  pattern  ActivePattern;  // which pattern is running
  direction Direction;     // direction to run the pattern

  unsigned long Interval;   // milliseconds between updates
  unsigned long lastUpdate; // last update of position

  uint32_t Color1, Color2;  // What colors are in use
  uint16_t TotalSteps;  // total number of steps in the pattern
  uint16_t Index;  // current step within the pattern

  bool randomSwitch;

  void (*OnComplete)();  // Callback on completion of pattern



  // Constructor - calls base-class constructor to initialize strip
  NeoPatterns(uint16_t pixels, uint8_t pin, uint8_t type, void (*callback)())
  :Adafruit_NeoPixel(pixels, pin, type) {
    OnComplete = callback;
  }


  // --- Utility Functions ---

  // Update the pattern
  void Update(){
    if((millis() - lastUpdate) > Interval) { // time to update
      lastUpdate = millis();
      switch(ActivePattern) {

        case RAINBOW_CYCLE:
          RainbowCycleUpdate();
          break;

        case THEATER_CHASE:
          TheaterChaseUpdate();
          break;

        case COLOR_WIPE:
          ColorWipeUpdate();
          break;

        case SCANNER:
          ScannerUpdate();
          break;

        case FADE:
          FadeUpdate();
          break;

        case SINGLERANDOM:
          SingleRandomUpdate();
          break;

        case ALLRANDOM:
          AllRandomUpdate();
          break;

        default:
          break;

      }
    }
  }

  // Increment the Index and reset at the end
  void Increment(){
    if (Direction == FORWARD) {
      Index++;
      if (Index >= TotalSteps) {
        Index = 0;
        if (OnComplete != NULL) {
          OnComplete(); // call the comlpetion callback
        }
      }
    }
    else { // Direction == REVERSE
      --Index;
      if (Index <= 0) {
        Index = TotalSteps-1;
        if (OnComplete != NULL) {
          OnComplete(); // call the comlpetion callback
        }
      }
    }
  }

  // Returns the Red component of a 32-bit color
  uint8_t Red(uint32_t color) {
    return (color >> 16) & 0xFF;
  }

  // Returns the Green component of a 32-bit color
  uint8_t Green(uint32_t color) {
    return (color >> 8) & 0xFF;
  }

  // Returns the Blue component of a 32-bit color
  uint8_t Blue(uint32_t color) {
    return color & 0xFF;
  }

  // Return color, dimmed by 75% (used by scanner)
  uint32_t DimColor(uint32_t color) {
    uint32_t dimColor = Color(Red(color) >> 1, Green(color) >> 1, Blue(color) >> 1);
    return dimColor;
  }

  // Input a value 0 to 255 to get a color value.
  // The colours are a transition r - g - b - back to r.
  uint32_t Wheel(byte WheelPos) {
    WheelPos = 255 - WheelPos;
    if(WheelPos < 85) {
      return Color(255 - WheelPos * 3, 0, WheelPos * 3);
    }
    else if(WheelPos < 170) {
      WheelPos -= 85;
      return Color(0, WheelPos * 3, 255 - WheelPos * 3);
    }
    else {
      WheelPos -= 170;
      return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
      }
  }

  // Reverse direction of the pattern
  void Reverse() {
    if (Direction == FORWARD) {
      Direction = REVERSE;
      Index = TotalSteps-1;
    }
    else {
      Direction = FORWARD;
      Index = 0;
    }
  }

  // Set all pixels to a color (synchronously)
  void ColorSet(uint32_t color) {
    for (int i = 0; i < numPixels(); i++) {
      setPixelColor(i, color);
    }
    show();
  }


  //------ PATTERNS ------



  // --- Rainbow Cycle ---

  // Initialize for a RainbowCycle
  void RainbowCycle(uint8_t interval, direction dir = FORWARD) {
    Serial.println("");
    Serial.println("--- Rainbow Cycle ---");
    Serial.print("Interval: ");
    Serial.println(interval);
    Serial.print("Direction: ");
    if(dir == FORWARD) { Serial.println("Forward"); } else { Serial.println("Reverse");}


    ActivePattern = RAINBOW_CYCLE;
    Interval = interval;
    TotalSteps = 255;
    Index = 0;
    Direction = dir;
  }

  // Update the Rainbow Cycle Pattern
  void RainbowCycleUpdate() {
    for(int i=0; i< numPixels(); i++) {
      setPixelColor(i, Wheel(((i * 256 / numPixels()) + Index) & 255));
    }
    show();
    Increment();
  }


  // --- Color Wipe ---

  // Initialize for a ColorWipe
  void ColorWipe(uint32_t color, uint8_t interval, direction dir = FORWARD) {
    Serial.println("");
    Serial.println("--- Color Wipe ---");
    Serial.print("Interval: ");
    Serial.println(interval);
    Serial.print("Color: ");
    Serial.println(color);
    Serial.print("Direction: ");
    if(dir == FORWARD) { Serial.println("Forward"); } else { Serial.println("Reverse");}

    ActivePattern = COLOR_WIPE;
    Interval = interval;
    TotalSteps = numPixels();
    Color1 = color;
    Index = 0;
    Direction = dir;
  }

  // Update the Color Wipe Pattern
  void ColorWipeUpdate() {
    setPixelColor(Index, Color1);
    show();
    Increment();
  }



  // --- Theater Chase ---

  // Initialize for a Theater Chase
  void TheaterChase(uint32_t color1, uint32_t color2, uint8_t interval, direction dir = FORWARD) {
    Serial.println("");
    Serial.println("--- Theater Chase ---");
    Serial.print("Interval: ");
    Serial.println(interval);
    Serial.print("Color 1: ");
    Serial.println(color1);
    Serial.print("Color 2: ");
    Serial.println(color2);
    Serial.print("Direction: ");
    if(dir == FORWARD) { Serial.println("Forward"); } else { Serial.println("Reverse");}

    ActivePattern = THEATER_CHASE;
    Interval = interval;
    TotalSteps = numPixels();
    Color1 = color1;
    Color2 = color2;
    Index = 0;
    Direction = dir;
  }

  // Update the Theater Chase Pattern
  void TheaterChaseUpdate() {
    for(int i=0; i< numPixels(); i++) {
      if ((i + Index) % 3 == 0) {
        setPixelColor(i, Color1);
      }
      else {
        setPixelColor(i, Color2);
      }
    }
    show();
    Increment();
  }

  // --- Single Random ---

  void SingleRandom(uint32_t color1, uint8_t interval, uint16_t genCount, bool rand, direction dir = FORWARD) {
    // Create random seed by reading the analog noise on pin 0
    Serial.println("");
    Serial.println("--- Single Random ---");
    Serial.print("Random: ");
    if (rand) { Serial.println("True"); } else { Serial.println("False"); }
    Serial.print("Interval: ");
    Serial.println(interval);
    Serial.print("Num Pixels: ");
    Serial.println(genCount);

    if (rand) {
      randomSwitch = true;
    } else {
      randomSwitch = false;
      Color1 = color1;
      Serial.print("Color: ");
      Serial.println(color1);
    }

    ActivePattern = SINGLERANDOM;
    Interval = interval;
    TotalSteps = genCount;
    Color1 = color1;
    Index = 0;
    Direction = dir;
  }

  void SingleRandomUpdate(){
    // zero out outer_strip
    for(int i=0; i < numPixels(); i++) {
      setPixelColor(i, 0, 0, 0);
    }

    // display all zeros
    show();

    // Select random pixel
    int pixel = random(0, numPixels());

    // Random Color
    if (randomSwitch){
      Color1 = Color(random(0, 255), random(0, 255), random(0, 255));
    }

    setPixelColor(pixel, Color1);
    show();
    Increment();
  }

  // --- All Random ---

  void AllRandom(uint8_t interval, uint16_t steps, direction dir = FORWARD) {
    // Create random seed by reading the analog noise on pin 0
    Serial.println("");
    Serial.println("--- All Random ---");
    Serial.print("Interval: ");
    Serial.println(interval);
    Serial.print("Steps: ");
    Serial.println(steps);

    ActivePattern = ALLRANDOM;
    Interval = interval;
    TotalSteps = steps;
    Index = 0;
    Direction = dir;
  }

  void AllRandomUpdate(){

    // Random Color
    for(int pixel = 0; pixel < numPixels(); pixel++) {
      setPixelColor(pixel, Color(random(0, 255), random(0, 255), random(0, 255)));
    }

    show();
    Increment();
  }

  // --- Scanner ---

  // Initialize for a SCANNNER
  void Scanner(uint32_t color1, uint8_t interval, direction dir = FORWARD) {
    Serial.println("");
    Serial.println("--- Scanner ---");
    Serial.print("Interval: ");
    Serial.println(interval);
    Serial.print("Color: ");
    Serial.println(color1);
    Serial.print("Direction: ");
    if(dir == FORWARD) { Serial.println("Forward"); } else { Serial.println("Reverse");}

    ActivePattern = SCANNER;
    Interval = interval;
    TotalSteps = (numPixels() - 1) * 2;
    Color1 = color1;
    Index = 0;
    Direction = dir;
  }

  // Update the Scanner Pattern
  void ScannerUpdate() {
    for (int i = 0; i < numPixels(); i++) {
      if (i == Index) { // first half of the scan
        setPixelColor(i, Color1);
      }
      else if (i == TotalSteps - Index) { // The return trip.\
        setPixelColor(i, Color1);
      }
      else {  // fade to black
        setPixelColor(i, DimColor(getPixelColor(i)));
      }
    }
    show();
    Increment();
  }



  // --- Fade ---

  // Initialize for a Fade
  void Fade(uint32_t color1, uint32_t color2, uint16_t steps, uint8_t interval, direction dir = FORWARD) {
    Serial.println("");
    Serial.println("--- Fade ---");
    Serial.print("Interval: ");
    Serial.println(interval);
    Serial.print("Steps: ");
    Serial.println(steps);
    Serial.print("Color 1: ");
    Serial.println(color1);
    Serial.print("Color 2: ");
    Serial.println(color2);

    ActivePattern = FADE;
    Interval = interval;
    TotalSteps = steps;
    Color1 = color1;
    Color2 = color2;
    Index = 0;
    Direction = dir;
  }

  // Update the Fade Pattern
  void FadeUpdate() {
    uint8_t red = ((Red(Color1) * (TotalSteps - Index)) + (Red(Color2) * Index)) / TotalSteps;
    uint8_t green = ((Green(Color1) * (TotalSteps - Index)) + (Green(Color2) * Index)) / TotalSteps;
    uint8_t blue = ((Blue(Color1) * (TotalSteps - Index)) + (Blue(Color2) * Index)) / TotalSteps;
    ColorSet(Color(red, green, blue));
    show();
    Increment();
  }



};
