////////////////////////////////////////////////////////////////////////////////
// Spring 2019, Capstone Design Class #41                                     //
// Straw Cleaner Firmware                                                     //
// Author: Susung Park (2014311254), School of Mechanical Engineering, SKKU   //
// Copyright (C) 2019-2019 Susung Park. All rights reserved.                  //
// This program requires two bipolar steppers, and Arduino Uno L293D shield.  //
// Standard libraries are copyrighted to respective authors.                  //
////////////////////////////////////////////////////////////////////////////////

#include <AFMotor.h>
#include "./parameters.h"
#include "./straw_cleaner.h"

// Initialize stepper motors.
AF_Stepper BrushStepper(1, SPR_BRUSH);
AF_Stepper CarouselStepper(2, SPR_CAROUSEL);


// Initialize StrawCleaner instance.
StrawCleaner strawCleaner(BrushStepper, CarouselStepper,
                          PPR_BRUSH, PPR_CAROUSEL, BRUSH_OPERATING_LENGTH);


// State variables.
#define AUTOMATIC   true
#define MANUAL      false

bool operatingMode = AUTOMATIC;



// Serial communications parameters.
bool rxActive = false;
char serialBuf[100];
char charBuf;
char msgLen = 0;


// Button struct and instances.
struct Button
{
    int pin;
    bool state;
};

Button operating_mode = { BUTTON_OPERATING_MODE, false };
Button carousel_spin = { BUTTON_CAROUSEL_SPIN, false };
Button brush_advance = { BUTTON_BRUSH_ADVANCE, false };
Button brush_retreat = { BUTTON_BRUSH_RETREAT, false };


// Regular methods.
void autoOperate();


// Interrupt methods.
void checkButton();
void checkAllButtons();
void flipOperatingMode(StrawCleaner&);


// Setup function
void setup()
{
    // Attach interrupts.
    attachInterrupt(digitalPinToInterrupt(BUTTON_INTERRUPT_PIN), checkAllButtons, RISING);

    // Serial should be replaced with Bluetooth code.
    Serial.begin(9600);
}


// Loop function
void loop()
{
    // Receive serial messages if available.
    if (Serial.available())
    {
        if (!rxActive)
        {
            msgLen = 0;
            rxActive = true;
        }

        charBuf = Serial.read();
        if (charBuf == '\n')
        {
            serialBuf[msgLen] = '\0';
            rxActive = false;
            // Add code to process command.
        }
        else
        {
            serialBuf[msgLen++] = charBuf;
        }
    }

    
}


// Check button state.
void checkButton(Button &button)
{
    button.state = digitalRead(button.pin);
}


// Check states of all buttons.
void checkAllButtons()
{
    checkButton(operating_mode);
    checkButton(carousel_spin);
    checkButton(brush_advance);
    checkButton(brush_retreat);
}


// Flip operating mode of straw cleaner.
void flipOperatingMode(StrawCleaner &device)
{
    device.FlipOperatingMode();
}


// Automatically operate the straw cleaner.
void autoOperate()
{
    
}
