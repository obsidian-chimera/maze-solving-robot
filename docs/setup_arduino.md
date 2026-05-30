# Arduino Setup

## Current Setup

Main sketch:

```text
arduino/main_robot/main_robot.ino
```

The sketch includes local project headers:

```cpp
#include "src/config.h"
#include "src/sensors.h"
#include "src/motors.h"
#include "src/fsm.h"
```

The current code uses Arduino core functions such as `pinMode`, `digitalWrite`, `analogWrite`, `analogRead`, `millis`, and `Serial`.

No third-party Arduino library is currently required.

## PlatformIO in VS Code

This repository includes `platformio.ini` for Arduino Uno:

```ini
[platformio]
src_dir = arduino/main_robot

[env:uno]
platform = atmelavr
board = uno
framework = arduino
monitor_speed = 9600
```

1. Open the repository root folder in VS Code.
2. Install the PlatformIO IDE extension if it is not already installed.
3. Wait for PlatformIO Core to finish initializing.
4. Open the PlatformIO panel.
5. Click Build to compile the project.
6. Connect the Arduino Uno and select the correct serial port.
7. Click Upload to flash the board.
8. Click Monitor to open the serial monitor at `9600` baud.

Equivalent terminal commands:

```bash
pio run
pio run -t upload
pio device monitor
```

## Arduino IDE

1. Open `arduino/main_robot/main_robot.ino`.
2. Select the correct Arduino board.
3. Select the correct port.
4. Click Verify to compile.
5. Click Upload after the code compiles.
6. Open Serial Monitor at `9600` baud for debug output.

## VS Code Arduino Workflow

1. Open this repository folder in VS Code.
2. Install an Arduino extension if needed.
3. Open `arduino/main_robot/main_robot.ino`.
4. Select the board and port in the extension.
5. Run verify/compile before upload.

## Before Running on Hardware

- Check [pin map](pin_map.md).
- Check [interface spec](interface_spec.md).
- Confirm Motor A steering direction before running at full speed.
- Confirm Motor B forward-drive direction before running at full speed.
- Replace the placeholder encoder constants in `config.h` with measured robot values.
- Set `USE_DUMMY_MOTORS`, `USE_DUMMY_SENSORS`, and `USE_DUMMY_ENCODERS` to `false` only after the matching real subsystem is integrated.
- Test with wheels raised before testing in the maze.

## Future Libraries

If a third-party Arduino library is added later, document it here with:

- library name
- reason for use
- install method
- example include line
