/*************************************************** 
Arduino library for TrueProx
Written by Dominick Lee. Founder of Makitronics, LLC.
Last Revised 8/4/2022

Analog sensors such as IR proximity or ultrasounds sensors can provide very accurate distance detection.
However, when using such sensors for presence detection, the results may be jittery.
This library serves as a debouncing tool to process analog values into consistent boolean states.

This library is provided free under the MIT license.
 ****************************************************/

#include <Arduino.h>
#include "TrueProx.h"

TrueProx::TrueProx(char logic, int threshold)
{
	_logic = logic;
	_threshold = threshold;
}

void TrueProx::setChecks(int numChecks)
{
	_numChecks = numChecks;
}

void TrueProx::setIntervals(int attackInterval, int releaseInterval)
{
	_attackInterval = attackInterval;
	_releaseInterval = releaseInterval;
}

void TrueProx::runTriggerCallback(bool isActive)
{
	if (localTriggerCallback) {	//checks to make sure user define callback function
		localTriggerCallback(isActive);
	}
}

void TrueProx::runCheckCallback(int checkIndex)
{
	if (localCheckCallback) {	//checks to make sure user define callback function
		localCheckCallback(checkIndex);
	}
}

void TrueProx::sensorVal(int analogVal)
{
  if (_isReverse != true) {
    _checkInterval = _attackInterval;
  } else {
    _checkInterval = _releaseInterval;
  }
  if (millis() - _prevMillis > _checkInterval) {
    _prevMillis = millis();
    
    if (_timeout < _numChecks) {
      _triggerSent = false;
      bool inZone = conditionMet(analogVal);
      if (_isReverse != true) {
        if (!inZone) {
          _timeout = 0;  //reset counter. Hold it
        } else {
          _timeout ++; //increment and wait for next
        }
      }
      else {
        if (inZone) {
          _timeout = 0;  //reset counter. Hold it
        } else {
          _timeout ++; //increment and wait for next
        }
      }
      
      if (_timeout == 0) {
        if (_zeroSent != true) { //only send zero once
          runCheckCallback(_timeout);
          _zeroSent = true;
        }
      }
      else {
        _zeroSent = false;
        runCheckCallback(_timeout);
      }
    }
    else {
      if (_triggerSent != true) {
        runTriggerCallback(!_isReverse);
        _triggerSent = true;
      }
      if (_reverseEnabled) {
        _isReverse = !_isReverse;
        _timeout = 0;
      }
    }
  }
}

void TrueProx::enableReverse(bool isEnabled)
{
	_reverseEnabled = isEnabled;
}

void TrueProx::resetTrigger()
{
	_timeout = 0;
	runTriggerCallback(false);
}

bool TrueProx::conditionMet(int val)
{
  if (_logic == '>') {
    if (val > _threshold) {
      return true;
    } else {
      return false;
    }
  }
  else if (_logic == '=') {
    if (val == _threshold) {
      return true;
    } else {
      return false;
    }
  }
  else if (_logic == '<') {
    if (val < _threshold) {
      return true;
    } else {
      return false;
    }
  }
  else {
    return false;
  }
}

