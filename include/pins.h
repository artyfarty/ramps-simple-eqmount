#include "pins_ramps.h"
#include "pins_ramps_lcd.h"

#define RA_STEPPER_STEP_PIN     E0_STEP_PIN
#define RA_STEPPER_DIR_PIN      E0_DIR_PIN
#define RA_STEPPER_ENABLE_PIN   E0_ENABLE_PIN

#define RA_ENCODER_PIN1_PIN     BTN_EN1
#define RA_ENCODER_PIN2_PIN     BTN_EN2
#define RA_ENCODER_BTN_PIN      BTN_ENC

#define LCD_CLOCK_PIN           DOGLCD_SCK
#define LCD_DATA_PIN            DOGLCD_MOSI
#define LCD_CS_PIN              DOGLCD_CS
#define LCD_RST_PIN             U8X8_PIN_NONE