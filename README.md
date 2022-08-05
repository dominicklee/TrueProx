# TrueProx
Arduino library for high accuracy non-blocking presence detection using analog proximity sensors

## Overview ##
Analog sensors such as IR proximity or ultrasounds sensors can provide very accurate distance detection. However, when using such sensors, the results may be jittery. Many applications that involve presence detection require more reliable sensor readings with consistent confirmation that an area is indeed occupied or unoccupied.

This library serves as a debouncing tool to process analog values into consistent Boolean states. TrueProx is essentially a tool for signal redundancy checking. In addition to the common threshold logic, advanced users may customize parameters such as `numChecks`, `attackInterval`, and `releaseInterval`.

## Disclaimer ##
This code is provided "as is" with no warranty of any kind. This library is to be used for educational purposes. Do not use TrueProx for mission critical or life-threatening machinery. The author is not responsible and shall not be held liable for any damages or injuries as a result of using the code in this repository. You agree to use this code at your own risk.

## Installation ##

### Installing by Downloading Release ###
- Make sure you have installed the Arduino IDE and your desired board.<br />
- Download the latest release of this library from [the release page](https://github.com/dominicklee/TrueProx/releases).<br />
- In the Arduino IDE, go to "Sketch" -> "Include Library" -> "Add .ZIP Library..." and select the file you just downloaded.<br />
- You can now go to "File" -> "Examples" -> "TrueProx" and select any of the examples to get started.

### Installing via Arduino Library Manager ###
- Make sure you have installed the Arduino IDE and your desired board.<br />
- In the Arduino IDE, go to "Tools" -> "Manage Libraries..." and search for "TrueProx".<br />
- Select the latest version of the library and click "Install".<br />
- You can now go to "File" -> "Examples" -> "TrueProx" and select any of the examples to get started.

## Concept and Usage ##
This library is compatible with most Arduino boards as well as the ESP8266 and ESP32. A proximity sensor such as an IR distance module, ultrasound sensor, analog sensor should be connected to the board. 

It is assumed that you already have the code to read the sensor data into an `int` variable. This library will make use of your integer value and perform several redundant checks (also called "assertions") on it through a time series with intervals of your choosing.

To use TrueProx, you first need to know the `threshold` value for your sensor and its desired usage. For example, if your sensor reports the distance in inches and your code considers a person "in proximity" when they are closer than **10** inches, your condition would be `< 10`. To break this down, your `logic` is `'<'` and `threshold` is `10`. Once you have this info, you are ready to use the TrueProx library.

The default values for `numChecks`, `attackInterval`, and `releaseInterval` are **5**, **150**, and **500** respectively. That means when an object is detected by the sensor, TrueProx will perform **5** non-blocking assertions, each spaced between **150** milliseconds. If any single assertion fails, the counter will be reset to **0**. Once all assertions pass, the "trigger callback" will be executed. 

The "trigger callback" will provide a Boolean used to indicate **object detected** or **no object detected**. The callback should contain code for your main application when an obstacle is detected or removed (e.g. flush toilet, stop motors, turn off light, etc).

If `enableReverse(true)` is called, then TrueProx will also handle the process of awaiting for the person or object to clear the area. When the a sensor reading indicates the absence of the object, TrueProx will perform **5** non-blocking assertions, each spaced between **500** milliseconds. It can be seen that the `releaseInterval` typically has a longer period than the `attackInterval`. 

Consider a use-case of a robot with simple obstacle avoidance. The `attackInterval` needs to be low since we expect the robot to stop as soon as it senses a person. Even with redundant assertions, this latency must be low to ensure the motors stop for the person. When the person clears the vicinity, we want to be sure the person is actually gone (not walking in circles) and has completely cleared the path. Therefore, the `releaseInterval` has a more graceful value. Certainly, these values should be tested and changed for each individual application.

## Example Code ##
In the examples, we will use the "APDS 9960" sensor and its respective Arduino library to detect proximity. You can expect the sensor to have a value between 0~255. Using this value, we will implement the `TrueProx` library to get reliable object detection.

```cpp
#include <Arduino_APDS9960.h>
#include <TrueProx.h>

TrueProx trueProx('<', 230);  // Instantiate object with logic/threshold

// Callback function gets run when all assertions pass
void onTrigger(bool isActive) {
  if (isActive) {
    Serial.println("Object Detected");
  } else {
    Serial.println("No Object");
  }
}

// Callback function for assertion countdown
void onCount(int checkIndex) {
  Serial.println(checkIndex);
}

void setup() {
  Serial.begin(115200);
  while (!Serial);

  if (!APDS.begin()) {
    Serial.println("Error initializing APDS9960 sensor!");
  }

  trueProx.setChecks(5);  // Optional. Default is 5 if not declared.
  trueProx.setIntervals(150, 300);  // Optional. Default is 150, 500 if not declared.
  trueProx.setTriggerCallback(onTrigger); // Set callback
  trueProx.setCheckCallback(onCount); // Set callback
  trueProx.enableReverse(true); // Optional. Default is true.
}

void loop() {
  if (APDS.proximityAvailable()) {  // Check if a proximity reading is available

    int proximity = APDS.readProximity();	// Get the sensor value
    trueProx.sensorVal(proximity);  // Run check on sensor data (non-blocking)
  }
}
```

## License ##
Arduino TrueProx Library 
Copyright (C) 2022 by Dominick Lee.

This program is free software: you can redistribute it and/or modify it under the terms of the MIT license.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.