/**************************************************************************
  This is an example for our Monochrome OLEDs based on SSD1306 drivers

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/category/63_98

  This example is for a 128x64 pixel display using SPI to communicate
  4 or 5 pins are required to interface.

  Adafruit invests time and resources providing this open
  source code, please support Adafruit and open-source
  hardware by purchasing products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries,
  with contributions from the open source community.
  BSD license, check license.txt for more information
  All text above, and the splash screen below must be
  included in any redistribution.
 **************************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
#include <avr/io.h>  //standard AVR header
#include <stdio.h>

#include "RTClib.h"

#include <avr/interrupt.h>

RTC_DS1307 rtc;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

/// using hardware SPI:
#define OLED_DC    9
#define OLED_CS    12
#define OLED_RESET 10

/// using software SPI (default case):
//Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
//                         OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

/// using hardware SPI:
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &SPI,
                         OLED_DC, OLED_RESET, OLED_CS);

//#define NUMFLAKES     10 // Number of snowflakes in the animation example

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16

// enum for application state
enum state {
  BEGIN_STATE,
  INITIAL_STATE,
  WAIT_STATE,
  LOOP_STATE
};

// the DIY life logo in hex.
static const unsigned char PROGMEM DIYlogo_bmp[] =
{
  0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
  0x0, 0x0, 0x0, 0x0, 0x7, 0x80, 0x0, 0x0, 0x0, 0x0,
  0x0, 0x0, 0x0, 0x3, 0xff, 0xff, 0x80, 0x0, 0x0, 0x0,
  0x0, 0x0, 0x0, 0x3f, 0xff, 0xff, 0xf8, 0x0, 0x0, 0x0,
  0x0, 0x0, 0x1, 0xff, 0xff, 0xff, 0xff, 0x0, 0x0, 0x0,
  0x0, 0x0, 0x7, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x0, 0x0,
  0x0, 0x0, 0x1f, 0xfc, 0x0, 0x0, 0xff, 0xf0, 0x0, 0x0,
  0x0, 0x0, 0x7f, 0xc0, 0x0, 0x0, 0xf, 0xfc, 0x0, 0x0,
  0x0, 0x1, 0xff, 0x0, 0x0, 0x0, 0x1, 0xfe, 0x0, 0x0,
  0x0, 0x3, 0xfc, 0x0, 0x0, 0x0, 0x0, 0x7f, 0x80, 0x0,
  0x0, 0x7, 0xf0, 0x0, 0x0, 0x0, 0x0, 0x3f, 0xc0, 0x0,
  0x0, 0x1f, 0xc0, 0x0, 0x80, 0x0, 0x0, 0xf, 0xe0, 0x0,
  0x0, 0x3f, 0x80, 0xf, 0x80, 0x0, 0x0, 0x7, 0xf0, 0x0,
  0x0, 0x3f, 0x0, 0x3f, 0x80, 0x0, 0x0, 0x1, 0xf8, 0x0,
  0x0, 0x7e, 0x0, 0xff, 0x80, 0x0, 0x0, 0x0, 0xfc, 0x0,
  0x0, 0xfc, 0x3, 0xff, 0x80, 0x0, 0x0, 0x0, 0x7e, 0x0,
  0x1, 0xf8, 0x7, 0xff, 0x80, 0x0, 0x0, 0x0, 0x3f, 0x0,
  0x3, 0xf0, 0x1f, 0xe3, 0x80, 0x0, 0x0, 0x0, 0x3f, 0x0,
  0x3, 0xe0, 0x3f, 0x83, 0x80, 0x0, 0x0, 0x0, 0x1f, 0x80,
  0x7, 0xe0, 0x7e, 0x3, 0x80, 0x0, 0x0, 0x0, 0xf, 0xc0,
  0x7, 0xc0, 0xf8, 0x3, 0x80, 0x0, 0x0, 0x0, 0xf, 0xc0,
  0xf, 0x80, 0xf0, 0x3, 0x80, 0x0, 0x0, 0x0, 0x7, 0xe0,
  0xf, 0x81, 0xe0, 0x3, 0x80, 0x0, 0x20, 0x4, 0x7, 0xe0,
  0x1f, 0x1, 0xc0, 0x3, 0x80, 0x0, 0x38, 0x1e, 0x3, 0xe0,
  0x1f, 0x3, 0xc0, 0x3, 0x83, 0xfc, 0x3e, 0x7e, 0x3, 0xf0,
  0x1f, 0x3, 0x80, 0x3, 0x87, 0xfc, 0x3f, 0xff, 0x3, 0xf0,
  0x1f, 0x3, 0x80, 0x3, 0x83, 0xfc, 0x3f, 0xff, 0x1, 0xf0,
  0x3e, 0x7, 0x0, 0x1, 0x83, 0xfc, 0x1f, 0xef, 0x1, 0xf0,
  0x3e, 0x7, 0x0, 0x1, 0x83, 0xfc, 0x7, 0xcf, 0x81, 0xf0,
  0x3e, 0x7, 0x0, 0x1, 0x83, 0xfc, 0x0, 0xf, 0x81, 0xf0,
  0x3e, 0x7, 0x0, 0x1, 0x83, 0xfc, 0x0, 0x7, 0x81, 0xf8,
  0x3e, 0x7, 0x0, 0x1, 0x83, 0xfc, 0x0, 0x7, 0x81, 0xf8,
  0x3e, 0x7, 0x0, 0x1, 0x83, 0xfc, 0x0, 0x7, 0x81, 0xf0,
  0x3e, 0x7, 0x0, 0x1, 0x83, 0xfc, 0x0, 0x7, 0x81, 0xf0,
  0x3e, 0x7, 0x0, 0x1, 0x83, 0xfc, 0x0, 0xf, 0x81, 0xf0,
  0x1e, 0x7, 0x0, 0x1, 0x83, 0xfc, 0x0, 0xf, 0x81, 0xf0,
  0x1f, 0x3, 0x80, 0x1, 0x83, 0xfc, 0x0, 0xf, 0x83, 0xf0,
  0x1f, 0x3, 0x80, 0x1, 0x83, 0xfc, 0x0, 0xf, 0x83, 0xf0,
  0x1f, 0x3, 0xc0, 0x1, 0x83, 0xfc, 0x0, 0x1f, 0x83, 0xe0,
  0xf, 0x81, 0xe0, 0x1, 0x83, 0xfc, 0x0, 0x3f, 0x3, 0xe0,
  0xf, 0x80, 0xf0, 0x1, 0x83, 0xfe, 0x0, 0x3f, 0x7, 0xe0,
  0x7, 0xc0, 0xf8, 0x1, 0x83, 0xfe, 0x0, 0x7f, 0x7, 0xc0,
  0x7, 0xc0, 0x7c, 0x1, 0x83, 0xfe, 0x1, 0xfe, 0xf, 0xc0,
  0x3, 0xe0, 0x3f, 0x1, 0x83, 0xfe, 0xf, 0xfc, 0x1f, 0x80,
  0x3, 0xf0, 0x1f, 0xc1, 0x83, 0xfe, 0x3f, 0xf8, 0x1f, 0x0,
  0x1, 0xf0, 0xf, 0xf9, 0x83, 0xfe, 0x3f, 0xf0, 0x3f, 0x0,
  0x0, 0xf8, 0x3, 0xff, 0x83, 0xfe, 0x3f, 0xe0, 0x7e, 0x0,
  0x0, 0x7c, 0x1, 0xff, 0x83, 0xfe, 0x3f, 0x80, 0xfc, 0x0,
  0x0, 0x7e, 0x0, 0x7f, 0x83, 0xfe, 0x3f, 0x1, 0xf8, 0x0,
  0x0, 0x3f, 0x0, 0xf, 0x83, 0xfe, 0x38, 0x3, 0xf0, 0x0,
  0x0, 0x1f, 0xc0, 0x3, 0xc1, 0xf8, 0x20, 0x7, 0xe0, 0x0,
  0x0, 0x7, 0xe0, 0x0, 0x0, 0x0, 0x0, 0x1f, 0xc0, 0x0,
  0x0, 0x3, 0xf8, 0x0, 0x0, 0x0, 0x0, 0x7f, 0x80, 0x0,
  0x0, 0x1, 0xfe, 0x0, 0x0, 0x0, 0x0, 0xfe, 0x0, 0x0,
  0x0, 0x0, 0x7f, 0x80, 0x0, 0x0, 0x7, 0xfc, 0x0, 0x0,
  0x0, 0x0, 0x1f, 0xf0, 0x0, 0x0, 0x3f, 0xf0, 0x0, 0x0,
  0x0, 0x0, 0x7, 0xff, 0xe0, 0x1f, 0xff, 0xc0, 0x0, 0x0,
  0x0, 0x0, 0x1, 0xff, 0xff, 0xff, 0xff, 0x0, 0x0, 0x0,
  0x0, 0x0, 0x0, 0x3f, 0xff, 0xff, 0xf8, 0x0, 0x0, 0x0,
  0x0, 0x0, 0x0, 0x7, 0xff, 0xff, 0x80, 0x0, 0x0, 0x0,
  0x0, 0x0, 0x0, 0x0, 0xf, 0xc0, 0x0, 0x0, 0x0, 0x0,
  0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0
};

/*
  static const unsigned char PROGMEM logo_bmp[] =
  { B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000
  };
*/

