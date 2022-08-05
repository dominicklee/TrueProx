/*************************************************** 
Arduino library for TrueProx
Written by Dominick Lee. Founder of Makitronics, LLC.
Last Revised 8/4/2022

Analog sensors such as IR proximity or ultrasounds sensors can provide very accurate distance detection.
However, when using such sensors for presence detection, the results may be jittery.
This library serves as a debouncing tool to process analog values into consistent boolean states.

This library is provided free under the MIT license.
 ****************************************************/

#ifndef TRUEPROX_H		//Standard declaration prevents redeclaring twice
#define TRUEPROX_H
 
#include <Arduino.h>

class TrueProx {
	
public:
	TrueProx(char logic, int threshold);
	void setChecks(int numChecks);
	void setIntervals(int attackInterval, int releaseInterval);
	
	void runTriggerCallback(bool isActive);
	void setTriggerCallback(void (*userDefinedCallback)(bool)) {
				  localTriggerCallback = userDefinedCallback; }

	void runCheckCallback(int checkIndex);
	void setCheckCallback(void (*userDefinedCallback)(int)) {
				  localCheckCallback = userDefinedCallback; }

	void sensorVal(int analogVal);
	void enableReverse(bool isEnabled);
	void resetTrigger();
	
private:
	void (*localTriggerCallback)(bool);
	void (*localCheckCallback)(int);
	bool conditionMet(int val);
	
	char _logic = '<';         //greater or less than
	int _threshold = 230;      //setpoint
	int _numChecks = 5;       //number of checks to perform
	int _checkInterval = 150;  //runtime number of miliseconds per check
	unsigned long _prevMillis = 0; //last checked
	int _timeout = 0;        //counter for checking
	bool _isReverse = false; //runtime for reverse
	bool _reverseEnabled = true;  //user based setting
	bool _zeroSent = false;  //to prevent repeatedly sending zeros
	int _attackInterval = 150; //number of miliseconds per check prior to activate
	int _releaseInterval = 500; //number of miliseconds per check prior to deactivate
	bool _triggerSent = false; //runtime for single send trigger
};

#endif // End of TRUEPROX_H