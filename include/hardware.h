#ifdef PINOUT_RAMPS

#define RA_ENCODER_MODE         INPUT_PULLUP
#define RA_ENCODER_TYPE         EB_STEP4_HIGH
#define RA_ENCODER_BTN_MODE     INPUT_PULLUP
#define RA_ENCODER_BTN_LEVEL    LOW

#endif

#ifdef PINOUT_NANO

#define RA_ENCODER_MODE         INPUT
#define RA_ENCODER_TYPE         EB_STEP4_LOW
#define RA_ENCODER_BTN_MODE     INPUT
#define RA_ENCODER_BTN_LEVEL    LOW

#endif