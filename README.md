
# PacmINO — Pac-Man® on Arduino!
![Tested on Arduino Uno](https://img.shields.io/badge/Tested%20on-Arduino%20Uno-blue)
![Tested on Arduino Nano](https://img.shields.io/badge/Tested%20on-Arduino%20Nano-green)
## How to setup
### Requirements
* 30–60 minutes of time
* Arduino Uno or Nano
* SSD1306 OLED display (128x64, I2C)
* 4 buttons **or** a joystick module
* Jumper wires
* A computer with Arduino IDE **or** PlatformIO
### Step 1: Download the sketch
Download [pacm.ino from Releases](https://github.com/oversouth/pacmINO/releases/tag/pacmino) and open it in Arduino IDE.
### Step 2: Install libraries
In Arduino IDE go to **Tools → Manage Libraries** and install:
| Library | Author |
|---|---|
| Adafruit SSD1306 | Adafruit |
| Adafruit GFX Library | Adafruit |
| Adafruit BusIO | Adafruit |
> When installing SSD1306, click **"Install All"** to get GFX and BusIO automatically.
### Step 3: Wire the display
Connect the OLED via I2C:
| OLED | Arduino |
|---|---|
| VCC | 3.3V or 5V |
| GND | GND |
| SDA | A4 |
| SCL | A5 
### Step 4: Wire the controls
#### Option A — Buttons (pins 4–7)
| Button | Pin |
|---|---|
| Up | 7 |
| Down | 6 |
| Left | 5 |
| Right | 4 |
Wire each button between its pin and GND with a 10kΩ pull-down resistor.
#### Option B — Joystick
Connect X-axis → A1, Y-axis → A0, VCC → 5V, GND → GND.  
Then in `pacm.ino`, comment out the `digitalRead()` lines in the `input_*` functions and uncomment the `analogRead()` lines below them.
### Step 5: Upload
Select your board (**Tools → Board**) and port (**Tools → Port**), then hit Upload.
## Features
- Ghost AI with random pathfinding
- Power-up orbs (eat ghosts for 10,000 pts!)
- 3 lives
- Best time saved to EEPROM
- Menu + credits screen

> NO GENERATIVE AI WAS USED FOR CODE,GENERATIVE AI USED FOR DOCUMENTATION
