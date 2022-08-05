#include <Arduino_APDS9960.h>
#include <TrueProx.h>

TrueProx trueProx('<', 230);  // Instantiate object with logic/threshold

// Callback function gets run in all assertions pass
void onTrigger(bool isActive) 
{
  if (isActive) {
    Serial.println("Object Detected");
    delay(5000);  // Wait 5 seconds
    trueProx.resetTrigger();  // Manually reset the state (put this line wherever you want)
    
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

  trueProx.setTriggerCallback(onTrigger); // Set callback
  trueProx.setCheckCallback(onCount); // Set callback
  
  trueProx.enableReverse(false); // Optional. Set false if you want one-way assertion.
  // If enableReverse is set to false, you would use trueProx.resetTrigger() to reset the state.
}

void loop() {
  if (APDS.proximityAvailable()) {  // Check if a proximity reading is available

    int proximity = APDS.readProximity();
    
    trueProx.sensorVal(proximity);  // Run check on sensor data
  }
}
