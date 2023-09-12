// Author: Alex Ghandhi
// Last Modified: 11 September 2023

/*
This code reads I2C input from a Wii Classic Controller.

Before data can be read, the device must first be decrypted.
This is accomplished with the 'new' method at the wiibrew wiki: http://wiibrew.org/wiki/Wiimote/Extension_Controllers
    write 0x55 to 0xF0
    write 0x00 to 0xFB

Set device into data format 0x03: http://wiibrew.org/wiki/Wiimote/Extension_Controllers/Classic_Controller
    write 0x03 to 0xFE

Controller Client I2C info:
    I2C bus address: 0x52
*/


#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"


///////////////
// CONSTANTS //
///////////////

// Device I2C Addresses
static const uint I2C_BUS_ADDR = 0x52;
static const uint8_t REGISTER_READ_ADDR = 0x00;

// Controller Decryption I2C Addresses and Write Values
static const uint8_t INIT_MSG_0[] = {0xF0, 0x55};
static const uint8_t INIT_MSG_1[] = {0xFB, 0x00};

// Set Device to Data Mode 3
static uint8_t DATA_MODE_MSG[] = {0xFE, 0x03};

//////////////////////
// GLOBAL VARIABLES //
//////////////////////

// Calibrated values for joysticks and triggers
static uint8_t LX_center, LY_center, RX_center, RY_center, LT_init, RT_init;

// Face Buttons
static bool BA, BB, BX, BY, BH, BSTART, BSELECT;
// Directional Buttons
static bool BDU, BDD, BDL, BDR;
// Triggers
static bool ZL, ZR, LT, RT;
static uint8_t LT_ANALOG, RT_ANALOG;
// Joysticks
static uint8_t LX, LY, RX, RY;


///////////////////////////////////
// BUTTON GETTER IMPLEMENTATIONS //
///////////////////////////////////

int ClassicController_joy_LX() {
    return (int)LX - LX_center;
}

int ClassicController_joy_LY() {
    return (int)LY - LY_center;
}

int ClassicController_joy_RX() {
    return (int)RX - RX_center;
}

int ClassicController_joy_RY() {
    return (int)RY - RY_center;
}

int ClassicController_LT_analog() {
    return (LT_init > LT_ANALOG) ? 0 : (int)(LT_ANALOG - LT_init);
}

int ClassicController_RT_analog() {
    return (RT_init > RT_ANALOG) ? 0 : (int)(RT_ANALOG - RT_init);
}

bool ClassicController_button_A() {
    return BA;
}

bool ClassicController_button_B() {
    return BB;
}

bool ClassicController_button_X() {
    return BX;
}

bool ClassicController_button_Y() {
    return BY;
}

bool ClassicController_button_UP() {
    return BDU;
}

bool ClassicController_button_DOWN() {
    return BDD;
}

bool ClassicController_button_LEFT() {
    return BDL;
}

bool ClassicController_button_RIGHT() {
    return BDR;
}

bool ClassicController_button_ZL() {
    return ZL;
}

bool ClassicController_button_ZR() {
    return ZR;
}

bool ClassicController_button_LT() {
    return LT;
}

bool ClassicController_button_RT() {
    return RT;
}

bool ClassicController_button_START() {
    return BSTART;
}

bool ClassicController_button_SELECT() {
    return BSELECT;
}

bool ClassicController_button_HOME() {
    return BH;
}


//////////////////////////////
// FUNCTION IMPLEMENTATIONS //
//////////////////////////////

