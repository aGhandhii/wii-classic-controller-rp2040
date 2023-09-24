// Author: Alex Ghandhi
// Last Modified: 24 September 2023

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
#include "ClassicController.h"
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


//////////////////////////////
// FUNCTION IMPLEMENTATIONS //
//////////////////////////////

// Reads data from the controller and updates the button variables to reflect that data
void ClassicController_update(ClassicController controller) {
    // Array to store output
    uint8_t controller_out[8];

    // Read 8 bytes of data and return an array of those bytes
    i2c_write_blocking(controller.i2c, I2C_BUS_ADDR, &REGISTER_READ_ADDR, 1, false);
    sleep_us(200);
    if (i2c_read_blocking(controller.i2c, I2C_BUS_ADDR, controller_out, 8, false) != 8) {
        printf("\nFailed to read button data from controller\n");
    }

    // Update button values to reflect data from controller_out
    controller.LX          = controller_out[0];
    controller.RX          = controller_out[1];
    controller.LY          = controller_out[2];
    controller.RY          = controller_out[3];
    controller.LT_ANALOG   = controller_out[4];
    controller.RT_ANALOG   = controller_out[5];
    controller.RIGHT       = (~(controller_out[6] >> 7) & 1);
    controller.DOWN        = (~(controller_out[6] >> 6) & 1);
    controller.LT          = (~(controller_out[6] >> 5) & 1);
    controller.SELECT      = (~(controller_out[6] >> 4) & 1);
    controller.HOME        = (~(controller_out[6] >> 3) & 1);
    controller.START       = (~(controller_out[6] >> 2) & 1);
    controller.RT          = (~(controller_out[6] >> 1) & 1);
    controller.ZL          = (~(controller_out[7] >> 7) & 1);
    controller.B           = (~(controller_out[7] >> 6) & 1);
    controller.Y           = (~(controller_out[7] >> 5) & 1);
    controller.A           = (~(controller_out[7] >> 4) & 1);
    controller.X           = (~(controller_out[7] >> 3) & 1);
    controller.ZR          = (~(controller_out[7] >> 2) & 1);
    controller.LEFT        = (~(controller_out[7] >> 1) & 1);
    controller.UP          = (~controller_out[7] & 1);
}

// Calibrates the joysticks and analog triggers
void ClassicController_calibrate(ClassicController controller) {
    // Obtain controller status
    ClassicController_update(controller);

    // Update calibration variables
    LX_center = controller.LX;
    LY_center = controller.LY;
    RX_center = controller.RX;
    RY_center = controller.RY;
    LT_init   = controller.LT;
    RT_init   = controller.RT;
}

// Initialize the controller
ClassicController ClassicController_init(i2c_inst_t *i2c, uint sda, uint scl, uint baudrate) {
    // Initialize I2C functionality
    i2c_init(i2c, baudrate);
    gpio_set_function(sda, GPIO_FUNC_I2C);
    gpio_set_function(scl, GPIO_FUNC_I2C);
    gpio_pull_up(sda);
    gpio_pull_up(scl);

    // Decrypt Controller
    sleep_ms(1);
    i2c_write_blocking(i2c, I2C_BUS_ADDR, INIT_MSG_0, 2, false);
    sleep_ms(1);
    i2c_write_blocking(i2c, I2C_BUS_ADDR, INIT_MSG_1, 2, false);
    sleep_ms(1);

    // Set controller to data report mode 0x03
    i2c_write_blocking(i2c, I2C_BUS_ADDR, DATA_MODE_MSG, 2, false);
    sleep_ms(1);

    // Create a Classic_Controller instance
    ClassicController controller;
    controller.i2c = i2c;

    // Calibrate the joysticks and triggers
    ClassicController_calibrate(controller);
    return controller;
}

// Print currently pressed buttons
void ClassicController_button_report(ClassicController controller) {
    printf("\e[1;1H\e[2J");  // Clear the console window
    printf("Button Report:\n");
    // Report Joystick Values
    printf("Left Joy:\tX: %d\tY: %d\nRight Joy:\tX: %d\tY: %d\n",
            controller.LX,
            controller.LY,
            controller.RX,
            controller.RY
    );
    // Report Trigger Values
    printf("Left Trigger: %d\nRight Trigger: %d\n",
            controller.LT_ANALOG,
            controller.RT_ANALOG
    );
    // Report all other buttons and trigger bottom-out values
    printf("Other Buttons:\n");
    if(controller.UP) printf("UP ");
    if(controller.DOWN) printf("DOWN ");
    if(controller.LEFT) printf("LEFT ");
    if(controller.RIGHT) printf("RIGHT ");
    if(controller.A) printf("A ");
    if(controller.B) printf("B ");
    if(controller.X) printf("X ");
    if(controller.Y) printf("Y ");
    if(controller.START) printf("START ");
    if(controller.SELECT) printf("SELECT ");
    if(controller.HOME) printf("HOME ");
    if(controller.ZL) printf("ZL ");
    if(controller.ZR) printf("ZR ");
    if(controller.LT) printf("LT ");
    if(controller.RT) printf("RT ");
}