char Time[]     = "  :  "; // char array for the time
char Date[]     = "  '/'  '/'    "; // char array for the time

byte second, minute, hour;

unsigned int data; // to read the temperature analog data and save it to data variable

volatile int second_pass = 0 ;
int seconds, minutes, hours, timerStep, days, months, years;
void T1Delay ();

long turn_up_time = 0;
int last_time, delay_time = 0;
int wite_time = 0;

// 2 objects from state enum to carry the current state and last state
enum state app_state,last_state;

// write a temp. word to the screen
void displayStaticText(int temprature)
{
  char str[12];
  snprintf(str, 12, "Temp.: %d", temprature);
  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.fillRect(94, 40, 36, 16, BLACK);
  display.display();      // Show initial text
  display.setCursor(10, 40);
  display.print(str);
  display.println();
  display.display();      // Show initial text
}

// display the temperature and time
void displayTemp(int temperature, char *text) {
  char str[12];
  snprintf(str, 12, "%d", temperature);
  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.fillRect(30, 20, 60, 16, BLACK);
  display.setCursor(30, 20);
  display.print(text);
  //Desired text size. 1 is default 6x8, 2 is 12x16, 3 is 18x24, etc
  // We start the line from 10 pixels and clac the space for 7 characters *12 =84 + 10 Then the size of the text that will change
  display.fillRect(94, 40, 36, 16, BLACK);
  display.display();      // Show initial text
  display.setCursor(94, 40);
  display.print(str);
  display.println();
  display.display();      // Show initial text  
}

