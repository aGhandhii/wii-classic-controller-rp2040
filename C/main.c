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
#include "pico/binary_info.h"
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

// Face Buttons
uint8_t BA, BB, BX, BY, BH, BSTART, BSELECT;
// Directional Buttons
uint8_t BDU, BDD, BDL, BDR;
// Triggers
uint8_t ZL, ZR, LT, RT;
uint8_t LT_ANALOG, RT_ANALOG;
// Joysticks
uint8_t LX, LY, RX, RY;


///////////////////////////
// FUNCTION DECLARATIONS //
///////////////////////////

void ClassicController_init(i2c_inst_t *i2c, uint SDA, uint SCL, uint BAUDRATE);
void ClassicController_update(i2c_inst_t *i2c);
void ClassicController_button_report();


//////////////////////////////
// FUNCTION IMPLEMENTATIONS //
//////////////////////////////

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
    sleep_ms(1);
}

// Reads data from the controller and updates the button variables to reflect that data
void ClassicController_update(i2c_inst_t *i2c) {
    // Array to store output
    uint8_t controller_out[8];

    // Read 8 bytes of data and return an array of those bytes
    i2c_write_blocking(i2c, I2C_BUS_ADDR, &REGISTER_READ_ADDR, 1, false);
    sleep_ms(1);
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

// Print currently pressed buttons
void ClassicController_button_report() {
    printf("\e[1;1H\e[2J");
    printf("Button Report:\n");
    // Report Joystick Values
    printf("Left Joy:\tX: %d\tY: %d\nRight Joy:\tX: %d\tY: %d\n", LX, LY, RX, RY);
    // Report Trigger Values
    printf("LT: %d\tRT: %d\n", LT_ANALOG, RT_ANALOG);
    // Report all other buttons and trigger bottom-out values
    printf("Other Buttons:\n");
    if(BDU) printf("UP ");
    if(BDD) printf("DOWN ");
    if(BDL) printf("LEFT ");
    if(BDR) printf("RIGHT ");
    if(BA) printf("A ");
    if(BB) printf("B ");
    if(BX) printf("X ");
    if(BY) printf("Y ");
    if(BSTART) printf("START ");
    if(BSELECT) printf("SELECT ");
    if(BH) printf("HOME ");
    if(ZL) printf("ZL ");
    if(ZR) printf("ZR ");
    if(LT) printf("LT ");
    if(RT) printf("RT ");
}


///////////////////
// MAIN FUNCTION //
///////////////////
int main() {
    // I2C instance
    i2c_inst_t *i2c = i2c1;

    // Initialize IO
    stdio_init_all();
    
    // Indicator LED
    gpio_init(25);
    gpio_set_dir(25, GPIO_OUT);

    // Blink slowly until usb connection is established
    while (!stdio_usb_connected()) {
        gpio_put(25, 1);
        sleep_ms(500);
        gpio_put(25, 0);
        sleep_ms(500);
    }
    printf("Connected to USB\n");
    sleep_ms(100);

    // Wait for device initialization
    ClassicController_init(i2c, 6, 7, 100000);

    // Main logic loop:
    while (true) {
        gpio_put(25, 1);
        sleep_ms(50);

        // Obtain controller data
        ClassicController_update(i2c);
        ClassicController_button_report();

        // Wait for a short time
        gpio_put(25, 0);
        sleep_ms(50);
    }
}
