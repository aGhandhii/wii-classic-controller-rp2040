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

// Pin Definitions
static const uint8_t SDA = 8;
static const uint8_t SCL = 9;

// I2C Frequency
static const int BAUDRATE = 100000;

// Device I2C Addresses
static const uint8_t I2C_BUS_ADDR = 0x52;
static const uint8_t REGISTER_READ_ADDR = 0x00;

// Controller Decryption I2C Addresses and Write Values
static const uint8_t INIT_ADDR_0 = 0xF0;
static const uint8_t INIT_MSG_0 = 0x55;
static const uint8_t INIT_ADDR_1 = 0xFB;
static const uint8_t INIT_MSG_1 = 0x00;

// Set Device to Data Mode 3
static const uint8_t DATA_MODE_ADDR = 0xFE;
static const uint8_t DATA_MODE_MSG = 0x03;


///////////////
// VARIABLES //
///////////////

// Face Buttons
char BA, BB, BX, BY, BH, BSTART, BSELECT;

// Directional Buttons
char BDU, BDD, BDL, BDR;

// Triggers
char ZL, ZR, LT, RT;
uint8_t LT_ANALOG, RT_ANALOG;

// Joysticks
uint8_t LX, LY, RX, RY;


///////////////////////////
// FUNCTION DECLARATIONS //
///////////////////////////

void i2c_reg_write(i2c_inst_t *i2c, const uint addr, const uint8_t reg, uint8_t *buf, const uint8_t nbytes);
int i2c_reg_read(i2c_inst_t *i2c, const uint addr, const uint8_t reg, uint8_t *buf, const uint8_t nbytes);
void controller_init();
void update();
void print_button_report();


///////////////////////
// UTILITY FUNCTIONS //
///////////////////////

// Write byte(s) to the specified register
void i2c_reg_write(i2c_inst_t *i2c, const uint addr, const uint8_t reg, uint8_t *buf, const uint8_t nbytes) {
    // Create data packet
    uint8_t msg[nbytes + 1];
    // Append register address to start of data packet
    msg[0] = reg;
    // Append message bytes after register address
    for (int i = 0; i < nbytes; i++) {
        msg[i + 1] = buf[i];
    }
    printf("Writing 0x%02X to Register 0x%02X at Client Address 0x%02X\n", msg[1], msg[0], addr);
    // Write data to register
    i2c_write_blocking(i2c, addr, msg, (nbytes + 1), false);
}

// Read byte(s) from specified register address
int i2c_reg_read(i2c_inst_t *i2c, const uint addr, const uint8_t reg, uint8_t *buf, const uint8_t nbytes) {
    int num_bytes_read = 0;
    if (nbytes < 1) {
        return 0;
    }
    // Read data from register
    i2c_write_blocking(i2c, addr, &reg, 1, true);
    num_bytes_read = i2c_read_blocking(i2c, addr, buf, nbytes, false);
    if (num_bytes_read != nbytes) {
        printf("Failed to read correct number of bytes from register!\n");
        return 0;
    }
    return num_bytes_read;
}

// Initialize the controller
// Use the 'new' method from wiibrew, and set device data report to 0x03
void controller_init() {
    // Decrypt Controller
    printf("\n\n\nStarting Device Decryption\n");
    sleep_ms(10);
    i2c_reg_write(i2c0, I2C_BUS_ADDR, INIT_ADDR_0, &INIT_MSG_0, 1);
    sleep_ms(10);
    i2c_reg_write(i2c0, I2C_BUS_ADDR, INIT_ADDR_1, &INIT_MSG_1, 1);

    // Set controller to data report mode 0x03
    printf("Setting data report mode to 0x03\n");
    sleep_ms(10);
    i2c_reg_write(i2c0, I2C_BUS_ADDR, DATA_MODE_ADDR, &DATA_MODE_MSG, 1);
    printf("Data report mode set to 0x03\n");
    sleep_ms(10);
}

