////////////////////////////////////////////////////////////////////////////////
// Spring 2019, Capstone Design Class #41                                     //
// Straw Cleaner Firmware                                                     //
// Author: Susung Park (2014311254), School of Mechanical Engineering, SKKU   //
// Copyright (C) 2019-2019 Susung Park. All rights reserved.                  //
// This program requires two bipolar steppers, and Arduino Uno L293D shield.  //
// Standard libraries are copyrighted to respective authors.                  //
////////////////////////////////////////////////////////////////////////////////

#ifndef STRAW_CLEANER_H_
#define STRAW_CLEANER_H_

#include "./parameters.h"
#include <AFMotor.h>

#define AUTOMATIC   true
#define MANUAL      false

#define CAROUSEL_LOCKED     1
#define CAROUSEL_UNLOCKED   0

#define BRUSH_ADVANCED      1
#define BRUSH_RETREATED     0

class StrawCleaner
{
public: // Initializers
    StrawCleaner(AF_Stepper &, AF_Stepper &, int, int, float, bool);
    void TestMove();

public: // Indicator methods
    bool IsAutoMode();

public: // Brush motion methods
    void BrushMove(int);
    void BrushAdvance();
    void BrushRetreat();

public: // Carousel motion methods
    void CarouselMove(int);
    void CarouselProceed();
    void CarouselRetract();

public: // Interrupt-response methods
    void FlipOperatingMode();
    void StopMotion();

private: // Stepper motor instances
    AF_Stepper &brush_stepper_;
    AF_Stepper &carousel_stepper_;

private: // Mechanical properties
    const float screw_pitch_;
    const int brush_stepper_ppr_;
    const int carousel_stepper_ppr_;
    const float brush_operating_steps_;

private: // Steps definition of each motor.
    int carousel_steps_[4]; // each corresponds to 1-2, 2-3, 3-4, 4-1

private: // State variables
    bool operating_mode_;
    bool brush_stepper_operating_;
    bool carousel_stepper_operating_;
    bool report_serial_;
    int brush_stepper_pos_;
    int carousel_stepper_pos_;

private:
    int brush_phase_;
    int carousel_phase_;
};


// Default initializer.
StrawCleaner::StrawCleaner(AF_Stepper &brush_stepper, AF_Stepper &carousel_stepper,
                           int brush_stepper_ppr, int carousel_stepper_ppr,
                           float brush_operating_length, bool report_serial = false)
    : brush_stepper_(brush_stepper),
      carousel_stepper_(carousel_stepper),
      brush_stepper_operating_(false),
      carousel_stepper_operating_(false),
      report_serial_(report_serial),
      operating_mode_(AUTOMATIC),
      brush_stepper_pos_(0),
      carousel_stepper_pos_(0),
      brush_operating_steps_(12000)
{
    // None
}

void StrawCleaner::TestMove()
{
    brush_stepper_.step(1000, FORWARD, INTERLEAVE);
}


// Move the brush. pos is given in percent units.
void StrawCleaner::BrushMove(int pos_percent)
{
    // Convert desired position into steps units.
    int targetPos = brush_operating_steps_ * pos_percent / 100;
    int stepDiff = targetPos - brush_stepper_pos_;

    // Move the motor from current position to target postion.
    if (report_serial_)
    {
        Serial.print("Move to position ");
        Serial.print(pos_percent);
        Serial.print("%... ");
    }

    if (stepDiff > 0)
//        Serial.println(stepDiff);
        brush_stepper_.step(stepDiff, FORWARD, INTERLEAVE);
    else
//        Serial.println(stepDiff);
        brush_stepper_.step(-stepDiff, BACKWARD, INTERLEAVE);
    brush_stepper_pos_ += stepDiff;

    if (report_serial_)
    {
        Serial.println("finished!");
    }
}


// Advance the brush (call BrushMove method with parameter 100.0)
void StrawCleaner::BrushAdvance()
{
    BrushMove(100);
}


// Retreat the brush.
void StrawCleaner::BrushRetreat()
{
    BrushMove(0);
}


// Spin the carousel to forward direction.
void StrawCleaner::CarouselMove(int steps)
{
    if (report_serial_)
    {
        Serial.print("Move ");
        Serial.print(steps);
        Serial.print(" steps... ");
    }
    
    if (steps > 0)
        carousel_stepper_.step(steps, FORWARD, INTERLEAVE);
    else
        carousel_stepper_.step(-steps, BACKWARD, INTERLEAVE);

    if (report_serial_)
        Serial.println("finished!");
}


// Spin the carousel to next phase.
void StrawCleaner::CarouselProceed()
{
    if (report_serial_)
        Serial.print("Proceed carousel... ");
    
    if (carousel_phase_ == CAROUSEL_LOCKED)
    {
        CarouselMove(CAROUSEL_LOCKING);
        carousel_phase_ = CAROUSEL_UNLOCKED;
    }
    else if (carousel_phase_ == CAROUSEL_UNLOCKED)
    {
        CarouselMove(CAROUSEL_UNLOCKING);
        carousel_phase_ = CAROUSEL_LOCKED;
    }

    if (report_serial_)
        Serial.println("Finished!");
}


// Spin the carousel to previous phase.
void StrawCleaner::CarouselRetract()
{
    if (report_serial_)
        Serial.println("Retract carousel...");
    
    if (carousel_phase_ == CAROUSEL_LOCKED)
    {
        CarouselMove(-CAROUSEL_LOCKING);
        carousel_phase_ = CAROUSEL_UNLOCKED;
    }
    else if (carousel_phase_ == CAROUSEL_UNLOCKED)
    {
        CarouselMove(-CAROUSEL_UNLOCKING);
        carousel_phase_ = CAROUSEL_LOCKED;
    }
}


// Flip operating mode (autoamtic <=> manual)
void StrawCleaner::FlipOperatingMode()
{
    operating_mode_ = !operating_mode_;
}


// Get info about operating mode.
bool StrawCleaner::IsAutoMode()
{
    return operating_mode_;
}

#endif
