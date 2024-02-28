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

//
// AUX1    5V  GND D2  D1
//          2   4   6   8
//          1   3   5   7
//         5V  GND A3  A4
//
#define AUX1_05                               57  // (A3)
#define AUX1_06                                2
#define AUX1_07                               58  // (A4)
#define AUX1_08                                1

//
// AUX2    GND A9 D40 D42 A11
//          2   4   6   8  10
//          1   3   5   7   9
//         VCC A5 A10 D44 A12
//
#define AUX2_03                               59  // (A5)
#define AUX2_04                               63  // (A9)
#define AUX2_05                               64  // (A10)
#define AUX2_06                               40
#define AUX2_07                               44
#define AUX2_08                               42
#define AUX2_09                               66  // (A12)
#define AUX2_10                               65  // (A11)


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