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
from time import sleep


class ClassicController(object):
    # Use bus address for the controller client
    bus_addr = 0x52

    # The buffer stores the controller output
    buffer = bytearray(8)

    ##################
    # DUNDER METHODS #
    ##################

    def __init__(self, sda, scl, freq) -> None:
        """ Initialize ClassicController Object.

        Decrypts the controller, sets the data to mode 3, and calibrates the controller.

        Args
            sda:  SDA pin
            scl:  SCL pin
            freq: frequency for I2C device
        """
        # Decrypt Controller
        # Set to data mode 3
        # Calibrate Joysticks and Triggers

    def __str__(self):
        # Return string representation of current button status

    #####################
    # UTILITY FUNCTIONS #
    #####################

    def decrypt_controller(self) -> bool:
        # Decrypt the controller

    def set_data_mode_3(self) -> bool:
        # Set controller to data mode 3

    def update(self) -> None:
        # Read data from controller and update the output buffer

    def calibrate_joysticks(self) -> bool:
        # Calibrate joysticks (store initial value as a 'center' value)

    def calibrate_triggers(self) -> bool:
        # Calibrate analogue triggers (same method as joysticks)

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

    def trigger_R(self):
        # Get value of Right trigger

    def joy_L(self):
        # Get value of Left joystick

    def joy_R(self):
        # Get value of Right joystick
