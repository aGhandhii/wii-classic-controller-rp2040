# Author: Alex Ghandhi
# Last Modified: 11 September 2023

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
from time import sleep_ms, sleep_us


class ClassicController(object):
    # I2C Client Address
    bus_addr: int = 0x52

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
        # The buffer stores the controller output
        self.buffer = bytearray(8)

        # Create I2C client
        self.i2c = I2C(id=id, sda=Pin(sda), scl=Pin(scl), freq=freq)
        sleep_ms(10)

        # Run a bus scan, this should show '[82]' (0x52)
        print(f"Bus Scan Results: {self.i2c.scan()}")
        sleep_ms(10)

        # Decrypt Controller, check the Extension ID and set Data Mode 3
        self.decrypt_controller()
        extensionID = bytearray(6)
        self.i2c.writeto(self.bus_addr, b'\xFA')
        sleep_ms(10)
        self.i2c.readfrom_into(self.bus_addr, extensionID)
        sleep_ms(10)
        print(f"Extension ID: {extensionID}")
        print("Setting Data Mode 3")
        self.set_data_mode_3()

        # Set initial values for calibration variables
        self.joy_LX_center = 0
        self.joy_LY_center = 0
        self.joy_RX_center = 0
        self.joy_RY_center = 0
        self.trigger_L_initial = 0
        self.trigger_R_initial = 0

        # Calibrate Joysticks and Triggers
        print("Calibrating...", end="")
        self.calibrate()
        print("Complete!")

    def __str__(self):
        self.update()
        print("\033c", end='')
        print(f"Left Joy: {self.joy_LX()}, {self.joy_LY()}")
        print(f"Right Joy: {self.joy_RX()}, {self.joy_RY()}")
        print(f"Triggers: L {self.trigger_L()}, R {self.trigger_R()}")
        if (self.button_A() is 1): print("A ", end="")
        if (self.button_B() is 1): print("B ", end="")
        if (self.button_X() is 1): print("X ", end="")
        if (self.button_Y() is 1): print("Y ", end="")
        if (self.button_UP() is 1): print("up ", end="")
        if (self.button_DOWN() is 1): print("down ", end="")
        if (self.button_LEFT() is 1): print("left ", end="")
        if (self.button_RIGHT() is 1): print("right ", end="")
        if (self.button_ZL() is 1): print("ZL ", end="")
        if (self.button_ZR() is 1): print("ZR ", end="")
        if (self.button_L() is 1): print("L ", end="")
        if (self.button_R() is 1): print("R ", end="")
        if (self.button_START() is 1): print("start ", end="")
        if (self.button_SELECT() is 1): print("select ", end="")
        if (self.button_HOME() is 1): print("home ", end="")

    #####################
    # UTILITY FUNCTIONS #
    #####################

    def decrypt_controller(self) -> None:
        self.i2c.writeto(self.bus_addr, b'\xF0\x55')
        sleep_ms(10)
        self.i2c.writeto(self.bus_addr, b'\xFB\x00')
        sleep_ms(10)

    def set_data_mode_3(self) -> None:
        self.i2c.writeto(self.bus_addr, b'\xFE\x03')
        sleep_ms(10)

    def update(self) -> None:
        # Read data from controller and update the output buffer
        # From wiibrew.org: Data is reported in 8 bytes when decrypted
        self.i2c.writeto(self.bus_addr, b'\x00\x00')
        sleep_us(100)
        self.i2c.readfrom_into(self.bus_addr, self.buffer)
        sleep_us(100)

    def calibrate(self) -> None:
        # Find center values for joysticks and triggers
        self.update()
        self.joy_LX_center = self.joy_LX()
        self.joy_LY_center = self.joy_LY()
        self.joy_RX_center = self.joy_RX()
        self.joy_RY_center = self.joy_RY()
        self.trigger_L_initial = self.trigger_L()
        self.trigger_R_initial = self.trigger_R()


    ##################
    # BUTTON GETTERS #
    ##################

    def button_A(self) -> int:
        # Get value of button A
        return int(~(self.buffer[7] >> 4) & 1)

    def button_B(self) -> int:
        # Get value of button B
        return int(~(self.buffer[7] >> 6) & 1)

    def button_X(self) -> int:
        # Get value of button X
        return int(~(self.buffer[7] >> 3) & 1)

    def button_Y(self) -> int:
        # Get value of button Y
        return int(~(self.buffer[7] >> 5) & 1)

    def button_HOME(self) -> int:
        # Get value of button HOME
        return int(~(self.buffer[6] >> 3) & 1)

    def button_START(self) -> int:
        # Get value of button START
        return int(~(self.buffer[6] >> 2) & 1)

    def button_SELECT(self) -> int:
        # Get value of button SELECT
        return int(~(self.buffer[6] >> 4) & 1)

    def button_UP(self) -> int:
        # Get value of button DPAD UP
        return int(~self.buffer[7] & 1)

    def button_DOWN(self) -> int:
        # Get value of button DPAD DOWN
        return int(~(self.buffer[6] >> 6) & 1)

    def button_LEFT(self) -> int:
        # Get value of button DPAD LEFT
        return int(~(self.buffer[7] >> 1) & 1)

    def button_RIGHT(self) -> int:
        # Get value of button DPAD RIGHT
        return int(~(self.buffer[6] >> 7) & 1)

    def button_ZL(self) -> int:
        # Get value of button ZL
        return int(~(self.buffer[7] >> 7) & 1)

    def button_ZR(self) -> int:
        # Get value of button ZR
        return int(~(self.buffer[7] >> 2) & 1)

    def button_L(self) -> int:
        # Get value of button L (bottom-out for trigger)
        return int(~(self.buffer[6] >> 5) & 1)

    def button_R(self) -> int:
        # Get value of button R (bottom-out for trigger)
        return int(~(self.buffer[6] >> 1) & 1)

    def trigger_L(self) -> int:
        # Get value of Left trigger
        return int(self.buffer[4]) if self.buffer[4] > self.trigger_L_initial else 0

    def trigger_R(self) -> int:
        # Get value of Right trigger
        return int(self.buffer[5]) if self.buffer[5] > self.trigger_R_initial else 0

    def joy_LX(self) -> int:
        # Get value of Left joystick x-axis
        return int(self.buffer[0]) - self.joy_LX_center

    def joy_LY(self) -> int:
        # Get value of Left joystick y-axis
        return int(self.buffer[2]) - self.joy_LY_center

    def joy_RX(self) -> int:
        # Get value of Right joystick x-axis
        return int(self.buffer[1]) - self.joy_RX_center

    def joy_RY(self) -> int:
        # Get value of Right joystick y-axis
        return int(self.buffer[3]) - self.joy_RY_center


# Debug: create instance and print controller output
if __name__ == '__main__':
    controller = ClassicController(id=1, sda=6, scl=7, freq=100000)
    while (True):
        print(controller)
        sleep_us(100)
