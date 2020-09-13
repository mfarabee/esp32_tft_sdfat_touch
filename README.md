# esp32_tft_sdfat_touch: Default code for my board

The program sets up the esp32 nodeMCU board to communicate with the 2.8" SPI TFT display as well as

enabling the onboard touch and SD card. In addition to getting the hardware all working,

this code also includes a module to create a keyboard as well as all of the hooks to make

the Adafruit_ImageReader work so that you can display bitmaps on the TFT display.

Parts required:
--------------
* 2.8" SPI TFT LCD Display Touch Panel 240X320
* ESP-WROOM-32 ESP32 ESP-32S Development Board (nodeMcu)
* My PCB  (optional)
* SD card (optional)

 Libraries required:
 --------------------
 * Adafruit_GFX.h        // https://github.com/adafruit/Adafruit-GFX-Library
 * Adafruit_ILI9341.h    // https://github.com/adafruit/Adafruit_ILI9341
 * XPT2046_Touchscreen.h // https://github.com/PaulStoffregen/XPT2046_Touchscreen
 * SPI.h
 * Adafruit_ImageReader.h
 * Adafruit_ImageReader_EPD.h
 * SdFat.h               //  https://github.com/greiman/SdFat  
    Make edit to SdFat.h library file:
    
     "#define USE_STANDARD_SPI_LIBRARY 2" in library file SdFatConfig.h to support multiple SPI ports
    

COMPILE:
-----------
 Arduino IDE: Use NodeMCU-32s board
 
 Edit: esp32_tft_sdfat_touch.h to enable/disable SD card or bitmap reader 
 
 Additioanl sample functions are available
 * calibrate touch  (in tftDisplay class)
 * show SD card info (in tftSD file)
 
 Execution of Sample program:
 -----------
 Copy bitmap and text files (bitmap directory) to SD card root directory
 Open Serial window at 115200 baud
 
 Program will do the following:
 * Display Beach picture, wait 5 seconds
 * Display sample Alpha keyboard, enter "test.txt" as the filename (result will display in Serial window)
 * Display sample Numeric keyboard (result will display in Serial window)
 * Enter a sample touch drawing loop with sand Dollar BMP as background. (every release will change color)


