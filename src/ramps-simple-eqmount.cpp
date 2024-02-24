#include <Arduino.h>
#include "pins.h"

#include <U8g2lib.h>
U8G2_ST7920_128X64_1_SW_SPI u8g2(U8G2_R0, DOGLCD_SCK, DOGLCD_MOSI, DOGLCD_CS);

#include <GyverStepper2.h>
GStepper2<STEPPER2WIRE> e0_stepper(200*16, E0_STEP_PIN, E0_DIR_PIN, E0_ENABLE_PIN);


#include <EncButton.h>
EncButton eb(BTN_EN1, BTN_EN2, BTN_ENC, INPUT_PULLUP, INPUT_PULLUP, LOW);

bool changes = true;

#define EXPERIMENTAL_SIDEREAL_SPEED  9.45
#define SIDEREAL_SPEED 8.913580247

float e0_speed = SIDEREAL_SPEED;

void setup() {
  pinMode(LED_PIN, OUTPUT);

  e0_stepper.setSpeed(e0_speed);
  e0_stepper.setMaxSpeed(1000);
  e0_stepper.setAcceleration(200);

  eb.setEncType(EB_STEP4_HIGH);
  u8g2.begin();

  Serial.begin(9600);
}



void loop() {
  eb.tick();

  if (eb.turn()) {
    if (eb.pressing()) {
      switch (eb.getClicks()) {
        case 0: e0_speed += 0.01 * eb.dir();;
          break;
        case 1: e0_speed += 0.5 * eb.dir();;
          break;
        case 2: e0_speed += 5 * eb.dir();;
          break;
      }

      //e0_speed += 0.01 * eb.dir();
      e0_stepper.setSpeed(e0_speed);
      changes = true;
    } else {
      e0_stepper.setTarget(e0_speed * eb.dir() * 5, RELATIVE);
      //e0_stepper.setTarget(32000 * eb.dir(), RELATIVE);
    }
  }


  if (e0_stepper.ready()) {
    e0_stepper.setSpeed(e0_speed);
  }


  e0_stepper.tick();

  if (millis() % 1000 < 500)
    digitalWrite(LED_PIN, HIGH);
  else
    digitalWrite(LED_PIN, LOW);

  if (changes) {
    changes = false;

    u8g2.firstPage();
    do {
      u8g2.clearBuffer();
      u8g2.setFont(u8g2_font_ncenB14_tr);
      u8g2.setCursor(0, 15);
      u8g2.print("Speed = ");
      u8g2.print(e0_speed);
      u8g2.sendBuffer();
    } while (u8g2.nextPage());
  }
}
