#include <BH1750.h>
#include <Wire.h>

//Light sensor = BH1750
//UV sensor = CJMCU GUVA-S12SD
//source for light levels: https://www.engineeringtoolbox.com/light-level-rooms-d_708.html
//source for sun burn risk: https://pdskin.com/blogs/uv-index-the-sun-safety-scale/
//source for Vitamin D production based on UV and time: https://www.gbhealthwatch.com/Did-you-know-Get-VitD-Sun-Exposure.php 

BH1750 lightMeter;
const int buttonPin = 13;  
const int ledPin = 12;
int buttonState = LOW;
int lastButtonState = LOW;

bool timing = false;
unsigned long pressStartTime = 0;
unsigned long pressEndTime = 0;

unsigned long previousMillis = 0;
const long interval = 2000; // read sensors every two seconds

bool sense = false;
float lightAvg = 0;
float uvAvg = 0;
unsigned long readCount = 0;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  lightMeter.begin();
  Serial.println("BH1750 begin");
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
}

void loop() {
  // Read the current state of the button
  int reading = digitalRead(buttonPin);

  // button was just pressed
  if (reading == HIGH && lastButtonState == LOW) {
    if (!timing) {
      pressStartTime = millis();
      beginningActivites();
      
    } else {
      pressEndTime = millis();
      endingActivites();
    }
    delay(50); // basic debounce
  }

  // Update last button state
  lastButtonState = reading;

  if (sense){
    readSensors();
  }

}


void readSensors() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    readCount++;

    // Light sensor
    float lx = lightMeter.readLightLevel();
    lightAvg += lx;

    Serial.print("Light: ");
    Serial.print(lx);
    Serial.println(" lx");

    // UV sensor
    float sensorValue = analogRead(A0);
    float sensorVoltage = sensorValue / 1024.0 * 5.0; // convert analog value to voltage
    float uvIndex = sensorVoltage / 0.1;
    uvAvg += uvIndex;

    Serial.print("UV Sensor Reading: ");
    Serial.print(sensorValue);
    Serial.print("    Voltage: ");
    Serial.print(sensorVoltage);
    Serial.print(" V");
    Serial.print("    UV index: ");
    Serial.println(uvIndex);
  }

}


void beginningActivites() {
  timing = true;
  sense = true;
  lightAvg = 0;
  uvAvg = 0;
  readCount = 0;
  // light LED while timing
  digitalWrite(ledPin, HIGH);
  Serial.println("Timer started.");
}


void endingActivites() {
  unsigned long elapsed = (pressEndTime - pressStartTime) / 1000;
  Serial.print("\nTime between presses: ");
  Serial.print(elapsed);
  Serial.println(" seconds");
  timing = false;
  sense = false;
  // turn off LED when stop timing
  digitalWrite(ledPin, LOW);

  lightAvg = lightAvg/readCount;
  uvAvg = uvAvg/readCount;
  Serial.print("Light average: ");
  Serial.print(lightAvg);
  Serial.print(" lx");
  Serial.print("\t\tUV index average: ");
  Serial.println(uvAvg);

  String sensorData = sensorResult();
  String timeElapsed = timeResult(elapsed);
  // based on UV index and time, make conclusions about vitamin D production and risk of sunburn
  String interpretSummary = interpretResult(elapsed); 

  Serial.println("\nSummary: ");
  Serial.print("You were ");
  Serial.print(sensorData);
  Serial.print(" for ");
  Serial.println(timeElapsed);
  Serial.println(interpretSummary);
}


// the UV ranges have been adjusted to the sensitivity of the UV sensor which appears higher than actual
String sensorResult() {
  String result = "";

  if (uvAvg < 2.8) { // low uv (<1.5) means inside
    if (lightAvg < 750) {
      result = "inside";
    }
    else { // more light means by window
      result = "inside by a window";
    }
  }
  else if (uvAvg >= 2.8 && uvAvg <= 5) { // moderate uv (1.5-4) means by window or cloudy outside
    if (lightAvg <= 1500) {
      result = "inside by a window";
    }
    else {
      result = "outside (cloudy or shade)";
    }
  }
  else { // high uv means outside or direct sun through a window
    if (lightAvg > 1500){
      result = "outside";
    }
    else {
      result = "inside by a window";
    }
  }

  return result;
}


String timeResult(int sec) {
  int hours = sec / 3600;
  sec = sec % 3600;
  int min = sec / 60;
  sec = sec % 60;

  String result = String(hours) + "h : " + String(min) + "m : " + String(sec) + "s";
  return result;

}


String interpretResult(int sec) {
  // interpret for vitamin D and sunburn risk
  float uvPercent = 0.0;
  String vitD = "";
  String burn = "There is limited risk of sunburn.";

  if (uvAvg < 3) {
    vitD = "UV is too low to produce vitamin D. You received 0% of your daily amount of vitamin D. ";
    if (sec >= 3300 && sec <= 3900) { // 60 mins +- 5 min range
      burn = "There is a risk of sunburn. Apply broad spectrum sunscreen if you intend to stay in the current environment.";
    }
  }
  else if (uvAvg >= 3 && uvAvg < 6) { // for daily vit D needs: average 25 mins (1500 seconds) for total amount
    uvPercent = (sec/1500.0) * 100.0;
    if (sec >= 1920 && sec <= 2520) { // 37 mins +- 5 min range
      burn = "There is a risk of sunburn. Apply sunscreen every 2 hours.";
    }
  }
  else if (uvAvg >= 6 && uvAvg < 8) { // for daily vit D needs: average 17 mins (1020 seconds) for total amount
    uvPercent = (sec/1020.0) * 100.0;
    if (sec >= 900 && sec <= 1500) { // 20 mins +- 5 min range
      burn = "There is a risk of sunburn. Apply sunscreen every 2 hours.";
    }
  }
  else if (uvAvg >= 8 && uvAvg < 10) { // for daily vit D needs: average 12 mins (720 seconds) for total amount
    uvPercent = (sec/720.0) * 100.0;
    if (sec >= 540 && sec <= 900) { // 12 mins +- 3 min range
      burn = "There is a risk of sunburn. Minimize sun exposure. Re-apply sunscreen and SPF lip balm.";
    }
  }
  else { // for daily vit D needs: average 7 mins (420 seconds) for total amount
    uvPercent = (sec/420.0) * 100.0;
    if (sec >= 300 && sec <= 660) { // 8 mins +- 3 min range
      burn = "There is a risk of sunburn. Avoid sun exposure during the day. It can be dangerous at an extreme UV index.";
    }
  }
  uvPercent = round(uvPercent * 100.0) / 100.0; // round to two decimal places
  if (vitD.length() == 0) {
    return "You received " + String(uvPercent) + "% of your daily amount of vitamin D. " + burn;
  }
  else {
    return vitD + burn;
  }
  
}

