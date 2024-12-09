//Luke Summers lsummers@g.hmc.edu
//Header file for light function and constants

#ifndef LIGHTS_H
#define LIGHTS_H

#include <stdint.h>

#define LED_SDI PB0
#define LED_CLK PA9
#define LED_STARTUP 0x00000000
#define LED_END 0xFFFFFFFF
#define LED_OFF 0x00000007
#define LED_RED 0xFF0000FF
#define LED_GREEN 0x00FF00FF
#define LED_BLUE 0x0000FFFF
#define LED_PURPLE 0xF0F0F0FF
#define LED_ORANGE 0x00F0F0FF

void lightOneLed(uint32_t color);

#endif