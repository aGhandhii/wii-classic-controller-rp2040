// Author: Alex Ghandhi
// Last Modified: 24 September 2023


#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#include "ClassicController.h"


///////////////////
// MAIN FUNCTION //
///////////////////
int main() {
    // I2C and controller instance
    i2c_inst_t *i2c = i2c1;
    ClassicController controller;

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
    controller = ClassicController_init(i2c, 6, 7, 100000);

    // Main logic loop:
    while (true) {
        gpio_put(25, 1);
        sleep_us(100);

        // Obtain controller data
        ClassicController_update(controller);
        ClassicController_button_report(controller);

        // Wait for a short time
        gpio_put(25, 0);
        sleep_us(100);
    }
}
