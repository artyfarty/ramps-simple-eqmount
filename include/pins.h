#ifdef PINOUT_RAMPS

#include "pins_ramps.h"
#include "pins_ramps_lcd.h"

#define RA_STEPPER_STEP_PIN     E0_STEP_PIN
#define RA_STEPPER_DIR_PIN      E0_DIR_PIN
#define RA_STEPPER_ENABLE_PIN   E0_ENABLE_PIN


#define RA_ENCODER_PIN1_PIN     BTN_EN1
#define RA_ENCODER_PIN2_PIN     BTN_EN2
#define RA_ENCODER_BTN_PIN      BTN_ENC


#ifdef USE_REMOTE
    #define RMT_ENCODER_PIN1_PIN    AUX2_07
    #define RMT_ENCODER_PIN2_PIN    AUX2_06
    #define RMT_ENCODER_BTN_PIN     AUX2_08
#endif

#define LCD_CLOCK_PIN           DOGLCD_SCK
#define LCD_DATA_PIN            DOGLCD_MOSI
#define LCD_CS_PIN              DOGLCD_CS
#define LCD_RST_PIN             U8X8_PIN_NONE

#endif

#ifdef PINOUT_NANO
#include "pins_nano.h"

#define RA_STEPPER_STEP_PIN     PIN_D9
#define RA_STEPPER_DIR_PIN      PIN_D8
#define RA_STEPPER_ENABLE_PIN   PIN_D7

#define RA_ENCODER_PIN1_PIN     PIN_D6
#define RA_ENCODER_PIN2_PIN     PIN_D5
#define RA_ENCODER_BTN_PIN      PIN_D4

#endif