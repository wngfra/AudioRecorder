// #include "arduino_secrets.h"
#include "cyclicBuffer.h"

#define L 512
#define FS 44100
#define ALPHA 0.2  // smoothing factor

typedef struct cyclicBuffer<unsigned int> AudioBuffer;

unsigned int smoothedVal;
unsigned int i, j;
unsigned int samplingPeriod;
unsigned long microSeconds;

AudioBuffer bufA(L), bufB(L);
AudioBuffer *bufPtrA, *bufPtrB;

void setup() {
  bufPtrA = &bufA;
  bufPtrB = &bufB;

  bufPtrA->push(0);

  samplingPeriod = round(1000000 * (1.0 / FS));  //Period in microseconds
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

  smoothedVal = (float)analogRead(A0) * ALPHA + (float)*(bufPtrA->last()) * (1.0 - ALPHA);
  bufPtrA->push(smoothedVal);

  Serial.println(smoothedVal);

  if (bufPtrA->is_full()) {
    swap(bufPtrA, bufPtrB);
  }

  /*remaining wait time between samples if necessary*/
  while (micros() < (microSeconds + samplingPeriod)) {}
}
