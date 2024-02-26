#include <Arduino.h>
#include "pins.h"
#include "stepperTimer.h"

#include <U8g2lib.h>
U8G2_ST7920_128X64_1_SW_SPI u8g2(U8G2_R0, LCD_CLOCK_PIN, LCD_DATA_PIN, LCD_CS_PIN);

#include <GyverStepper2.h>
GStepper2<STEPPER2WIRE> ra_stepper(200*16, RA_STEPPER_STEP_PIN, RA_STEPPER_DIR_PIN, RA_STEPPER_ENABLE_PIN);

#include <EncButton.h>
EncButton eb(RA_ENCODER_PIN1_PIN, RA_ENCODER_PIN2_PIN, RA_ENCODER_BTN_PIN, INPUT_PULLUP, INPUT_PULLUP, LOW);

bool redraw_lcd = true;
uint16_t lcd_indicator_icon = 0;

bool blinker = false;
unsigned long previousMillis = 0;
const long interval = 1000;

#define INDICATOR_L 0x25c0
#define INDICATOR_R 0x25b6

#define SIDEREAL_SPEED 8.54218107

float ra_speed = SIDEREAL_SPEED;

void setSpeed(double new_speed) {
  ra_speed = new_speed;
  
  ra_stepper.setSpeed(ra_speed);

  setPeriod(ra_stepper.getPeriod());
  startTimer();
  redraw_lcd = true;
}

void shuttle(int32_t multipler, int8_t dir) {
  lcd_indicator_icon = (dir > 0) ? INDICATOR_R : INDICATOR_L;
  redraw_lcd = true;

  ra_stepper.setTarget(ra_speed * dir * multipler, RELATIVE);
  //setPeriod(ra_stepper.getPeriod());
}

void setup() {
  initTimer();
  ra_stepper.setAcceleration(600);
  ra_stepper.setMaxSpeed(1000);

  setSpeed(SIDEREAL_SPEED);

  eb.setEncType(EB_STEP4_HIGH);
  u8g2.begin();

  Serial.begin(9600);
}

ISR(TIMER1_COMPA_vect) {
  if (ra_stepper.tickManual()) setPeriod(ra_stepper.getPeriod());
  else stopTimer();
}


void loop() {
  eb.tick();

  if (eb.turn()) {
    if (eb.pressing()) {
      switch (eb.getClicks()) {
        case 0:
          shuttle(100, eb.dir());
          break;
        case 1: 
          setSpeed(ra_speed + 0.01 * eb.dir());
          break;
        case 2:
          setSpeed(ra_speed + 0.5 * eb.dir());
          break;
        case 3:
          setSpeed(ra_speed + 5 * eb.dir());
          break;
      }

    } else {
      shuttle(10, eb.dir());
    }
  }


  if (ra_stepper.ready()) {
    lcd_indicator_icon = 0;
    redraw_lcd = true;
    setSpeed(ra_speed);
  }

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    blinker = !blinker;
    redraw_lcd = true;
  }

  if (redraw_lcd) {
    redraw_lcd = false;

    u8g2.firstPage();
    do {
      u8g2.clearBuffer();

      if (blinker) {
        u8g2.setFont(u8g2_font_unifont_t_weather);
        u8g2.drawGlyph(0, 15, 0x0030); 
      }
      if (lcd_indicator_icon != 0) {
        u8g2.setFont(u8g2_font_9x15_t_symbols);
        u8g2.drawGlyph(0, 60, lcd_indicator_icon); 
      }

      u8g2.setFont(u8g2_font_ncenB14_tr);
      u8g2.setCursor(20, 15);
      u8g2.print("Startrack");
      u8g2.setFont(u8g2_font_6x12_t_symbols);
      u8g2.setCursor(0, 32);
      u8g2.print("spd: ");
      u8g2.print(ra_speed);
      u8g2.print("st/s");
      u8g2.setCursor(0, 45);
      u8g2.print("pos: ");
      u8g2.print(ra_stepper.pos);

      u8g2.sendBuffer();
    } while (u8g2.nextPage());
  }
}