// write the date to the oled
void displayDate(char *text) {

  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.setCursor(5, 0);
  display.print(text);
  display.println();
  display.display();      // Show initial text
}

void setup() {

  while (!Serial); // for Leonardo/Micro/Zero
  Serial.begin(115200);
  Serial.println("Serial started");
  seconds = 0;
  minutes = 0;
  hours = 0;
  days = 0;
  months = 0;
  years = 0;

 //Start the RTC and get the compile date and time from it.
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    //rtc.adjust(DateTime(2019, 8, 19, 11, 30, 0));
  }
  
  //make a variable DateTime from rtc.now() to get the date time details
  Serial.print("The time is : ");
  DateTime now = rtc.now();
  seconds = now.second();
  minutes = now.minute();
  hours = now.hour();
  days = now.day();
  months = now.month();
  years = now.year();  

  // add the time to time array
  snprintf(Time, 9, "%02d:%02d", now.hour(), now.minute());
  Serial.println(Time);

  // add the Date to Date array
  snprintf(Date, 11, "%02d/%02d/%02d", now.day(), now.month(), now.year());
  Serial.println(Date);

  DDRC   = 0X00;  // make Port C an input
  ADCSRA = 0x87;  // make ADC enable and select CLK/128
  ADMUX  = 0xE0;  // 2.56V Vref and ADC0 data will be left-justified
  //ADMUX  = 0xC0; // 2.56V Vref internal, right justified, select ADC Channel 0

  ADCSRA |= (1 << ADSC) ;          // start conversion
  while ( (ADCSRA & (1 << ADIF)) == 0 ); // wait for end
  data  = ADCH / 3; //The 10-bit output of the A/D is divided by 3 to get the real temperature.


  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }
  display.display();

 // set the app_state to first state after begin the oled
  app_state = BEGIN_STATE;
  last_state = BEGIN_STATE;

  cli(); // disable global interrupts

  // compare match mode
  TCCR1A = 0b00000000;//0; // Set prescaler 256 and CTC mode 4
  TCCR1B = 0b00001100; // Set prescaler 256 and CTC mode 4
  // set compare match register to desired timer count:
  OCR1A = 62499;

  // enable timer compare interrupt:
  TIMSK1 = 0b00000010;// |= (1 << OCIE0A);
  TCNT1  = 0; // sET THE compare match flag to 0
  sei(); // enable global interrupts
}

