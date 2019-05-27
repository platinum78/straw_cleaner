#ifndef PINS_DEFINITION_H_
#define PINS_DEFINITION_H_

// Button pins definition
#define BUTTON_INTERRUPT_PIN    2
#define BUTTON_OPERATING_MODE   8
#define BUTTON_CAROUSEL_SPIN    9
#define BUTTON_BRUSH_ADVANCE    10
#define BUTTON_BRUSH_RETREAT    11

// Steps per revolution of each stepper
#define SPR_BRUSH      200
#define SPR_CAROUSEL   200

// Pulse per revolution of each stepper
#define PPR_BRUSH      200
#define PPR_CAROUSEL   200

// Operating length of brush (in mm)
#define BRUSH_OPERATING_LENGTH          250

// Maximum speed of each stepper
#define BRUSH_MAX_RPM       300
#define CAROUSEL_MAX_RPM    30

// Carousel stepper positions
#define CAROUSEL_LOCKING    160
#define CAROUSEL_UNLOCKING  240

#endif