// Reads data from the controller and updates the button variables to reflect that data
void ClassicController_update(i2c_inst_t *i2c) {
    // Array to store output
    uint8_t controller_out[8];

    // Read 8 bytes of data and return an array of those bytes
    i2c_write_blocking(i2c, I2C_BUS_ADDR, &REGISTER_READ_ADDR, 1, false);
    sleep_us(500);
    if (i2c_read_blocking(i2c, I2C_BUS_ADDR, controller_out, 8, false) != 8) {
        printf("\nFailed to read button data from controller\n");
    }

    // Update button values to reflect data from controller_out
    LX          = controller_out[0];
    RX          = controller_out[1];
    LY          = controller_out[2];
    RY          = controller_out[3];
    LT_ANALOG   = controller_out[4];
    RT_ANALOG   = controller_out[5];
    BDR         = (~(controller_out[6] >> 7) & 1);
    BDD         = (~(controller_out[6] >> 6) & 1);
    LT          = (~(controller_out[6] >> 5) & 1);
    BSELECT     = (~(controller_out[6] >> 4) & 1);
    BH          = (~(controller_out[6] >> 3) & 1);
    BSTART      = (~(controller_out[6] >> 2) & 1);
    RT          = (~(controller_out[6] >> 1) & 1);
    ZL          = (~(controller_out[7] >> 7) & 1);
    BB          = (~(controller_out[7] >> 6) & 1);
    BY          = (~(controller_out[7] >> 5) & 1);
    BA          = (~(controller_out[7] >> 4) & 1);
    BX          = (~(controller_out[7] >> 3) & 1);
    ZR          = (~(controller_out[7] >> 2) & 1);
    BDL         = (~(controller_out[7] >> 1) & 1);
    BDU         = (~controller_out[7] & 1);
}

// Calibrates the joysticks and analog triggers
void ClassicController_calibrate(i2c_inst_t *i2c) {
    // Obtain controller status
    ClassicController_update(i2c);

    // Update calibration variables
    LX_center = LX;
    LY_center = LY;
    RX_center = RX;
    RY_center = RY;
    LT_init = LT;
    RT_init = RT;
}

// Initialize the controller
void ClassicController_init(i2c_inst_t *i2c, uint SDA, uint SCL, uint BAUDRATE) {
    // Initialize I2C functionality
    i2c_init(i2c, BAUDRATE);
    gpio_set_function(SDA, GPIO_FUNC_I2C);
    gpio_set_function(SCL, GPIO_FUNC_I2C);
    gpio_pull_up(SDA);
    gpio_pull_up(SCL);

    // Decrypt Controller
    sleep_ms(1);
    i2c_write_blocking(i2c, I2C_BUS_ADDR, INIT_MSG_0, 2, false);
    sleep_ms(1);
    i2c_write_blocking(i2c, I2C_BUS_ADDR, INIT_MSG_1, 2, false);
    sleep_ms(1);

    // Set controller to data report mode 0x03
    i2c_write_blocking(i2c, I2C_BUS_ADDR, DATA_MODE_MSG, 2, false);
    sleep_ms(10);

    // Calibrate the joysticks and triggers
    ClassicController_calibrate(i2c);
}

// Print currently pressed buttons
void ClassicController_button_report() {
    printf("\e[1;1H\e[2J");  // Clear the console window
    printf("Button Report:\n");
    // Report Joystick Values
    printf("Left Joy:\tX: %d\tY: %d\nRight Joy:\tX: %d\tY: %d\n",
            ClassicController_joy_LX(),
            ClassicController_joy_LY(),
            ClassicController_joy_RX(),
            ClassicController_joy_RY()
    );
    // Report Trigger Values
    printf("Left Trigger: %d\nRight Trigger: %d\n",
            ClassicController_LT_analog(),
            ClassicController_RT_analog()
    );
    // Report all other buttons and trigger bottom-out values
    printf("Other Buttons:\n");
    if(ClassicController_button_UP()) printf("UP ");
    if(ClassicController_button_DOWN()) printf("DOWN ");
    if(ClassicController_button_LEFT()) printf("LEFT ");
    if(ClassicController_button_RIGHT()) printf("RIGHT ");
    if(ClassicController_button_A()) printf("A ");
    if(ClassicController_button_B()) printf("B ");
    if(ClassicController_button_X()) printf("X ");
    if(ClassicController_button_Y()) printf("Y ");
    if(ClassicController_button_START()) printf("START ");
    if(ClassicController_button_SELECT()) printf("SELECT ");
    if(ClassicController_button_HOME()) printf("HOME ");
    if(ClassicController_button_ZL()) printf("ZL ");
    if(ClassicController_button_ZR()) printf("ZR ");
    if(ClassicController_button_LT()) printf("LT ");
    if(ClassicController_button_RT()) printf("RT ");
}