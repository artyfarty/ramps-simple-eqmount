#include <Arduino.h>
#include "pins.h"
#include "stepperTimer.h"

#include <U8g2lib.h>
U8G2_ST7920_128X64_1_SW_SPI u8g2(U8G2_R0, DOGLCD_SCK, DOGLCD_MOSI, DOGLCD_CS);

#include <GyverStepper2.h>
GStepper2<STEPPER2WIRE> e0_stepper(200*16, E0_STEP_PIN, E0_DIR_PIN, E0_ENABLE_PIN);

#include <EncButton.h>
EncButton eb(BTN_EN1, BTN_EN2, BTN_ENC, INPUT_PULLUP, INPUT_PULLUP, LOW);

bool changes = true;
uint16_t indicator = 0;

bool blinker = false;
unsigned long previousMillis = 0;
const long interval = 1000;


#define INDICATOR_L 0x25c0
#define INDICATOR_R 0x25b6

#define EXPERIMENTAL_SIDEREAL_SPEED  9.45
#define SIDEREAL_SPEED 8.54218107

float e0_speed = SIDEREAL_SPEED;

void setSpeed(double new_speed) {
  e0_speed = new_speed;
  
  e0_stepper.setSpeed(e0_speed);

  setPeriod(e0_stepper.getPeriod());
  startTimer();
  changes = true;
}

void shuttle(int32_t multipler, int8_t dir) {
  indicator = (dir > 0) ? INDICATOR_R : INDICATOR_L;
  changes = true;

  e0_stepper.setTarget(e0_speed * dir * multipler, RELATIVE);
  //setPeriod(e0_stepper.getPeriod());
}

void setup() {
  initTimer();
  e0_stepper.setAcceleration(600);
  e0_stepper.setMaxSpeed(1000);

  setSpeed(SIDEREAL_SPEED);

  eb.setEncType(EB_STEP4_HIGH);
  u8g2.begin();

  Serial.begin(9600);
}

ISR(TIMER1_COMPA_vect) {
  if (e0_stepper.tickManual()) setPeriod(e0_stepper.getPeriod());
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
          setSpeed(e0_speed + 0.01 * eb.dir());
          break;
        case 2:
          setSpeed(e0_speed + 0.5 * eb.dir());
          break;
        case 3:
          setSpeed(e0_speed + 5 * eb.dir());
          break;
      }

    } else {
      shuttle(10, eb.dir());
    }
  }


  if (e0_stepper.ready()) {
    indicator = 0;
    changes = true;
    setSpeed(e0_speed);
  }

  //e0_stepper.tick();
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    blinker = !blinker;
    changes = true;
  }

  if (changes) {
    changes = false;

    u8g2.firstPage();
    do {
      u8g2.clearBuffer();

      if (blinker) {
        u8g2.setFont(u8g2_font_unifont_t_weather);
        u8g2.drawGlyph(0, 15, 0x0030); 
      }
      if (indicator != 0) {
        u8g2.setFont(u8g2_font_9x15_t_symbols);
        u8g2.drawGlyph(0, 60, indicator); 
      }

      u8g2.setFont(u8g2_font_ncenB14_tr);
      u8g2.setCursor(20, 15);
      u8g2.print("Startrack");
      u8g2.setFont(u8g2_font_6x12_t_symbols);
      u8g2.setCursor(0, 32);
      u8g2.print("spd: ");
      u8g2.print(e0_speed);
      u8g2.print("st/s");
      u8g2.setCursor(0, 45);
      u8g2.print("pos: ");
      u8g2.print(e0_stepper.pos);

      u8g2.sendBuffer();
    } while (u8g2.nextPage());
  }
}
