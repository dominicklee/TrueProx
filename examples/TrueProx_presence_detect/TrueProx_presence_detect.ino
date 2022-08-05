#include <Arduino_APDS9960.h>
#include <TrueProx.h>

TrueProx trueProx('<', 230);  // Instantiate object with logic/threshold

// Callback function gets run in all assertions pass
void onTrigger(bool isActive) 
{
  if (isActive) {
    Serial.println("Object Detected");
  } else {
    Serial.println("No Object");
  }
}

// Callback function for assertion countdown
void onCount(int checkIndex) 
{
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

    int proximity = APDS.readProximity();
    
    trueProx.sensorVal(proximity);  // Run check on sensor data
  }
}
