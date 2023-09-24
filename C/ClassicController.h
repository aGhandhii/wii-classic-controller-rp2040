// Author: Alex Ghandhi
// Last Modified: 24 September 2023
//
// Header file for Classic Controller API
// Contains controller type and function definitions.


#ifndef CLASSICCONTROLLER_H_
#define CLASSICCONTROLLER_H_

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

// Controller Type Declaration
typedef struct {
    // Buttons
    uint8_t LX;
    uint8_t LY;
    uint8_t RX;
    uint8_t RY;
    uint8_t LT_ANALOG;
    uint8_t RT_ANALOG;
    bool A;
    bool B;
    bool X;
    bool Y;
    bool HOME;
    bool START;
    bool SELECT;
    bool UP;
    bool DOWN;
    bool LEFT;
    bool RIGHT;
    bool ZL;
    bool ZR;
    bool LT;
    bool RT;
    // I2C Instance
    i2c_inst_t* i2c;
} ClassicController;

// FUNCTION DECLARATIONS

// Initialize I2C connection, then decrypt and calibrate the controller
ClassicController ClassicController_init(i2c_inst_t *i2c, uint sda, uint scl, uint baudrate);

// Update the button and joystick values
void ClassicController_update(ClassicController controller);

// Recalibrate the joysticks and analog triggers
void ClassicController_calibrate(ClassicController controller);

// Print a readable report of the current button status
void ClassicController_button_report(ClassicController controller);


#endif  // C:CLASSICCONTROLLER_H_
