# TrueProx
Arduino library for high accuracy non-blocking presence detection using analog proximity sensors

## Overview ##
Analog sensors such as IR proximity or ultrasounds sensors can provide very accurate distance detection. However, when using such sensors for presence detection, the results may be jittery. Many applications that use presence detection use

This library serves as a debouncing tool to process analog values into consistent Boolean states. TrueProx is essential a tool for signal redundancy checking. In addition to the common threshold logic, advanced users may customize parameters such as `numChecks`, `attackInterval`, and `releaseInterval`.

## Technical ##
This library is compatible with most Arduino boards as well as the ESP8266 and ESP32. A proximity sensor such as an IR distance module, ultrasound sensor, analog sensor should be connected to the board. 

It is assumed that you already have the code to read the sensor data into an `int` variable. This library will make use of your integer value and perform several redundant checks on it through a time series with intervals of your choosing.

To use TrueProx, you first need to know the `threshold` value for your sensor and its desired usage. For example, if your sensor reports the distance in inches and your code considers a person "in proximity" when they are closer than **10** inches, your condition would be `< 10`. To break this down, your `logic` is `'<'` and `threshold` is `10`. Once you have this info, you are ready to use the TrueProx library.

The default values for `numChecks`, `attackInterval`, and `releaseInterval` are **5**, **150**, and **500** respectively. That means when an object is detected by the sensor, TrueProx will perform **5** non-blocking assertions, each spaced between **150** milliseconds. If any single assertion fails, the counter will be reset to **0**. Once all assertions pass, the "trigger callback" will be executed. 

The "trigger callback" will provide a Boolean used to indicate **object detected** or **no object detected**. The callback should contain code for your main application when an obstacle is detected or removed (e.g. flush toilet, stop motors, turn off light, etc).

If `enableReverse(true)` is called, then TrueProx will also handle the process of awaiting for the person or object to clear the area. When the a sensor reading indicates the absence of the object, TrueProx will perform **5** non-blocking assertions, each spaced between **500** milliseconds. It can be seen that the `releaseInterval` typically has a longer period than the `attackInterval`. 

Consider a use-case of a robot with simple obstacle avoidance. The `attackInterval` needs to be low since we expect the robot to stop as soon as it senses a person. Even with redundant checking, this latency must be low to ensure the motors stop for the person. When the person clears the vicinity, we want to be sure the person is actually gone (not walking in circles) and has completely cleared the path. Therefore, the `releaseInterval` has a more graceful value. Certainly, these values should be changed for each individual application.

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