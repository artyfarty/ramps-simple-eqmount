#include <Arduino.h>
#include "pins.h"
#include "hardware.h"
#include "stepperTimer.h"

#ifdef USE_LCD
  #include <U8g2lib.h>
  U8G2_ST7920_128X64_1_SW_SPI u8g2(U8G2_R0, LCD_CLOCK_PIN, LCD_DATA_PIN, LCD_CS_PIN);

  uint16_t lcd_indicator_icon = 0;

  #define INDICATOR_L 0x25c0
  #define INDICATOR_R 0x25b6
#endif

bool update_indicators = true;
bool blinker = false;
unsigned long previousMillis = 0;
const long interval = 1000;

#include <GyverStepper2.h>
#define GS_NO_ACCEL
GStepper2<STEPPER2WIRE> ra_stepper(200*16, RA_STEPPER_STEP_PIN, RA_STEPPER_DIR_PIN, RA_STEPPER_ENABLE_PIN);

#define EB_NO_CALLBACK
#define EB_DEB_TIME     50 
#define EB_CLICK_TIME   500
#define EB_HOLD_TIME    600
#define EB_STEP_TIME    200
#define EB_FAST_TIME    30

#include <EncButton.h>
EncButtonT<RA_ENCODER_PIN1_PIN, RA_ENCODER_PIN2_PIN, RA_ENCODER_BTN_PIN> main_encoder(RA_ENCODER_MODE, RA_ENCODER_BTN_MODE, RA_ENCODER_BTN_LEVEL);

#ifdef USE_REMOTE
EncButtonT<RMT_ENCODER_PIN1_PIN, RMT_ENCODER_PIN2_PIN, RMT_ENCODER_BTN_PIN> rmt_encoder(RMT_ENCODER_MODE, RMT_ENCODER_BTN_MODE, RMT_ENCODER_BTN_LEVEL);
#endif

#include "sidereal_speed.h"

float ra_speed = SIDEREAL_SPEED;

void setSpeed(double new_speed) {
  ra_speed = new_speed;
  
  ra_stepper.setSpeed(ra_speed);

  setPeriod(ra_stepper.getPeriod());
  startTimer();
  update_indicators = true;
}

void shuttle(int32_t multipler, int8_t dir) {
  #ifdef USE_LCD
    lcd_indicator_icon = (dir > 0) ? INDICATOR_R : INDICATOR_L;
  #endif

  update_indicators = true;

  ra_stepper.setTarget(ra_stepper.getTarget() + (ra_speed * dir * multipler));
  setPeriod(ra_stepper.getPeriod());
}

void setup() {
  initTimer();
  ra_stepper.setAcceleration(600);
  ra_stepper.setMaxSpeed(16000);

  setSpeed(SIDEREAL_SPEED);

  main_encoder.setEncType(RA_ENCODER_TYPE);

  #ifdef USE_REMOTE
    rmt_encoder.setEncType(RMT_ENCODER_TYPE);
  #endif 

  #ifdef USE_LCD
    u8g2.begin();
  #endif

  Serial.begin(9600);
}

ISR(TIMER1_COMPA_vect) {
  if (ra_stepper.tickManual()) setPeriod(ra_stepper.getPeriod());
  else stopTimer();
}


void loop() {
  main_encoder.tick();

  if (main_encoder.turn()) {
    if (main_encoder.pressing()) {
      switch (main_encoder.getClicks()) {
        case 0:
          shuttle(100, main_encoder.dir());
          break;
        case 1: 
          setSpeed(ra_speed + 0.01 * main_encoder.dir());
          break;
        case 2:
          setSpeed(ra_speed + 0.5 * main_encoder.dir());
          break;
        case 3:
          setSpeed(ra_speed + 5 * main_encoder.dir());
          break;
      }

    } else {
      shuttle(10, main_encoder.dir());
    }
  }

  #ifdef USE_REMOTE
    rmt_encoder.tick();
    if (rmt_encoder.turn()) {
      if (rmt_encoder.pressing()) {
        switch (rmt_encoder.getClicks()) {
          case 0:
            shuttle(10, rmt_encoder.dir());
            break;
          case 1: 
            shuttle(100, rmt_encoder.dir());
            break;
          case 2: 
            shuttle(200, rmt_encoder.dir());
            break;
        }

      } else {
        shuttle(10, rmt_encoder.dir());
      }
    }
  #endif


  if (ra_stepper.ready()) {
    #ifdef USE_LCD
      lcd_indicator_icon = 0;
    #endif

    update_indicators = true;
    setSpeed(ra_speed);
  }

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    blinker = !blinker;
    update_indicators = true;
  }

  #ifdef USE_LCD
    if (update_indicators) {
      update_indicators = false;

      u8g2.firstPage();
      do {
        u8g2.clearBuffer();

        if (blinker) {
          u8g2.setFont(u8g2_font_unifont_t_weather);
          u8g2.drawGlyph(0, 15, 0x0030); 
        }
        if (lcd_indicator_icon != 0) {
          u8g2.setFont(u8g2_font_9x15_t_symbols);
          u8g2.drawGlyph(110, 60, lcd_indicator_icon); 
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
        u8g2.print(ra_stepper.getCurrent());
        if (ra_stepper.getStatus() == 1) {
          u8g2.print(" > ");
          u8g2.print(ra_stepper.getTarget());
        }
        
        u8g2.setCursor(0, 60);
        u8g2.print("enc: ");
        u8g2.print(main_encoder.counter);

        #ifdef USE_REMOTE
          u8g2.print(" rmt: ");
          u8g2.print(rmt_encoder.counter);
        #endif

        u8g2.sendBuffer();
      } while (u8g2.nextPage());
    }
  #endif
}
