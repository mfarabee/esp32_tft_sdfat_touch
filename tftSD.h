#ifndef TFTSD_H
#define TFTSD_H
#include <Arduino.h>
#include <SdFat.h>  //  https://github.com/greiman/SdFat    or  https://github.com/adafruit/SdFat
// Image Reader will read 24bit BMP images from the SD card and display on the TFT
// This requires SdFat library as well as editing the default SdFatConfig.h file
// #define USE_STANDARD_SPI_LIBRARY 2           <- This is required to use alternate SPI
// #define FAT12_SUPPORT 1                       <- Not sure if this is required
// #define ENABLE_EXTENDED_TRANSFER_CLASS 1      <- Not sure if this is required


void SdVolume();
void SdList(String dir);
int SdFileExists(String fName, int displayType=0);
void SdReadFile(String fName);
void SdListTft(String dir);
void SdListTft(File dir,int spaces);

#endif
