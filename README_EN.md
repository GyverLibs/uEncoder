This is an automatic translation, may be incorrect in some places. See sources and examples!

#uEncoder
Another class of encoder and encoder with a button (based on [uButton](https://github.com/GyverLibs/uButton)) for Arduino.

In terms of API, this is 99% analogous to [EncButton](https://github.com/GyverLibs/EncButton), but a little lighter, 1.5 times faster to poll and written in a more readable finite-automata style, and not on flags.

### Compatibility
Compatible with all Arduino platforms (uses Arduino functions)

### Dependencies
- [uButton](https://github.com/GyverLibs/uButton)
- [GyverIO](https://github.com/GyverLibs/GyverIO)

## Contents
- [Usage](#usage)
- [Versions](#versions)
- [Installation](#install)
- [Bugs and feedback](#feedback)

<a id="usage"></a>

## Usage
### Settings Defines
Declare before connecting the library

```cpp
#define UE_FAST_TIME 32 // time between ticks for fast rotation
```

### Classes
####uEncoderVirt
Virtual encoder class without button.

```cpp
// =============== SETTINGS ===============

// encoder types
enum class Type {
Step4Low, // active low signal (pull up to VCC).Full period (4 phases) per click
Step4High, // active high signal (pull-up to GND).Full period (4 phases) per click
Step2, // half a period (2 phases) per click
Step1, // quarter period (1 phase) per click
};

// set the encoder type (Type::Step4Low, Type::Step4High, Type::Step2, Type::Step1)
void setEncType(Type type);

// invert encoder direction
void setEncReverse(bool rev);

// encoder initialization
void initEnc(bool e0, bool e1);

// =============== STATUS ===============

enum class State {
Idle, // idle
Left, // turn left
LeftFast, // fast turn left
LeftHold, // pressed turn left
LeftHoldFast, // fast pressed turn to the left
Right, // turn right
RightFast, // fast turn to the right
RightHold, // pressed turn right
RightHoldFast, // pressed fast turn to the right
};
State getState();// get the current state
void reset();// reset the state (forcefully end processing)

// =============== EVENTS ===============

// there was a turn [event]
uint8_t turn();

// encoder direction (1 or -1) [state]
int8_t dir();

// encoder direction (0 or 1) [state]
bool dirBool();

// pressed encoder rotation [event]
bool turnH();

// fast encoder rotation [state]
bool fast();

// turn right [event]
bool right();

// turn left [event]
bool left();

// pressed turn right [event]
bool rightH();

// pressed turn left [event]
bool leftH();

// encoder button pressed [state]
bool encHolding();

// =============== POLL ===============

// poll the encoder
bool tick(bool e0, bool e1, bool pressed = false);

// poll the encoder without changing state
State poll(bool e0, bool e1, bool pressed = false);
```

####uEncoder
The `uEncoder` class inherits `uEncoderVirt`, sending data from it to `poll` inside `tick`.

```cpp
// yshow pins and their mode of operation
uEncoder(uint8_t p0, uint8_t p1, uint8_t mode = INPUT);

// poll the encoder
bool tick(bool pressed = false);

// poll the encoder in the interrupt
void tickISR(bool pressed = false);
```

####uEncButton
The `uEncButton` class inherits `uEncoder` and `uButton` and polls them.All button events are available.Turning the encoder while the button is pressed resets all button events except release.

```cpp
uEncButton(uint8_t encA, uint8_t encB, uint8_t btn, uint8_t modeEnc = INPUT, uint8_t modeBtn = INPUT_PULLUP);

// poll.Returns true on event
bool tick();

// poll the encoder in the interrupt
void tickISR();
```

## Examples
```cpp

// timeout settings, ms
// #define UE_FAST_TIME 32 // time between ticks for fast rotation

#include <uEncButton.h>

uEncButton eb(2, 3, 4);

// Enka polling mode in interrupt
// void isr() {
// eb.tickISR();
// }

void setup() {
Serial.begin(115200);

// Enka polling mode in interrupt
// attachInterrupt(0, isr, CHANGE);
// attachInterrupt(1, isr, CHANGE);
}

void loop() {
eb.tick();

// general rotation processing
if (eb.turn()) {
Serial.print("turn: dir ");
Serial.print(eb.dir());
Serial.print(", fast ");
Serial.print(eb.fast());
Serial.print(", hold ");
Serial.print(eb.pressing());
Serial.print(", clicks ");
Serial.print(eb.getClicks());
Serial.println();
}

// separate rotation processing
if (eb.left()) Serial.println("left");
if (eb.right()) Serial.println("right");
if (eb.leftH()) Serial.println("leftH");
if (eb.rightH()) Serial.println("rightH");

// button
if (eb.press()) Serial.println("press");
if (eb.click()) Serial.println("click");
if (eb.release()) Serial.println("release");

// general timeout
if (eb.timeout(1000)) Serial.println("timeout");
}
```

<a id="versions"></a>

## Versions
- v1.0

<a id="install"></a>
## Installation
- The library can be found by name **uEncoder** and installed through the library manager in:
- Arduino IDE
- Arduino IDE v2
- PlatformIO
- [Download library](https://github.com/GyverLibs/uEncoder/archive/refs/heads/main.zip).zip archive for manual installation:
- Unpack and put in *C:\Program Files (x86)\Arduino\libraries* (Windows x64)
- Unpack and put in *C:\Program Files\Arduino\libraries* (Windows x32)
- Unpack and put in *Documents/Arduino/libraries/*
- (Arduino IDE) automatic installation from .zip: *Sketch/Connect library/Add .ZIP library…* and indicate the downloaded archive
- Read more detailed instructions for installing libraries[here](https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%D0%BD%D0%BE%D0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)
### Update
- I recommend always updating the library: in new versions errors and bugs are corrected, as well as optimization is carried out and new features are added
- Through the IDE library manager: find the library as during installation and click "Update"
- Manually: **delete the folder with the old version**, and then put the new one in its place.“Replacement” cannot be done: sometimes new versions delete files that will remain after replacement and can lead to errors!

<a id="feedback"></a>

## Bugs and feedback
When you find bugs, create an **Issue**, or better yet, immediately write to [alex@alexgyver.ru](mailto:alex@alexgyver.ru)
The library is open for improvement and your **Pull Requests**!

When reporting bugs or incorrect operation of the library, be sure to indicate:
- Library version
- Which MK is used?
- SDK version (for ESP)
- Arduino IDE version
- Do the built-in examples that use functions and constructs that lead to a bug in your code work correctly?
- What code was loaded, what work was expected from it and how it works in reality
- Ideally, attach the minimum code in which the bug is observed.Not a canvas of a thousand lines, but a minimal onecode