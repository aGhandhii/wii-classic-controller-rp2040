# Author: Alex Ghandhi
# Last Modified: 10 September 2023

"""
This code reads I2C input from a Wii Classic Controller.

Before data can be read, the device must first be decrypted.
This is accomplished with the 'new' method at the wiibrew wiki: http://wiibrew.org/wiki/Wiimote/Extension_Controllers
    write 0x55 to 0xF0
    write 0x00 to 0xFB

Set device into data format 0x03: http://wiibrew.org/wiki/Wiimote/Extension_Controllers/Classic_Controller
    write 0x03 to 0xFE

Controller Client I2C info:
    I2C bus address: 0x52
"""

from machine import Pin, I2C
from time import sleep_ms


class ClassicController(object):
    # I2C Client Address
    bus_addr = 0x52

    # The buffer stores the controller output
    buffer = bytearray(8)

    ##################
    # DUNDER METHODS #
    ##################

    def __init__(self, sda: int, scl: int, freq: int) -> None:
        """ Initialize ClassicController Object.

        Args
            sda:  SDA pin
            scl:  SCL pin
            freq: frequency for I2C device
        """
        # Create I2C client
        self.i2c = I2C(id=0, sda=Pin(sda), scl=Pin(scl), freq=freq)

        # Decrypt Controller and set Data Mode 3
        self.decrypt_controller()
        self.set_data_mode_3()

        # Calibrate Joysticks and Triggers
        self.calibrate_joysticks()
        self.calibrate_triggers()

    def __str__(self):
        # Return string representation of current button status

    #####################
    # UTILITY FUNCTIONS #
    #####################

    def decrypt_controller(self) -> None:
        # Decrypt the controller
        # write 0x55 to 0xF0 and 0x00 to 0xFB
        self.i2c.writeto(addr=self.bus_addr, buf=b'\xF0\x55')
        sleep_ms(10)
        self.i2c.writeto(addr=self.bus_addr, buf=b'\xFB\x00')
        sleep_ms(10)

    def set_data_mode_3(self) -> None:
        # Set controller to data mode 3
        # write 0x03 to 0xFE
        self.i2c.writeto(addr=self.bus_addr, buf=b'\xFE\x03')
        sleep_ms(10)

    def update(self) -> None:
        # Read data from controller and update the output buffer
        # From wiibrew.org: Data is reported in 8 bytes at 0x08 when decrypted
        self.i2c.writeto(addr=self.bus_addr, buf=b'\x08')
        self.i2c.readfrom_into(addr=self.bus_addr, buf=self.buffer)

    def calibrate_joysticks(self) -> None:
        # Calibrate joysticks (store initial value as a 'center' value)
        self.update()
        self.joy_LX_center = self.joy_LX()
        self.joy_LY_center = self.joy_LY()
        self.joy_RX_center = self.joy_RX()
        self.joy_RY_center = self.joy_RY()

    def calibrate_triggers(self) -> None:
        # Calibrate analogue triggers (same method as joysticks)
        self.update()
        self.trigger_L_initial = self.trigger_L()
        self.trigger_R_initial = self.trigger_R()

    ##################
    # BUTTON GETTERS #
    ##################

    def button_A(self):
        # Get value of button A

    def button_B(self):
        # Get value of button B

    def button_X(self):
        # Get value of button X

    def button_Y(self):
        # Get value of button Y

    def button_HOME(self):
        # Get value of button HOME

    def button_START(self):
        # Get value of button START

    def button_SELECT(self):
        # Get value of button SELECT

    def button_UP(self):
        # Get value of button DPAD UP

    def button_DOWN(self):
        # Get value of button DPAD DOWN

    def button_LEFT(self):
        # Get value of button DPAD LEFT

    def button_RIGHT(self):
        # Get value of button DPAD RIGHT

    def button_ZL(self):
        # Get value of button ZL

    def button_ZR(self):
        # Get value of button ZR

    def trigger_L(self):
        # Get value of Left trigger
        return int(self.buffer[4])

    def trigger_R(self):
        # Get value of Right trigger
        return int(self.buffer[5])

    def joy_LX(self):
        # Get value of Left joystick x-axis
        return int(self.buffer[0])

    def joy_LY(self):
        # Get value of Left joystick y-axis
        return int(self.buffer[2])

    def joy_RX(self):
        # Get value of Right joystick x-axis
        return int(self.buffer[1])

    def joy_RY(self):
        # Get value of Right joystick y-axis
        return int(self.buffer[3])