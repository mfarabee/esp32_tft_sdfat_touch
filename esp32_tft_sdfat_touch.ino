// For NodeMCU-32s and 240x320 TFT touch display with SD card
/*
Connections:
DISPLAY    ESP32        NOTES
T_IRQ       NC          Not using IRQ
T_DO        NC
T_DIN       GPIO23
T_CS        GPIO4
T_CLK       GPIO18
SDO(MISO)   GPIO19
LED         3.3V
SCK         GPIO18
SDI(MOSI)   GPIO23
DC          GPIO08
RESET       GPIO27
CS          GPIO05
GND         GND
VCC         +5V (VIN)   The display has a voltage regulator requiring +5V input

SD_CS       GPIO15
SD_MOSI     GPIO13
SD_MISO     GPIO26      No Tristate driver on Display card for SPI interface
SD_SCK      GPIO14
*/

#include "esp32_tft_sdfat_touch.h"
#include "tftDisplay.h"
#include "tftSD.h"
#include "keyboard.h"

// IMAGE READER
// The Adafruit image reader will only read 24bit color bitmaps
// Load a picture into "Microsoft Paint"
//    Resize it to 240x320 pixels (or close to it)
//    Save-As -> 24-bit Bitmap

// https://learn.adafruit.com/adafruit-gfx-graphics-library/loading-images
// If not using Image Reader, don't include headers to save memory
#if USE_IMAGEREADER == 1  
  #define SD_ENABLE 1
  #include <Adafruit_ImageReader.h>
  #include <Adafruit_ImageReader_EPD.h>

  Adafruit_ImageReader *reader;
  ImageReturnCode ImageReaderStat;
#endif

TFT_CLASS *Tft;

// Example of how to create keyboards for data entry
// Alphanumeric Keyboard (27 keys) recommendations for rotation=[1,2]=row=3 col=9 | for rotation=[0,3]=row=7 col=4
// Numeric Keyboard (12 keys) recommendations for rotation=[1,2] =row=3 col=4 | for rotation=[0,3]=row=4 col=3
char KbdLower[]  = "abcdefghijklmnopqrstuvwxyz ";
char KbdUpper[]  = "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";
char KbdSymbol[]= "1234567890!@#$%^&*-_:;,./+?";
char KbdNum[]    = "1234567890.-";
char KbdEmpty[]  = "";
KEYBOARD_CLASS *kbd, *kbd2;


// Sample drawing routine
void drawIt() {
  int x, y,rotation;
  static int color=0;
  static unsigned long myTimer=0;
  const int myColor[]={ILI9341_WHITE,ILI9341_BLUE,ILI9341_GREEN,ILI9341_RED};
  char inData;
  if (Serial.available() > 0) {
    inData = Serial.read();
    switch (inData) {
      case 'c':
        Tft->fillScreen(ILI9341_BLACK);
        break;
      case 'r':
        rotation = (Tft->getRotation()+1) % 4;
        Serial.print("ROT:"); Serial.println(rotation);
        Tft->setRotation(rotation);
        break;
    }
  }
  if (Tft->isTouched(&x, &y)) {
    // Change color if, stopped drawing and started again
    if((millis()-myTimer)>200){//  if not touching for more than 200ms
      color=(color+1)%4;
    }
    Tft->fillCircle(x, y, 2, myColor[color]);
    myTimer=millis();
  }
}


void setup() {
  String result="";
  
  Serial.begin(115200);
  
  Tft= new TFT_CLASS(SD_ENABLE,1); // Set  SD card=off/on  and initial rotation to 1
  // Tft->calibrate(); // un-comment to calibrate touch

  // Examples of SD usage, un-comment to list SD card files to terminal (Set SD_USAGE = 1)
  // SdVolume();  
  // SdFileExists("/beach24.bmp",2);
  // SdList("");
  // SdListTft("");
  // delay(2000);

#if USE_IMAGEREADER >0
  // initialize image reader
  reader = new Adafruit_ImageReader(*Tft->SdCard);
  
  ImageReaderStat= reader->drawBMP("/beach24.bmp",*Tft,0,0);
  reader->printStatus(ImageReaderStat);
  delay(5000);
#endif

  // Example of how to create keyboards
  Tft->fillScreen(ILI9341_BLACK);
  kbd = new KEYBOARD_CLASS(Tft,3,9,ILI9341_WHITE,ILI9341_BLUE,KbdLower,KbdUpper,KbdSymbol);
  kbd2 = new KEYBOARD_CLASS(Tft,3,4,ILI9341_WHITE,ILI9341_BLUE,KbdNum,KbdEmpty,KbdEmpty);
  result = kbd->activateKeyboard("Filename?");
  Serial.print("result= ");Serial.println(result);
  SdReadFile(result);
  result=kbd2->activateKeyboard("Enter Zip Code");
  Serial.print("result= ");Serial.println(result);

#if USE_IMAGEREADER >0
  ImageReaderStat= reader->drawBMP("/sandDollar.bmp",*Tft,0,0);
#endif
}



void loop() {

  // Sample drawing program
  drawIt();
}
