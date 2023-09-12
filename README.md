# Wii Classic Controller RP2040
Use a Wii Classic Controller with the Raspberry Pi Pico and RP2040 microcontrollers. Implemented in C and MicroPython.

## C

Include the `ClassicController` header and source in a project.

Call `ClassicController_init()` to initialize and decrypt the controller for use.

*Example Code:*
```c
#include "ClassicController.h"

int main() {
    i2c_inst_t *i2c = i2c1;
    ClassicController_init(i2c, 6, 7, 100000);

    while (true) {
        ClassicController_update(i2c);
    }
}
```

<details>
<summary>C Command Reference</summary>
<br>

| Command | Description |
| --- | --- |
| `void ClassicController_init(i2c_inst_t *i2c, uint SDA, uint SCL, uint BAUDRATE)` | Initialize I2C connection, then Decrypt and Calibrate the controller |
| `void ClassicController_update(i2c_inst_t *i2c)` | Update the Button Values |
| `void ClassicController_calibrate(i2c_inst_t *i2c)` | Calibrate the Joysticks and Analog Triggers |
| `void ClassicController_button_report()` | Print a readable Input Report to the Console |
| `int ClassicController_joy_LX()` | Get value for Left Joystick x-axis $\in[-128,127]$ |
| `int ClassicController_joy_LY()` | Get value for Left Joystick y-axis $\in[-128,127]$ |
| `int ClassicController_joy_RX()` | Get value for Right Joystick x-axis $\in[-128,127]$ |
| `int ClassicController_joy_RY()` | Get value for Right Joystick y-axis $\in[-128,127]$ |
| `int ClassicController_LT_analog()` | Get value for Left Analog Trigger $\in[0,255]$ |
| `int ClassicController_RT_analog()` | Get value for Right Analog Trigger $\in[0,255]$ |
| `bool ClassicController_button_A()` | Get value for Face Button A |
| `bool ClassicController_button_B()` | Get value for Face Button B |
| `bool ClassicController_button_X()` | Get value for Face Button X |
| `bool ClassicController_button_Y()` | Get value for Face Button Y |
| `bool ClassicController_button_UP()`   | Get value for Directional Button UP |
| `bool ClassicController_button_DOWN()` | Get value for Directional Button DOWN |
| `bool ClassicController_button_LEFT()` | Get value for Directional Button LEFT |
| `bool ClassicController_button_RIGHT()`| Get value for Directional Button RIGHT |
| `bool ClassicController_button_ZL()` | Get value for Shoulder Button ZL |
| `bool ClassicController_button_ZR()` | Get value for Shoulder Button ZR |
| `bool ClassicController_button_LT()` | Get value for Shoulder Button L |
| `bool ClassicController_button_RT()` | Get value for Shoulder Button R |
| `bool ClassicController_button_START()` | Get value for START Button |
| `bool ClassicController_button_SELECT()` | Get value for SELECT Button |
| `bool ClassicController_button_HOME()` | Get value for HOME Button |

</details>


## MicroPython

Add `ClassicController.py` to the project folder.

Import the `ClassicController` class and create an instance. Initialization and decryption are performed automatically.

*Example Code:*
```python
from ClassicController import ClassicController

controller = ClassicController(id=1, sda=6, scl=7, freq=100000)
    while (True):
        controller.update()
```

<details>
<summary>MicroPython Command Reference</summary>
<br>

| Command | Description |
| --- | --- |
| `void ClassicController_init(i2c_inst_t *i2c, uint SDA, uint SCL, uint BAUDRATE)` | Initialize I2C connection, then Decrypt and Calibrate the controller |
| `update()` | Update the Button Values |
| `calibrate()` | Calibrate the Joysticks and Analog Triggers |
| `int joy_LX()` | Get value for Left Joystick x-axis $\in[-128,127]$ |
| `int joy_LY()` | Get value for Left Joystick y-axis $\in[-128,127]$ |
| `int joy_RX()` | Get value for Right Joystick x-axis $\in[-128,127]$ |
| `int joy_RY()` | Get value for Right Joystick y-axis $\in[-128,127]$ |
| `int trigger_L()` | Get value for Left Analog Trigger $\in[0,255]$ |
| `int trigger_R()` | Get value for Right Analog Trigger $\in[0,255]$ |
| `int button_A()` | Get value for Face Button A |
| `int button_B()` | Get value for Face Button B |
| `int button_X()` | Get value for Face Button X |
| `int button_Y()` | Get value for Face Button Y |
| `int button_UP()`   | Get value for Directional Button UP |
| `int button_DOWN()` | Get value for Directional Button DOWN |
| `int button_LEFT()` | Get value for Directional Button LEFT |
| `int button_RIGHT()`| Get value for Directional Button RIGHT |
| `int button_ZL()` | Get value for Shoulder Button ZL |
| `int button_ZR()` | Get value for Shoulder Button ZR |
| `int button_LT()` | Get value for Shoulder Button L |
| `int button_RT()` | Get value for Shoulder Button R |
| `int button_START()` | Get value for START Button |
| `int button_SELECT()` | Get value for SELECT Button |
| `int button_HOME()` | Get value for HOME Button |

</details>

## Hardware Notes

When connecting the Controller to the Pico, the SDA and SCL pins require pullup resistors to $3.3V$.
I used $5k\Omega$ resistors for each line.

![image](https://github.com/aGhandhii/wii-classic-controller-rp2040/assets/110639969/11e3e7a2-ff6f-4619-b1b0-60a964100337)


## References

Relies on [Wiibrew](https://wiibrew.org/wiki/Wiimote/Extension_Controllers/Classic_Controller) for specifics on the Classic Controller I2C interface.

Credits to [This Digikey Article](https://www.digikey.com/en/maker/projects/raspberry-pi-pico-rp2040-i2c-example-with-micropython-and-cc/47d0c922b79342779cdbd4b37b7eb7e2) and [PicoNunchuck](https://github.com/MichaelBell/PicoNunchuck) by MichaelBell for code inspiration.
