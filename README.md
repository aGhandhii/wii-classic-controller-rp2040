# wii-classic-controller-rp2040
Wii Classic Controller compatibility layer for Raspberry Pi Pico

Relies on [Wiibrew](https://wiibrew.org/wiki/Wiimote/Extension_Controllers/Classic_Controller) for specifics on the I2C interface.

SDA and SCL pins require pullup resistors to 3.3V, I used 5kOhm for each line.
