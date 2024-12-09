// STM32F401RE_TIM.c
// TIM functions

#include "STM32L432KC_TIM.h"
#include "STM32L432KC_RCC.h"

void initTIM(TIM_TypeDef * TIMx){
  // Set prescaler to give 1 us time base
  uint32_t psc_div = (uint32_t) ((SystemCoreClock/1e6));

  // Set prescaler division factor
  TIMx->PSC = (psc_div - 1);
  // Generate an update event to update prescaler value
  TIMx->EGR |= 1;
  // Enable counter
  TIMx->CR1 |= 1; // Set CEN = 1
}

void delay_millis(TIM_TypeDef * TIMx, uint32_t ms){
  TIMx->ARR = ms;// Set timer max count
  TIMx->EGR |= 1;     // Force update
  TIMx->SR &= ~(0x1); // Clear UIF
  TIMx->CNT = 0;      // Reset count

  while(!(TIMx->SR & 1)); // Wait for UIF to go high
}

void setUpPWM(int frequency, int dutyCycle) {
    //TIM16->TIM_PSC |= (0b1 << 1); // set clock prescaler
    TIM15->CR1 |= (0b1 << 7); // enable auto preload reload
    TIM15->ARR &= 0;
    TIM15->ARR = (984533/frequency); // set the auto reload register


    TIM15->CCER |= (0b1 << 0); // enable capture compare 1
    TIM15->CCR1 = TIM15->ARR * (dutyCycle/100.0); // 50%  duty cycle
    TIM15->CCMR1 &= ~(0b11 << 0); // set CC1 as output
    TIM15->CCMR1 |= (0b1 << 6); // setting to pwm mode 1
    TIM15->CCMR1 |= (0b1 << 5); // setting to pwm mode 1
    
    TIM15->BDTR |= (0b1 << 15); // enable main output enable 
    
    TIM15->CCMR1 |= (0b1 << 3); // enable pre load register

   
    
    TIM15->EGR |= (0b1 << 0); // enableUpdate generation
    TIM15->CR1 |= (0b1 << 0); //enable the counter for timer 16
}

void changeFreq(int frequency, int dutyCycle) {
  TIM15->ARR = (4 * 984533/frequency);
  TIM15->CCR1 = TIM16->ARR * (dutyCycle/100.0);
  TIM15->EGR |= (0b1 << 0);
}

void setUpTIM6() {
    TIM6->PSC = 0b1010; // scale clock down by 10
    TIM6->EGR |= (0b1 << 0); // enableUpdate generation
    TIM6->SR &= ~(0b1 << 0);
    TIM6->CR1 |= (0b1 << 0); // enable the counter of tim6
}
void delayTIM6(int ms) {
    TIM6->ARR = (ms*98.453*4);
    while (TIM6->SR == 0){
    
    }
    TIM6->SR &= ~(0b1 << 0);
}

void delay(TIM_TypeDef* TIM, int ms) {
  TIM->ARR = ms;
  TIM->SR &= ~(1 << 0);
  TIM->CNT = 0;
  // wait on counter overflow
  while (!(TIM->SR)) {}
}
/*
void genSqWave(int freq, int dur, int pin) {
  if (freq == 0) {
    delay(TIM6, dur);
  } else {
    TIM6->ARR = 4 * dur;
    setArr(TIM2, 1000000 / (2 * freq));
    TIM15->ARR = 1000000 * 4 / (2 * freq);
    resetFlag(TIM1);
    resetFlag(TIM2);
    resetCount(TIM1);
    resetCount(TIM2);
    // wait on counter overflow
    while (!(TIM1->SR)) {
      digitalWrite(pin, 1, GPIO);
      // wait on counter overflow
      while (!(TIM2->SR)) {}
      resetFlag(TIM2);
      digitalWrite(pin, 0, GPIO);
      // wait on counter overflow
      while (!(TIM2->SR)) {}
      resetFlag(TIM2);
    }
  }
}

void play(){
  setUpTIM6();
  setUpPWM(0, 50);
  const int notes[][2] = {
  {659,	125},
  {623,	125},
  {659,	125},
  {623,	125},
  {659,	125},
  {494,	125},
  {587,	125},
  {523,	125},
  {440,	250},
  {  0,	125},
  {262,	125},
  {330,	125},
  {440,	125},
  {494,	250},
  {  0,	125},
  {330,	125},
  {416,	125},
  {494,	125},
  {523,	250},
  {  0,	125},
  {330,	125},
  {659,	125},
  {623,	125},
  {659,	125},
  {623,	125},
  {659,	125},
  {494,	125},
  {587,	125},
  {523,	125},
  {440,	250},
  {  0,	125},
  {262,	125},
  {330,	125},
  {440,	125},
  {494,	250},
  {  0,	125},
  {330,	125},
  {523,	125},
  {494,	125},
  {440,	250},
  {  0,	125},
  {494,	125},
  {523,	125},
  {587,	125},
  {659,	375},
  {392,	125},
  {699,	125},
  {659,	125},
  {587,	375},
  {349,	125},
  {659,	125},
  {587,	125},
  {523,	375},
  {330,	125},
  {587,	125},
  {523,	125},
  {494,	250},
  {  0,	125},
  {330,	125},
  {659,	125},
  {  0,	250},
  {659,	125},
  {1319,	125},
  {  0,	250},
  {623,	125},
  {659,	125},
  {  0,	250},
  {623,	125},
  {659,	125},
  {623,	125},
  {659,	125},
  {623,	125},
  {659,	125},
  {494,	125},
  {587,	125},
  {523,	125},
  {440,	250},
  {  0,	125},
  {262,	125},
  {330,	125},
  {440,	125},
  {494,	250},
  {  0,	125},
  {330,	125},
  {416,	125},
  {494,	125},
  {523,	250},
  {  0,	125},
  {330,	125},
  {659,	125},
  {623,	125},
  {659,	125},
  {623,	125},
  {659,	125},
  {494,	125},
  {587,	125},
  {523,	125},
  {440,	250},
  {  0,	125},
  {262,	125},
  {330,	125},
  {440,	125},
  {494,	250},
  {  0,	125},
  {330,	125},
  {523,	125},
  {494,	125},
  {440,	500},
  {  0,	0}};

  int numNotes = sizeof(notes)/sizeof(notes[0]);
  for (int j = 0; j < numNotes; j++) {
        int pitch = notes[j][0];  // Get the pitch
        int delay = notes[j][1];  // Get the delay
        changeFreq(pitch, 50);
        delay_millis(TIM16, delay * 1000);
  }
  changeFreq(0, 50);
}*/

