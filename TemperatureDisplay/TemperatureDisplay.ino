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
//#include <time.h>

#include "RTClib.h"

#include <avr/interrupt.h>

RTC_DS1307 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

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

#define NUMFLAKES     10 // Number of snowflakes in the animation example

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16

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

//char Time[]     = "  :  :  ";
char Time[]     = "  :  ";
char Date[]     = "  '/'  '/'    ";
byte second, minute, hour;

unsigned int data;

volatile int second_pass = 0 ;
int seconds, minutes, hours, timerStep, days, months, years;
void T1Delay ();

int turn_up_time = 0;
int last_time = 0;

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
  Serial.print("The time is : ");
  DateTime now = rtc.now();
  seconds = now.second();
  minutes = now.minute();
  hours = now.hour();
  days = now.day();
  months = now.month();
  years = now.year();

  //  snprintf(Time, 9, "%02d:%02d:%02d", now.hour(), now.minute(), now.second());
  //  Serial.println(Time);

  snprintf(Time, 9, "%02d:%02d", now.hour(), now.minute());
  Serial.println(Time);

  snprintf(Date, 11, "%02d/%02d/%02d", now.day(), now.month(), now.year());
  Serial.println(Date);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();

  //  // Draw a single pixel in white
  //  display.drawPixel(10, 10, WHITE);
  //
  //  // Show the display buffer on the screen. You MUST call display() after
  //  // drawing commands to make them visible on screen!
  //  display.display();
  //  delay(2000);


  // Invert and restore display, pausing in-between
  //  display.invertDisplay(true);
  //  delay(1000);
  //  display.invertDisplay(false);
  //  delay(1000);

  //display.clearDisplay();

  display.drawBitmap(25, 1, DIYlogo_bmp, 78, 62, WHITE);
  display.display();
  delay(2000);

  DDRC   = 0X00;  // make Port C an input
  ADCSRA = 0x87;  // make ADC enable and select CLK/128
  ADMUX  = 0xE0;  // 2.56V Vref and ADC0 data will be left-justified
  //ADMUX  = 0xC0; // 2.56V Vref internal, right justified, select ADC Channel 0

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

  /*
    cli(); // disable global interrupts

    TCCR2A = 0b00000000;//0; // set entire TCCR1A register to 0
    TCCR2B = 0b00000111; // same for
    // enable timer compare interrupt:
    TIMSK2 = 0b00000001;// |= (1 << OCIE0A);
    sei(); // enable global interrupts
  */

  ADCSRA |= (1 << ADSC) ;          // start conversion
  while ( (ADCSRA & (1 << ADIF)) == 0 ); // wait for end
  data  = ADCH / 3; //The 10-bit output of the A/D is divided by 4 to get the real temperature.

  display.clearDisplay();

  displayStaticText(data);
  displayDate(Date);
  snprintf(Time, 9, "%02d:%02d", hours, minutes);
  displayTemp(data, Time);
}

//ISR(TIMER2_OVF_vect)
ISR (TIMER1_COMPA_vect)    // Timer1 ISR
{
  //  timerStep++;
  //  if (timerStep == 62)
  //  {
  //    timerStep = 0;
  //    seconds++;
  //    //minutes++;
  //    second_pass = 1;
  //  }
  turn_up_time++;
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
}

//ISR(TIMER2_OVF_vect)
//{
//  //  DateTime now = rtc.now();
//  timerStep++;
//  if (timerStep == 62)
//  {
//    timerStep = 0;
//    seconds++;
//    pr_seconds = 1;
//    //PORTB ^= 0x20;
//    //    Serial.println(" second ");
//    //    Serial.println(minutes);
//  }
//
//  //PORTB ^= 0x20;    // toggle PORTB.5
//  //seconds = seconds + 1;
//  //if (seconds == 3000)
//  if (seconds == 60)
//  {
//    minutes = minutes + 1;
//    seconds = 0;
//  }
//  if (minutes == 60)
//  {
//    hours = hours + 1;
//    minutes = 0;
//  }
//  //  Serial.print("time now: ");
//  //  Serial.print(hours);
//  //  Serial.print(":");
//  //  Serial.print(minutes);
//  //  Serial.print(":");
//  //  Serial.println(seconds);
//  //  pr_seconds = 0 ;
//  //  snprintf(Time, 9, "%02d:%02d:%02d", hours, minutes, seconds);
//  //snprintf(Time, 9, "%02d:%02d", hours, minutes);
//
//
//
//}

void loop() {
  if (second_pass == 1) {
    // Clear the buffer
    //display.clearDisplay();
    second_pass = 0;
    ADCSRA |= (1 << ADSC) ;          // start conversion
    while ( (ADCSRA & (1 << ADIF)) == 0 ); // wait for end
    data  = ADCH / 3; //The 10-bit output of the A/D is divided by 4 to get the real temperature.
    displayDate(Date);
    snprintf(Time, 9, "%02d:%02d", hours, minutes);
    displayTemp(data, Time);
    Serial.println(" minute ");
    Serial.println(minutes);
  }

  // Get the date from RTC every 15 minutes
  if(turn_up_time - last_time == 900) // 1/4 hour 15 m * 60 s  
  {
    DateTime now = rtc.now();
    days = now.day();
    months = now.month();
    years = now.year();   

    snprintf(Date, 11, "%02d/%02d/%02d", now.day(), now.month(), now.year());
    Serial.println(Date);
    displayDate(Date);
  }

}

void displayStaticText(int temprature)
{
  char str[12];
  snprintf(str, 12, "Temp.: %d", temprature);
  //display.clearDisplay();
  //display.clearDisplay();
  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.fillRect(94, 40, 36, 16, BLACK);
  display.display();      // Show initial text
  display.setCursor(10, 40);
  display.print(str);
  display.println();
  display.display();      // Show initial text
}

void displayTemp(int temprature, char *text) {
  char str[12];
  snprintf(str, 12, "%d", temprature);
  //display.clearDisplay();
  //display.clearDisplay();
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
  //delay(100);
}

void displayDate(char *text) {

  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.setCursor(5, 0);
  display.print(text);
  display.println();
  display.display();      // Show initial text
  //delay(100);
}

void helloOLED(void) {
  display.clearDisplay();

  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.setCursor(30, 0);            // Start at top-left corner
  display.println(F("The"));
  display.setCursor(15, 20);
  display.println(F("DIY Life"));
  display.display();      // Show initial text
  delay(100);
}