// Interrupt Service Routine
ISR (TIMER1_COMPA_vect)    // Timer1 ISR
{  
  second_pass = 1;
  seconds++;
  if (seconds == 60)
  {
    minutes = minutes + 1;
    seconds = 0;
  }
  if (minutes == 60)
  {
    hours = hours + 1;
    minutes = 0;
  }
  turn_up_time++;
}

void loop() { 

  // switch case to handle the 3 pages that will display on the OLED
  switch (app_state) {
    case BEGIN_STATE:    // First state show the adafruit logo
      last_state = BEGIN_STATE;
      app_state = WAIT_STATE;
      wite_time = 3;
      delay_time = 0;
      turn_up_time = 0;
      break;  
    case INITIAL_STATE:  // Second state show the DIYLife logo
      display.drawBitmap(25, 1, DIYlogo_bmp, 78, 62, WHITE);
      display.display();
      last_state = INITIAL_STATE;
      app_state = WAIT_STATE;
      wite_time = 2;
      break;
    case WAIT_STATE:  // Third state gife time before clear the display
      if (turn_up_time - delay_time >= wite_time) // Wait for 2 seconds
      {
        // Check the last state BEGIN_STATE or INITIAL_STATE 
        if (last_state == BEGIN_STATE){   
          app_state = INITIAL_STATE;
          last_state = WAIT_STATE;
        }
        if (last_state == INITIAL_STATE) {         
          app_state = LOOP_STATE;
          last_state = WAIT_STATE;
        }
        delay_time = turn_up_time;
        display.clearDisplay();
      }      
      break;
    case LOOP_STATE: // Forth and last state read and display the temperature with the date and time for ever.
      last_state = LOOP_STATE;
      if (second_pass == 1) {
        // Clear the buffer
        //display.clearDisplay();
        second_pass = 0;
        ADCSRA |= (1 << ADSC) ;          // start conversion
        while ( (ADCSRA & (1 << ADIF)) == 0 ); // wait for end
        data  = ADCH / 3; //The 10-bit output of the A/D is divided by 4 to get the real temperature.
        displayStaticText(data);
        displayDate(Date);
        snprintf(Time, 9, "%02d:%02d", hours, minutes);
        displayTemp(data, Time);
      }
  }

  // Get the date from RTC every 15 minutes
  if (turn_up_time - last_time >= 900) // 1/4 hour 15 m * 60 s
  {
    last_time = turn_up_time;
    DateTime now = rtc.now();
    days = now.day();
    months = now.month();
    years = now.year();
    snprintf(Date, 11, "%02d/%02d/%02d", now.day(), now.month(), now.year());
    Serial.println(Date);
    displayDate(Date);
  }

}
