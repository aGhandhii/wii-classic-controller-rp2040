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
    bus_addr: int = 0x52

    # The buffer stores the controller output
    buffer = bytearray(8)

    ##################
    # DUNDER METHODS #
    ##################

    def __init__(self, id: int, sda: int, scl: int, freq: int) -> None:
        """ Initialize ClassicController Object.

        Args
            id:   i2c ID
            sda:  SDA pin
            scl:  SCL pin
            freq: frequency for I2C device
        """
        # Create I2C client
        self.i2c = I2C(id=id, sda=Pin(sda), scl=Pin(scl), freq=freq)
        sleep_ms(10)
        print("Performing bus scan")
        print(self.i2c.scan())
        print("Bus scan complete")

        # Decrypt Controller and set Data Mode 3
        print("Decrypting Controller")
        self.decrypt_controller()
        print("Setting Data Mode 3")
        self.set_data_mode_3()

        # Calibrate Joysticks and Triggers
        print("Calibrating")
        self.calibrate_joysticks()
        self.calibrate_triggers()
        print("Calibration Complete")

    def __str__(self):
        self.update()
        print(f"Left Joy X: {self.joy_LX()}")

    #####################
    # UTILITY FUNCTIONS #
    #####################

    def decrypt_controller(self) -> None:
        print("writing 0x55 to 0xF0")
        self.i2c.writeto(self.bus_addr, b'\xF0\x55')
        sleep_ms(50)
        print("writing 0x00 to 0xFB")
        self.i2c.writeto(self.bus_addr, b'\xFB\x00')
        sleep_ms(50)

    def set_data_mode_3(self) -> None:
        print("writing 0x03 to 0xFE")
        self.i2c.writeto(self.bus_addr, b'\xFE\x03')
        sleep_ms(50)

    def update(self) -> None:
        # Read data from controller and update the output buffer
        # From wiibrew.org: Data is reported in 8 bytes at 0x08 when decrypted
        print("Requesting data from bus")
        self.i2c.writeto(self.bus_addr, b'\x08')
        print("Reading button data")
        self.i2c.readfrom_into(self.bus_addr, self.buffer)
        print("Done updating")
        sleep_ms(50)

    def calibrate_joysticks(self) -> None:
        # Find center values for joysticks
        self.update()
        self.joy_LX_center = self.joy_LX()
        self.joy_LY_center = self.joy_LY()
        self.joy_RX_center = self.joy_RX()
        self.joy_RY_center = self.joy_RY()

    def calibrate_triggers(self) -> None:
        # Find initial values for the analogue triggers
        self.update()
        self.trigger_L_initial = self.trigger_L()
        self.trigger_R_initial = self.trigger_R()

    ##################
    # BUTTON GETTERS #
    ##################

    def button_A(self) -> int:
        # Get value of button A
        return int((self.buffer[7] >> 4) & 1)

    def button_B(self) -> int:
        # Get value of button B
        return int((self.buffer[7] >> 6) & 1)

    def button_X(self) -> int:
        # Get value of button X
        return int((self.buffer[7] >> 3) & 1)

    def button_Y(self) -> int:
        # Get value of button Y
        return int((self.buffer[7] >> 5) & 1)

    def button_HOME(self) -> int:
        # Get value of button HOME
        return int((self.buffer[6] >> 3) & 1)

    def button_START(self) -> int:
        # Get value of button START
        return int((self.buffer[6] >> 2) & 1)

    def button_SELECT(self) -> int:
        # Get value of button SELECT
        return int((self.buffer[6] >> 4) & 1)

    def button_UP(self) -> int:
        # Get value of button DPAD UP
        return int(self.buffer[7] & 1)

    def button_DOWN(self) -> int:
        # Get value of button DPAD DOWN
        return int((self.buffer[6] >> 6) & 1)

    def button_LEFT(self) -> int:
        # Get value of button DPAD LEFT
        return int((self.buffer[7] >> 1) & 1)

    def button_RIGHT(self) -> int:
        # Get value of button DPAD RIGHT
        return int((self.buffer[6] >> 7) & 1)

    def button_ZL(self) -> int:
        # Get value of button ZL
        return int((self.buffer[7] >> 7) & 1)

    def button_ZR(self) -> int:
        # Get value of button ZR
        return int((self.buffer[7] >> 2) & 1)

    def trigger_L(self) -> int:
        # Get value of Left trigger
        return int(self.buffer[4])

    def trigger_R(self) -> int:
        # Get value of Right trigger
        return int(self.buffer[5])

    def joy_LX(self) -> int:
        # Get value of Left joystick x-axis
        return int(self.buffer[0])

    def joy_LY(self) -> int:
        # Get value of Left joystick y-axis
        return int(self.buffer[2])

    def joy_RX(self) -> int:
        # Get value of Right joystick x-axis
        return int(self.buffer[1])

    def joy_RY(self) -> int:
        # Get value of Right joystick y-axis
        return int(self.buffer[3])


# Debug: create instance and print controller output
if __name__ == '__main__':
    print("Creating Controller Object")
    controller = ClassicController(id=0, sda=8, scl=9, freq=400000)
    while (True):
        print(controller)
        sleep_ms(100)