// Reads data from the controller and updates the button variables to reflect that data
void update() {
    // Array to store output
    uint8_t controller_out[8];
    
    // Read 8 bytes of data and return an array of those bytes
    if (i2c_reg_read(i2c0, I2C_BUS_ADDR, REGISTER_READ_ADDR, controller_out, 8) != 8) {
        printf("\nFailed to read button data from controller\n");
    }

    // Update button values to reflect data from controller_out
    LX          = controller_out[0];
    RX          = controller_out[1];
    LY          = controller_out[2];
    RY          = controller_out[3];
    LT_ANALOG   = controller_out[4];
    RT_ANALOG   = controller_out[5];
    BDR         = ((controller_out[6] >> 7) & 1);
    BDD         = ((controller_out[6] >> 6) & 1);
    LT          = ((controller_out[6] >> 5) & 1);
    BSELECT     = ((controller_out[6] >> 4) & 1);
    BH          = ((controller_out[6] >> 3) & 1);
    BSTART      = ((controller_out[6] >> 2) & 1);
    RT          = ((controller_out[6] >> 1) & 1);
    ZL          = ((controller_out[7] >> 7) & 1);
    BB          = ((controller_out[7] >> 6) & 1);
    BY          = ((controller_out[7] >> 5) & 1);
    BA          = ((controller_out[7] >> 4) & 1);
    BX          = ((controller_out[7] >> 3) & 1);
    ZR          = ((controller_out[7] >> 2) & 1);
    BDL         = ((controller_out[7] >> 1) & 1);
    BDU         = (controller_out[7] & 1);
}

// Print currently pressed buttons
void print_button_report() {
    printf("Button Report:\n");
    // Report Joystick Values
    printf("Left Joy:\tX: %d\tY: %d\nRight Joy:\tX: %d\tY: %d\n", LX, LY, RX, RY);
    // Report Trigger Values
    printf("LT: %d\tRT: %d\n", LT_ANALOG, RT_ANALOG);
    // Report all other buttons and trigger bottom-out values
    printf("Other Buttons:\n");
    if(BDU     == 0) printf("UP ");
    if(BDD     == 0) printf("DOWN ");
    if(BDL     == 0) printf("LEFT ");
    if(BDR     == 0) printf("RIGHT ");
    if(BA      == 0) printf("A ");
    if(BB      == 0) printf("B ");
    if(BX      == 0) printf("X ");
    if(BY      == 0) printf("Y ");
    if(BSTART  == 0) printf("START ");
    if(BSELECT == 0) printf("SELECT ");
    if(BH      == 0) printf("HOME ");
    if(ZL      == 0) printf("ZL ");
    if(ZR      == 0) printf("ZR ");
    if(LT      == 0) printf("LT ");
    if(RT      == 0) printf("RT ");
    printf("\n");
}


///////////////////
// MAIN FUNCTION //
///////////////////
int main() {

    // Initialize i2c pins
    int actual_baudrate = i2c_init(i2c0, BAUDRATE);

    // Initialize serial
    stdio_init_all();

    gpio_init(25);  // LED indicator
    gpio_init(SDA);
    gpio_init(SCL);
    gpio_set_function(SDA, GPIO_FUNC_I2C);
    gpio_set_function(SCL, GPIO_FUNC_I2C);
    gpio_pull_up(SDA);
    gpio_pull_up(SCL);
    gpio_set_dir(25, GPIO_OUT);

    // Blink slowly until usb connection is established
    while (!stdio_usb_connected()) {
        gpio_put(25, 1);
        sleep_ms(500);
        gpio_put(25, 0);
        sleep_ms(500);
    }
    printf("Connected to USB\n");
    printf("Set Baudrate: %d", actual_baudrate);
    sleep_ms(1000);

    // Wait for device initialization
    controller_init();

    // Main logic loop:
    while (true) {
        gpio_put(25, 1);
        sleep_ms(50);

        // Read controller data
        update();

        // Print debug
        print_button_report();

        // Wait for a short time
        gpio_put(25, 0);
        sleep_ms(50);
    }
}
