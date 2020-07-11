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

#include "tftDisplay.h"
#include "tftSD.h"
#include "keyboard.h"

// Enable of SD card, enable=1, disable=0
#define SD_ENABLE 1
// Enable Adafruit Image Reader (displays BMP files from SD card enable=1, disable=0
#define USE_IMAGEREADER 1

// Image Reader will read 24bit BMP images from the SD card and display on the TFT
// This requires SdFat library as well as editing the default SdFatConfig.h file
// #define ENABLE_EXTENDED_TRANSFER_CLASS 1
// #define USE_STANDARD_SPI_LIBRARY 2
// #define FAT12_SUPPORT 1

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
  int x, y,rotation,color;
  static unsigned long myTimer=0;
  int myColor[]={ILI9341_WHITE,ILI9341_BLUE,ILI9341_GREEN,ILI9341_RED};
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
    if((millis()-timer)>200){//  if not touching for more than 200ms
      color=(color+1)%4;
    }
    Tft->fillCircle(x, y, 2, color);
    timer=millis();
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
  delay(10000);
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
  ImageReaderStat= reader->drawBMP("/us_24b.bmp",*Tft,0,0);
#endif
}

// add to tftdisplay class
void swipeDetect(int *deltaX,int *deltaY){
  static unsigned long timer=0;
  static int prevX=0,prevY=0;
  int x,y;
  if(Tft->isTouched(&x,&y)){
    if((millis()-timer)<=500){
      *deltaX=x-prevX;
      *deltaY=y-prevY;
    }
    if(deltaX>tolerance || deltaY>tolerance){
      prevX=x;
      prevY=y;
    }else{
      *deltaX=0;
      *deltaY=0;
    }
    timer=millis();
  }
}

#define SCROLL_WINDOW_T 0
#define SCROLL_WINDOW_L 0
#define SCROLL_WINDOW_H 320
#define SCROLL_WINDOW_W 240
#define SCROLL_WINDOW_ROWS 13
#define SCROLL_WINDOW_TSCALE 3
#define SCROLL_WINDOW_ROW_H 24
#define SCROLL_WINDOW_BCOLOR ILI9341_BLACK
void scrollDown(int line,String text){
  int y;
  //rows=int(Tft->height()/charHeight);
  y=(line%SCROLL_WINDOW_ROWS)*SCROLL_WINDOW_ROW_H;
  Tft->fillRect(SCROLL_WINDOW_T,SCROLL_WINDOW_ROW_H*(SCROLL_WINDOW_ROWS-1), SCROLL_WINDOW_W, SCROLL_WINDOW_H,ILI9341_BLACK);
  Tft-scrollTo(y);
  Tft->setTextSize(SCROLL_WINDOW_TSCALE);   
  Tft->setCursor(0,0);
  Tft->print(text);
}
void scrollUp(int line,String text){
  int y;
  //rows=int(Tft->height()/charHeight);
  y=(line%SCROLL_WINDOW_ROWS)*(SCROLL_WINDOW_TSCALE*8);
  Tft->fillRect(SCROLL_WINDOW_L,SCROLL_WINDOW_T, SCROLL_WINDOW_W, SCROLL_WINDOW_ROW_H,ILI9341_BLACK);
  Tft-scrollTo(y);
  Tft->setTextSize(SCROLL_WINDOW_TSCALE);   
  Tft->setCursor(SCROLL_WINDOW_L,SCROLL_WINDOW_ROW_H*(SCROLL_WINDOW_ROWS-1));
  Tft->print(text);
}
void scrollTest(){
  int swipeX,swipeY;
  setAddrWindow(0,0,Tft->width(),Tft->height());
  // populate doc array
  // load screen
  while(1){
    swipeDetect(&swipeX,&swipeY);
    if(swipeY<0){
      if(line <lineCount){
        ++line;
        scrollUp(line,3,doc[line]);
      }
    }else if(swipeY>0){
      if(line >0{
        --line;
        scrollDown(line,3,doc[line]);
      }
    }
    delay(10);
  }
}


void loop() {
  // Sample drawing program
  drawIt();
}
