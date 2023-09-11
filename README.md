# wii-classic-controller-rp2040
Wii Classic Controller compatibility layer for Raspberry Pi Pico

Relies on [Wiibrew](https://wiibrew.org/wiki/Wiimote/Extension_Controllers/Classic_Controller) for specifics on the Classic Controller I2C interface.

Credits to [This Digikey Article](https://www.digikey.com/en/maker/projects/raspberry-pi-pico-rp2040-i2c-example-with-micropython-and-cc/47d0c922b79342779cdbd4b37b7eb7e2) and [PicoNunchuck](https://github.com/MichaelBell/PicoNunchuck) by MichaelBell.

SDA and SCL pins require pullup resistors to 3.3V, I used 5kOhm for each line.
