#include <Arduino.h>
#include "stepperTimer.h"

void initTimer() {
  TCCR1A = 0;
  TCCR1B = bit(WGM12) | 0b011;
}

void setPeriod(uint32_t prd) {
  OCR1A = (uint32_t)prd >> 2;
}

void startTimer() {
  TIMSK1 = bit(OCIE1A);
  TCNT1 = 0;
}

void stopTimer() {
  TIMSK1 = 0;
  TCNT1 = 0;
}