// Author: Alex Ghandhi
// Last Modified: 11 September 2023


#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#include "ClassicController.h"


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
        sleep_ms(1);

        // Obtain controller data
        ClassicController_update(i2c);
        ClassicController_button_report();

        // Wait for a short time
        gpio_put(25, 0);
        sleep_ms(10);
    }
}
