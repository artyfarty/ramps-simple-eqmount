// For RAMPS 1.4
#define X_STEP_PIN 54
#define X_DIR_PIN 55
#define X_ENABLE_PIN 38
#define X_MIN_PIN 3
#define X_MAX_PIN -1  //PIN 2 is used

#define Y_STEP_PIN 60
#define Y_DIR_PIN 61
#define Y_ENABLE_PIN 56
#define Y_MIN_PIN 14
#define Y_MAX_PIN -1  //PIN 15 is used

#define Z_STEP_PIN 46
#define Z_DIR_PIN 48
#define Z_ENABLE_PIN 62
#define Z_MIN_PIN 18
#define Z_MAX_PIN -1  //PIN 19 is used

//extruder 1
#define E0_STEP_PIN 26
#define E0_DIR_PIN 28
#define E0_ENABLE_PIN 24

//extruder 2
#define E1_STEP_PIN 36
#define E1_DIR_PIN 34
#define E1_ENABLE_PIN 30


#define SDPOWER -1

//ChipSelect, Hardware SS Pin on Mega, 10 for Arduino Boards, always kept as output
#define SDCS_PIN 53

#define LED_PIN 13

#define FAN_PIN 9

#define PS_ON_PIN 12  //ATX , awake=LOW, SLEEP=High

#define HEATER_0_PIN 10  // Extruder Heater
#define HEATER_1_PIN 8

#define TEMP_0_PIN 13  // ANALOG NUMBERING
#define TEMP_1_PIN 14  // ANALOG NUMBERING

// DisplayBoard pins


//
// AUX3    GND D52 D50 5V
//          7   5   3   1
//          8   6   4   2
//         NC  D53 D51 D49
//
#define AUX3_02 49
#define AUX3_03 50
#define AUX3_04 51
#define AUX3_05 52
#define AUX3_06 53

//
// AUX4    5V GND D32 D47 D45 D43 D41 D39 D37 D35 D33 D31 D29 D27 D25 D23 D17 D16
//
#define AUX4_03 32
#define AUX4_04 47
#define AUX4_05 45
#define AUX4_06 43
#define AUX4_07 41
#define AUX4_08 39
#define AUX4_09 37
#define AUX4_10 35
#define AUX4_11 33
#define AUX4_12 31
#define AUX4_13 29
#define AUX4_14 27
#define AUX4_15 25
#define AUX4_16 23
#define AUX4_17 17
#define AUX4_18 16

#define EXP1_01_PIN AUX4_09  // 37
#define EXP1_02_PIN AUX4_10  // 35
#define EXP1_03_PIN AUX4_17  // 17
#define EXP1_04_PIN AUX4_18  // 16
#define EXP1_05_PIN AUX4_16  // 23
#define EXP1_06_PIN AUX4_15  // 25
#define EXP1_07_PIN AUX4_14  // 27
#define EXP1_08_PIN AUX4_13  // 29
#define EXP2_01_PIN AUX3_03  // 50 (MISO)
#define EXP2_02_PIN AUX3_05  // 52
#define EXP2_03_PIN AUX4_11  // 33
#define EXP2_04_PIN AUX3_06  // 53
#define EXP2_05_PIN AUX4_12  // 31
#define EXP2_06_PIN AUX3_04  // 51
#define EXP2_07_PIN AUX3_02  // 49
#define EXP2_08_PIN -1       // RESET

#define DOGLCD_CS EXP1_04_PIN
#define DOGLCD_MOSI EXP1_03_PIN
#define DOGLCD_SCK EXP1_05_PIN

#define BTN_ENC 35
#define BTN_EN1 33
#define BTN_EN2 31