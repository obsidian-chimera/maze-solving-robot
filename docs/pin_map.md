# Pin Map

## Project Pins

| Pin | Code Name | Function |
|---|---|---|
| D2 | `LEFT_SIDE_SWITCH_PIN` | left side switch |
| D4 | `RIGHT_SIDE_SWITCH_PIN` | right side switch |
| D6 | `SELF_MADE_H_BRIDGE_IN1_PIN` | self-made H-bridge IN1 |
| D7 | `SELF_MADE_H_BRIDGE_IN2_PIN` | self-made H-bridge IN2 |
| D10 | `FRONT_IR_LED_ENABLE_PIN` | optional front IR LED enable, currently not controlled by code |
| D3 | `MOTOR_A_PWM` | Motor A PWM, controls left/right steering strength |
| D11 | `MOTOR_B_PWM` | Motor B PWM, controls forward drive strength |
| A2 | `FRONT_PHOTODIODE_PIN` | front photodiode |
| A3 | `REAR_PHOTODIODE_PIN` | rear photodiode |

## Motor Control Pins

| Pin | Function | Note |
|---|---|---|
| D3 | `MOTOR_A_PWM` | PWM for left/right steering strength |
| D11 | `MOTOR_B_PWM` | PWM for forward drive strength |
| D6 | `SELF_MADE_H_BRIDGE_IN1_PIN` | H-bridge direction input for steering |
| D7 | `SELF_MADE_H_BRIDGE_IN2_PIN` | H-bridge direction input for steering |

DIR and BRAKE pins from the original motor shield plan are not used in the current code.

## Analog Pins

Use A2-A5 for analog sensors in the current project layout.

| Pin | Use |
|---|---|
| A0 | Spare |
| A1 | Spare |
| A2 | front photodiode |
| A3 | rear photodiode |
| A4 | Spare |
| A5 | Spare |
