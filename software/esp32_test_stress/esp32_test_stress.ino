//C.Moore for CCHS
//Demo reel and stress test for the overhead RGB lighting system
//Dev board:
//  WEMOS D1 R32 (ESP32)
//This is not perfect, but it's good enough!
//Supply voltage/current can be monitored with TrashPower


#include <FastLED.h>

FASTLED_USING_NAMESPACE

#define DATA_PIN2    26
#define DATA_PIN3    25
#define DATA_PIN4    17
#define DATA_PIN5    16
#define DATA_PIN6    27
#define DATA_PIN7    14

#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS    170
#define NUM_LINES    6
CRGB leds[NUM_LINES][NUM_LEDS];

#define BRIGHTNESS         255  //100% brightness (led colour of white fades along legnth of strip)
//#define BRIGHTNESS         191  //75% brightness (led colour still fades to red a little)
//#define BRIGHTNESS         127  //50% brightness, results in best uniformity of white when all illuminated

#define FRAMES_PER_SECOND  25   //Must be a whole integer factor of 1000 (ms), eg: 100/50/25/20

void setup() {
  delay(2000);
  
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN2,COLOR_ORDER>(leds[0], NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE,DATA_PIN3,COLOR_ORDER>(leds[1], NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE,DATA_PIN4,COLOR_ORDER>(leds[2], NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE,DATA_PIN5,COLOR_ORDER>(leds[3], NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE,DATA_PIN6,COLOR_ORDER>(leds[4], NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE,DATA_PIN7,COLOR_ORDER>(leds[5], NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);

  randomSeed(50);
}


// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { line_seq_w, line_seq_r, line_seq_g, line_seq_b, stack_white_fade_check, flash_black_white, line_seq_rainbow, colour_pinwheel, bivar_trig };

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
  
long unsigned millis_prev = 0;
long unsigned millis_curr = 0;

void loop()
{
  bool update_display = false;

  int update_period_ms = 1000/FRAMES_PER_SECOND;
  //Detect millis() rollover
  millis_prev = millis_curr;
  millis_curr = millis();
  if(millis_curr != millis_prev){
    //millis rollover, check if we need to update on this event:
    if(millis_curr % update_period_ms == 0){
      //Set flag
      update_display = true;
    }
  }
  
  if(update_display){
    // Call the current pattern function once, updating the 'leds' array
    gPatterns[gCurrentPatternNumber]();

    // send the 'leds' array out to the actual LED strip
    FastLED.show(); 
  }

  // do some periodic updates
  EVERY_N_MILLISECONDS( 5 ) { gHue++; } // slowly cycle the "base color" through the rainbow
  EVERY_N_SECONDS( 6 ) { nextPattern(); } // change patterns periodically
}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

bool first_run = true;
void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
  fill_solid(leds[0], NUM_LEDS*NUM_LINES, 0);
  first_run = true;
}

//Validates:
// correct order of the lines
// full functionality of each LED in RGB
// voltage drop along length of strip resulting in colour drift (orange)
void line_seq_w() 
{
  static int line;
  EVERY_N_SECONDS( 1 ){ ++line; }
  if(line >= NUM_LINES){ line = 0; }
  if(first_run){
    line = 0;
    first_run = false;
  }
  // FastLED's built-in rainbow generator
  if(line < NUM_LINES ){
    fill_solid(leds[line], NUM_LEDS, CRGB::White);
  }
}

//Switch on each line as red in sequence: 1,2,3,4,5,6
void line_seq_r() 
{
  static int line;
  EVERY_N_SECONDS( 1 ){ ++line; }
  if(line >= NUM_LINES){ line = 0; }
  if(first_run){
    line = 0;
    first_run = false;
  }
  // FastLED's built-in rainbow generator
  if(line < NUM_LINES ){
    fill_solid(leds[line], NUM_LEDS, CRGB( 255, 0, 0)); 
  }
}


//Switch on each line as green in sequence: 1,2,3,4,5,6
void line_seq_g() 
{
  static int line;
  EVERY_N_SECONDS( 1 ){ ++line; }
  if(line >= NUM_LINES){ line = 0; }
  if(first_run){
    line = 0;
    first_run = false;
  }
  // FastLED's built-in rainbow generator
  if(line < NUM_LINES ){
    fill_solid(leds[line], NUM_LEDS, CRGB( 0, 255, 0));
  }
}

//Switch on each line as blue in sequence: 1,2,3,4,5,6
void line_seq_b() 
{
  static int line;
  EVERY_N_SECONDS( 1 ){ ++line; }
  if(line >= NUM_LINES){ line = 0; }
  if(first_run){
    line = 0;
    first_run = false;
  }
  // FastLED's built-in rainbow generator
  if(line < NUM_LINES ){
    fill_solid(leds[line], NUM_LEDS, CRGB( 0, 0, 255));
  }
}

//Fill from the end with white at maximum brightness to observe colour fade in strip due to voltage drop
void stack_white_fade_check()
{
  static int row_fill_start;
  if(first_run){
    row_fill_start = 0;
    first_run = false;
  }

  int advance_rows = 2; //On each iteration, advance this many rows  
  int row_fill_stop = row_fill_start + advance_rows;

  for(int row = row_fill_start; row < row_fill_stop; ++row){
    for(int line = 0; line < NUM_LINES; ++line){

      //array bound safety checks
      if(row >= 0 && row < NUM_LEDS){

        //Reverse direction:
        int row_reverse = NUM_LEDS-row;
        leds[line][row_reverse] = CRGB( 255, 255, 255);
      }

    }
  }
  row_fill_start = row_fill_stop;

}

//Validates that pattern is being sent
void line_seq_rainbow() 
{
  static int line;
  EVERY_N_SECONDS( 1 ){ ++line; }
  if(line >= NUM_LINES){ line = 0; }
  if(first_run){
    line = 0;
    first_run = false;
  }
  // FastLED's built-in rainbow generator
  if(line < NUM_LINES ){
    fill_rainbow( leds[line], NUM_LEDS, gHue, 7);
  }
}

//This function is useful to identify problems in communication of LED config due to high current transients
void flash_black_white(){
  static bool state;
  state = !state;
  if(state){
    fill_solid(leds[0], NUM_LEDS*NUM_LINES, CRGB::White);
  }
  else{
    fill_solid(leds[0], NUM_LEDS*NUM_LINES, CRGB::Black);
  }
}

//This function is nonsense really, it just demonstrates how to use trig functions and how to include the time variable
void bivar_trig(){
  for(int x = 0; x < NUM_LINES; ++x){
    for(int y = 0; y < NUM_LEDS; ++y){
      //leds[x][y].r = (1+sin(M_2_PI*(millis_curr % 1000)/1000.0)*round(float(x)/float(NUM_LINES-1)))*127; 
      //leds[x][y].g = (1+cos(M_2_PI*(millis_curr % 1000)/1000.0)*round(float(y)/float(NUM_LEDS -1)))*127; 
      //leds[x][y].b = 0;

      int periods = 6;
      leds[x][y].r = round((1+sin(2.0*M_PI*(float(x)/float(NUM_LINES)) + (millis_curr / 2000.0)))*64.0);
      leds[x][y].g = round((1+sin(2.0*M_PI*(float(y)/float(NUM_LEDS)*periods) + float(x)*(millis_curr / 500.0)))*127.0);
      leds[x][y].b = round((1+cos(2.0*M_PI*(float(y)/float(NUM_LEDS)*5) - float(x)*(millis_curr / 500.0)))*127.0);
    }
  }

}

//This makes the centre of the space the centre of a wheel of colour in which the hue shifts around the circle
void colour_pinwheel(){

  float x_mid = float(NUM_LINES)/2.0;
  float y_mid = float(NUM_LEDS)/2.0;

  for(int x = 0; x < NUM_LINES; ++x){
    for(int y = 0; y < NUM_LEDS; ++y){ 

      //Cartesian to polar conversion with normalisation
      float a = (float(x)+0.5-x_mid)/x_mid;
      float b = (float(y)+0.5-y_mid)/y_mid;
      float mag = sqrt(a*a+b*b);
      float arg = atan(b/a);

      //Limit extent of circle to less than or equal unity magnitude
      if (mag > 1.0){
        mag = 0.0;
      }
      float brightness = round(mag*255.0);  //Scale up for fastled
      uint8_t hue = round(((M_PI/2.0 + arg)/M_PI)*255);
      hue += gHue; //For time-based rotation, uint8_t truncates most significant bits of operation

      leds[x][y] = CHSV( hue, 255, brightness );
    }
  }
}


