#ifdef PINOUT_RAMPS

#define RA_ENCODER_MODE         INPUT_PULLUP
#define RA_ENCODER_TYPE         EB_STEP4_LOW
#define RA_ENCODER_BTN_MODE     INPUT_PULLUP
#define RA_ENCODER_BTN_LEVEL    LOW

#endif

#ifdef USE_REMOTE

#define RMT_ENCODER_MODE         INPUT
#define RMT_ENCODER_TYPE         EB_STEP2
#define RMT_ENCODER_BTN_MODE     INPUT
#define RMT_ENCODER_BTN_LEVEL    LOW

#endif

#ifdef PINOUT_NANO

#define RA_ENCODER_MODE         INPUT
#define RA_ENCODER_TYPE         EB_STEP4_LOW
#define RA_ENCODER_BTN_MODE     INPUT
#define RA_ENCODER_BTN_LEVEL    LOW

#endif