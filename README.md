# AVR Temperature Display
----
## Description

> In This project we used native AVR Registers with the Arduino Uno and Arduino IDE to display the date, time and temperature on an OLED display.

## External Libraries

### Adafruit_GFX
by Adafruit to manage the displaying items on the OLED, So thanks to Adafruit. To install Adafruit_GFX [library](https://github.com/adafruit/Adafruit-GFX-Library).

### Adafruit_SSD1306
by Adafruit to use the OLED in arduino IDE, thanks again to Adafruit. To install Adafruit_SSD1306 [library](https://github.com/adafruit/Adafruit_SSD1306).

### RTClib
by Adafruit, it is A fork of Jeelab's RTC library, to manage the RTC date and time, Adafruit many thanks. To install RTC [library](https://github.com/adafruit/RTClib).

## How It Works

The Arduino Uno reads the temperature through the LM35 sensor. Read the date, and time from the RTC in setup() only, then we run a timer with a "one second" tick. updating the time every second using that tick. and in the loop the readings (clock and temperature) are displayed on the OLED.
We also have added our logo on the Startup To be shown on the oled for 2 seconds after showing the Adafruit company logo.
   
## Quick Start

To run the code you need to:

 1. Install the Adafruit_GFX library from [github](https://github.com/adafruit/Adafruit-GFX-Library). or from the tools menu in the Arduino IDE. choose “Manage libraries”, then type Adafruit_GFX in the search bar and install the one from Adafruit.
 2. Install the Adafruit_SSD1306 library from [github](https://github.com/adafruit/Adafruit_SSD1306), or from the library Manager, type Adafruit_SSD1306 in the search bar and install it.
 3.	Install the RTClib library from [github](https://github.com/adafruit/RTClib), Or in Manage libraries type RTClib in the search bar and install the one from Adafruit.

For more details please check our [blog post](https://www.the-diy-life.co/2019/09/03/temperature-display/)

----
## Thanks
