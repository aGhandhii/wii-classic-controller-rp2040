// Author: Alex Ghandhi
// Last Modified: 12 September 2023
//
// Header file for Classic Controller API
// Contains function definitions.


#ifndef CLASSICCONTROLLER_H_
#define CLASSICCONTROLLER_H_


// FUNCTION DECLARATIONS

// Initialize I2C connection, then decrypt and calibrate the controller
void ClassicController_init(i2c_inst_t *i2c, uint sda, uint scl, uint baudrate);

// Update the button and joystick values
void ClassicController_update(i2c_inst_t *i2c);

// Recalibrate the joysticks and analog triggers
void ClassicController_calibrate(i2c_inst_t *i2c);

// Print a readable report of the current button status
void ClassicController_button_report();

// Obtain button and joystick values from the controller
int ClassicController_joy_LX();
int ClassicController_joy_LY();
int ClassicController_joy_RX();
int ClassicController_joy_RY();
int ClassicController_LT_analog();
int ClassicController_RT_analog();
bool ClassicController_button_A();
bool ClassicController_button_B();
bool ClassicController_button_X();
bool ClassicController_button_Y();
bool ClassicController_button_UP();
bool ClassicController_button_DOWN();
bool ClassicController_button_LEFT();
bool ClassicController_button_RIGHT();
bool ClassicController_button_ZL();
bool ClassicController_button_ZR();
bool ClassicController_button_LT();
bool ClassicController_button_RT();
bool ClassicController_button_START();
bool ClassicController_button_SELECT();
bool ClassicController_button_HOME();

#endif  // C:CLASSICCONTROLLER_H_
