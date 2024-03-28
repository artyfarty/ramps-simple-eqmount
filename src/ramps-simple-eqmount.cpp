#include <Arduino.h>
#include "pins.h"
#include "hardware.h"
#include "stepperTimer.h"

#ifdef DEBUG
#include <ArduinoLog.h>
#endif

#ifdef USE_LCD
  #include <U8g2lib.h>
  U8G2_ST7920_128X64_1_SW_SPI u8g2(U8G2_R0, LCD_CLOCK_PIN, LCD_DATA_PIN, LCD_CS_PIN);

  uint16_t lcd_indicator_icon = 0;

  #define INDICATOR_L 0x25c0
  #define INDICATOR_R 0x25b6
#endif

bool update_indicators = true;
bool blinker = false;
unsigned long currentMillis = 0;
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

#ifdef USE_MAIN_ENCODER
EncButtonT<RA_ENCODER_PIN1_PIN, RA_ENCODER_PIN2_PIN, RA_ENCODER_BTN_PIN> main_encoder(RA_ENCODER_MODE, RA_ENCODER_BTN_MODE, RA_ENCODER_BTN_LEVEL);
#endif

#ifdef USE_REMOTE
EncButtonT<RMT_ENCODER_PIN1_PIN, RMT_ENCODER_PIN2_PIN, RMT_ENCODER_BTN_PIN> rmt_encoder(RMT_ENCODER_MODE, RMT_ENCODER_BTN_MODE, RMT_ENCODER_BTN_LEVEL);
#endif

#ifdef USE_FINE_REMOTE
EncoderT<FRMT_ENCODER_PIN1_PIN, FRMT_ENCODER_PIN2_PIN> frmt_encoder(FRMT_ENCODER_MODE);

unsigned long frmt_paused_since = 0;
const long frmt_pause_length = 1000;
#endif

#include "sidereal_speed.h"

float ra_speed = SIDEREAL_SPEED;

#define FINE_MULT_DEFAULT 15
short fine_multiplier;
short coarse_multiplier;


void startSidereal() {
  #ifdef DEBUG
    Log.traceln("Starting sidereal at %F ", ra_speed);
  #endif

  ra_stepper.setSpeed(ra_speed);
  setPeriod(ra_stepper.getPeriod());
  startTimer();
}

void stopSidereal() {
  #ifdef DEBUG
    Log.traceln("Stopping sidereal");
  #endif
  
  #ifdef USE_FINE_REMOTE
  frmt_paused_since = currentMillis;
  #endif

  ra_stepper.brake();
  stopTimer();
}

void setSpeed(double new_speed) {
  #ifdef DEBUG
    Log.traceln("New speed = %F ", ra_speed);
  #endif

  ra_speed = new_speed;

  startSidereal();
  update_indicators = true;
}

void setShuttleSpeeds(short new_speed) {
  fine_multiplier = new_speed;
  coarse_multiplier = new_speed * 20;

  update_indicators = true;

  #ifdef DEBUG
    Log.traceln("New shuttle speeds = %d/%d ", fine_multiplier, coarse_multiplier);
  #endif
}

void shuttle(int32_t multipler, int8_t dir) {
  #ifdef USE_LCD
    lcd_indicator_icon = (dir > 0) ? INDICATOR_R : INDICATOR_L;
  #endif

  #ifdef DEBUG
    Log.traceln("Shuttle order %d, dir: %d", multipler, dir);
  #endif

  update_indicators = true;

  ra_stepper.setTarget(ra_stepper.getCurrent() + (ra_speed * dir * multipler));
  setPeriod(ra_stepper.getPeriod());
}

#ifdef USE_MAIN_ENCODER
void handleMainEnc() {
  main_encoder.tick();

  if (main_encoder.turn()) {
    #ifdef DEBUG
      Log.traceln("Main encoder turn!");
    #endif
    if (main_encoder.pressing()) {
      switch (main_encoder.getClicks()) {
        case 0:
          shuttle(coarse_multiplier, main_encoder.dir());
          break;
        case 1: 
          setShuttleSpeeds(fine_multiplier + 1 * main_encoder.dir());
          break;
        case 2: 
          setSpeed(ra_speed + 0.01 * main_encoder.dir());
          break;
        case 3:
          setSpeed(ra_speed + 1 * main_encoder.dir());
          break;
      }

    } else {
      shuttle(fine_multiplier, main_encoder.dir());
    }
  }
}
#endif

