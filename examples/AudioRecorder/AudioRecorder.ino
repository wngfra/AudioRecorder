// #include "arduino_secrets.h"
#include "cyclicBuffer.h"

#define L 128
#define Fs 44100

typedef struct cyclicBuffer<unsigned int> AudioBuffer;

unsigned int meanVal;
unsigned int i, j;
unsigned int samplingPeriod;
unsigned long microSeconds;

AudioBuffer bufA(L), bufB(L);
AudioBuffer *bufPtrA, *bufPtrB;

void setup() {
  bufPtrA = &bufA;
  bufPtrB = &bufB;
  meanVal = 0;

  samplingPeriod = round(1000000 * (1.0 / Fs));  //Period in microseconds
  microSeconds = micros();

  Serial.begin(115200);
}

void swap(AudioBuffer *&pA, AudioBuffer *&pB) {
  AudioBuffer *tmp = pA;
  pA = pB;
  pB = tmp;
}

void loop() {
  microSeconds = micros();
  bufPtrA->push(analogRead(A0));

  if (bufPtrA->is_full()) {
    swap(bufPtrA, bufPtrB);
  }

  if (bufPtrB->is_full()) {
    meanVal = 0;
    for (i = 0; i < L; ++i) {
      meanVal += *(bufPtrB->pop());
    }
    Serial.println((int)(meanVal / L));
  }

  /*remaining wait time between samples if necessary*/
  while (micros() < (microSeconds + samplingPeriod)) {}
}
