# esp32_tft_sdfat_touch

default code for my board
-----------------------------

 Libraries required:
 --------------------
 Adafruit_GFX.h        // https://github.com/adafruit/Adafruit-GFX-Library
 Adafruit_ILI9341.h    // https://github.com/adafruit/Adafruit_ILI9341
 XPT2046_Touchscreen.h // https://github.com/PaulStoffregen/XPT2046_Touchscreen
 SPI.h
 Adafruit_ImageReader.h
 Adafruit_ImageReader_EPD.h

 SdFat.h               //  https://github.com/greiman/SdFat  
    Make edit to SdFat.h library file:
    "#define USE_STANDARD_SPI_LIBRARY 2" in library file SdFatConfig.h to support multiple SPI ports



