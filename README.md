<div align="center">

<img src="https://github.com/aGhandhii/wii-classic-controller-rp2040/assets/110639969/1bc2b27c-4221-4cb0-8ee1-11edcf9ccd25" />

<img src="https://img.shields.io/badge/License-The_Unlicense-red" />

# Wii Classic Controller RP2040

Use a Wii Classic Controller with the Raspberry Pi Pico and RP2040 microcontrollers.  
Implemented in C and MicroPython.

## C

Include the `ClassicController` header and source in a project.

Call `ClassicController_init()` to initialize and decrypt the controller for use.

</div>

*Example Code:*
```c
#include "ClassicController.h"

int main() {
    i2c_inst_t *i2c = i2c1;
    ClassicController controller;

    stdio_init_all();

    controller = ClassicController_init(i2c, 6, 7, 100000);

    while (true) {
        ClassicController_update(controller);
    }
}
```

<div align="center">

<details>
<summary>C Command Reference</summary>
<br>

| Command | Description |
| --- | --- |
| `ClassicController ClassicController_init(i2c_inst_t *i2c, uint sda, uint scl, uint baudrate)` | Initialize I2C connection, then Decrypt and Calibrate the controller |
| `void ClassicController_update(ClassicController controller)` | Update the Button Values |
| `void ClassicController_calibrate(ClassicController controller)` | Calibrate the Joysticks and Analog Triggers |
| `void ClassicController_button_report(ClassicController controller)` | Print a readable Input Report to the Console |
| `int ClassicController.LX` | Get value for Left Joystick x-axis $\in[-128,127]$ |
| `int ClassicController.LY` | Get value for Left Joystick y-axis $\in[-128,127]$ |
| `int ClassicController.RX` | Get value for Right Joystick x-axis $\in[-128,127]$ |
| `int ClassicController.RY` | Get value for Right Joystick y-axis $\in[-128,127]$ |
| `int ClassicController.LT_ANALOG` | Get value for Left Analog Trigger $\in[0,255]$ |
| `int ClassicController.RT_ANALOG` | Get value for Right Analog Trigger $\in[0,255]$ |
| `bool ClassicController.A` | Get value for Face Button A |
| `bool ClassicController.B` | Get value for Face Button B |
| `bool ClassicController.X` | Get value for Face Button X |
| `bool ClassicController.Y` | Get value for Face Button Y |
| `bool ClassicController.UP`   | Get value for Directional Button UP |
| `bool ClassicController.DOWN` | Get value for Directional Button DOWN |
| `bool ClassicController.LEFT` | Get value for Directional Button LEFT |
| `bool ClassicController.RIGHT`| Get value for Directional Button RIGHT |
| `bool ClassicController.ZL` | Get value for Shoulder Button ZL |
| `bool ClassicController.ZR` | Get value for Shoulder Button ZR |
| `bool ClassicController.LT` | Get value for Shoulder Button L |
| `bool ClassicController.RT` | Get value for Shoulder Button R |
| `bool ClassicController.START` | Get value for START Button |
| `bool ClassicController.SELECT` | Get value for SELECT Button |
| `bool ClassicController.HOME` | Get value for HOME Button |

</details>


## MicroPython

Add `ClassicController.py` to the project folder.

Import the `ClassicController` class and create an instance. Initialization and decryption are performed automatically.

</div>

*Example Code:*
```python
from ClassicController import ClassicController

controller = ClassicController(id=1, sda=6, scl=7, freq=100000)
    while (True):
        controller.update()
```

<div align="center">

<details>
<summary>MicroPython Command Reference</summary>
<br>

| Command | Description |
| --- | --- |
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
I used $4.7k\Omega$ resistors for each line.

<details>
<summary>Sample Circuit</summary>
<br>

![sample_schematic](https://github.com/aGhandhii/wii-classic-controller-rp2040/assets/110639969/d9e0f751-a839-44f9-adb1-3c8d660ad360)

</details>

## References

Relies on [Wiibrew](https://wiibrew.org/wiki/Wiimote/Extension_Controllers/Classic_Controller) for specifics on the Classic Controller I2C interface.

Credits to [This Digikey Article](https://www.digikey.com/en/maker/projects/raspberry-pi-pico-rp2040-i2c-example-with-micropython-and-cc/47d0c922b79342779cdbd4b37b7eb7e2) and [PicoNunchuck](https://github.com/MichaelBell/PicoNunchuck) by MichaelBell for code inspiration.

</div>
