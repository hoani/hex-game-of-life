#ifndef _HGOL_LED_GRID_H_
#define _HGOL_LED_GRID_H_

#include "grid.h"

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define GRID_PIN 6

#endif