#ifdef USE_REMOTE
void handleRemoteEncoder() {
    rmt_encoder.tick();
    if (rmt_encoder.turn()) {
      #ifdef DEBUG
        Log.traceln("Remote encoder turn!");
      #endif
      if (rmt_encoder.pressing()) {
        switch (rmt_encoder.getClicks()) {
          case 0:
            shuttle(fine_multiplier, rmt_encoder.dir());
            break;
          case 1: 
            shuttle(coarse_multiplier, rmt_encoder.dir());
            break;
          case 2: 
            shuttle(coarse_multiplier * 3, rmt_encoder.dir());
            break;
        }

      } else {
        shuttle(10, rmt_encoder.dir());
      }
    }
}
#endif

#ifdef USE_FINE_REMOTE
void handleFineEncoder() {
    frmt_encoder.tick();

    if (frmt_encoder.turn()) {
      #ifdef DEBUG
        Log.traceln("Fine remote encoder turn!");
      #endif
      stopSidereal();
      ra_stepper.dir = frmt_encoder.dir();
      ra_stepper.step();
    } else {
      if (ra_stepper.getStatus() == 0 && (frmt_paused_since + frmt_pause_length) < currentMillis ) {
        #ifdef DEBUG
          Log.traceln("Resuming sidereal after fine enc");
        #endif
        startSidereal();
      }
    }
}
#endif

#ifdef USE_LCD
void handleLCD() {
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
        u8g2.print("Adj: ");
        u8g2.print(fine_multiplier);

        /*
        u8g2.setCursor(0, 60);
        u8g2.print("enc: ");
        u8g2.print(main_encoder.counter);

        #ifdef USE_REMOTE
          u8g2.print(" rmt: ");
          u8g2.print(rmt_encoder.counter);
        #endif
        */

        u8g2.sendBuffer();
      } while (u8g2.nextPage());
    }
}
#endif

#ifdef ENC_ISR
void encISR() {
  #ifdef USE_REMOTE
    rmt_encoder.tickISR();
  #endif

  #ifdef USE_FINE_REMOTE
    frmt_encoder.tickISR();
  #endif
}
#endif

void setup() {
  #ifdef DEBUG
    Serial.begin(9600);
    delay(1000);
    Serial.print("INIT LOGGER");

    Log.begin(LOG_LEVEL_VERBOSE, &Serial);
    Log.traceln("Hello");
  #endif

  currentMillis = millis();
  initTimer();
  ra_stepper.setMaxSpeed(16000);
  ra_stepper.enable();

  setSpeed(SIDEREAL_SPEED);
  setShuttleSpeeds(FINE_MULT_DEFAULT);

  #ifdef USE_MAIN_ENCODER
    main_encoder.setEncType(RA_ENCODER_TYPE);
  #endif

  #ifdef USE_REMOTE
    rmt_encoder.setEncType(RMT_ENCODER_TYPE);
  #endif 

  #ifdef USE_FINE_REMOTE
    frmt_encoder.setEncType(FRMT_ENCODER_TYPE);
  #endif

  #ifdef USE_LCD
    u8g2.begin();
  #endif

  #ifdef ENC_ISR
    attachInterrupt(ENC_ISR_INT0, encISR, CHANGE);
    attachInterrupt(ENC_ISR_INT1, encISR, CHANGE);
    
    #ifdef USE_REMOTE
      rmt_encoder.setEncISR(true);
    #endif

    #ifdef USE_FINE_REMOTE
      frmt_encoder.setEncISR(true);
    #endif
  #endif

  #ifdef DEBUG
    Log.traceln("Setup finished");
  #endif
}

ISR(TIMER1_COMPA_vect) {
  if (ra_stepper.tickManual()) setPeriod(ra_stepper.getPeriod());
  else stopTimer();
}


void loop() {
  currentMillis = millis();

  #ifdef USE_MAIN_ENCODER
    handleMainEnc();
  #endif

  #ifdef USE_REMOTE
    handleRemoteEncoder();
  #endif

  #ifdef USE_FINE_REMOTE
    handleFineEncoder();
  #endif

  if (ra_stepper.ready()) {
    #ifdef USE_LCD
      lcd_indicator_icon = 0;
    #endif

    #ifdef DEBUG
      Log.traceln("Resuming sidereal after shuttle");
    #endif

    update_indicators = true;
    setSpeed(ra_speed);
  }

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    blinker = !blinker;
    update_indicators = true;
    #ifdef DEBUG
    Log.traceln("heartbeat");
    #endif
  }

  #ifdef USE_LCD
    handleLCD();
  #endif
}
