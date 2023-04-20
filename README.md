# GDP32 - Micro-Sampler
The micro sampler is designed with variable samples in mind so the software
is designed the same. There are a few configurable options in the software.
- **COMM_PROTO** (terminal, rs232, i2c, pwm)
    - Communication protocol to be used rs232 is most common for Autonomous
    Underwater Vehicles (AUV's) and is the most secure to deployment vehicles.
- **SAMPLES** (default: 6)
    - Sample number depends on design limits the number of samples collected.
- **ROTATION_DISTANCE** (default: 32.5)
    - Must be calculated via practical tests or CAD it determines how much the
    cam moves each activation.
- **BAUD_RATE** (default: 9600)
    - Baud rate the sampler communicates at over serial.

The pinout on the schematic is labelled unhelpfully in the current version.
Sampler is designed for VCC 5v.
- Interface Pinout for Arduino:
    - `GND A4 A5 GND VCC`

## PCB Diagram
![PCB Wiring Diagram](/images/PCB_design.PNG)

![PCB Schematic](/images/Schematic.PNG)

## Communication Protocols (COMM_PROTO)
### Terminal
Gives a menu with commands and options.
Commands function identically to the RS232 protocol.
Only avaliable through the Atmega328p, TX, RK pins which are wired to the programmer port.

### RS232
- This protocol has 3 commands:
    - `$sample`
        - Triggers the sampler to rotate the servo and returns `$ok`.
        At sample 6 the servo will reset to the zero position.
        - (Careful this will allow for additional software triggers but 
        no physical collection, error checking needs to be added here.)
    - `$reset`
        - Resets the sampler to zero position returns `$ok`.
    - `$state`
        - Responds `$ok` if the sampler is alive, used for checking pinout.
- Current Pinout:
    - `GND RK  TX  GND VCC`

### I2C
- The sampler listens to address 9, this is customisable but be aware of conflicts with other I2C devices.
Expecting a request of 3 bytes and will return `$ok` no other commands are avaliable.
- Current Pinout:
    - `GND SDA SCL GND VCC`

### PWM
- Still to be implemented.
- Current plan is 4 PWM pulses of 200ms with 100ms gap.
- Current Pinout:
    -  `GND PWM GND GND VCC`

## Known Issues
- Interface port labelling is for I2C only and not all other communication protocols.
    - Current Resolution: List pinouts from left to right for each communication protocol.
- Servo port labelling is inverted.
    - Current Resolution: Pinout should be `SIG VCC GND` NOT `GND VCC SIG` as marked
