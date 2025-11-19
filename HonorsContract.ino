#include <BH1750.h>
#include <Wire.h>

//Light sensor = BH1750
//UV sensor = CJMCU GUVA-S12SD
//website for light levels: https://www.engineeringtoolbox.com/light-level-rooms-d_708.html

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
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.begin();
  lightMeter.begin();
  Serial.println(F("BH1750 Test begin"));
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
  Serial.print("Time between presses: ");
  Serial.print(elapsed);
  Serial.println(" seconds");
  timing = false;
  sense = false;
  // turn off LED when stop timing
  digitalWrite(ledPin, LOW);

  Serial.print("\nLight average: ");
  Serial.print(lightAvg);
  Serial.print("\tUV average: ");
  Serial.println(uvAvg);


  String sensorResult = results();
  String timeElapsed = timeResult(elapsed);

  Serial.print("\nYou were " + sensorResult + " for " + timeElapsed);
}

void readSensors() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    readCount++;

    // Light sensor
    float lux = lightMeter.readLightLevel();
    lightAvg += (lux - lightAvg) / readCount;

    Serial.print("Light: ");
    Serial.print(lux);
    Serial.println(" lx");

    // UV sensor
    float sensorValue = analogRead(A0);
    float sensorVoltage = sensorValue / 1024.0 * 5.0; // convert analog value to voltage
    float uvIndex = sensorVoltage / 0.1;
    uvAvg += (uvIndex - uvAvg) / readCount;

    Serial.print("UV Sensor Reading: ");
    Serial.print(sensorValue);
    Serial.print("    Voltage: ");
    Serial.print(sensorVoltage);
    Serial.print(" V");
    Serial.print("    UV index: ");
    Serial.println(uvIndex);
  }

}

String results() {
  String result = "";

  if (uvAvg < 1.5) { // low uv means inside
    if (lightAvg < 750) {
      result = "inside";
    }
    else { // more light means by window
      result = "inside by a window";
    }
  }
  else if (uvAvg >= 1.5 && uvAvg <= 4) { // moderate uv means by window or cloudy outside
    if (lightAvg <= 1500) {
      result = "inside by a window";
    }
    else {
      result = "outside (cloudy or shade)";
    }
  }
  else { // high uv means outside or direct5 sun through a window
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

  String str = String(hours) + "h : " + String(min) + "m : " + String(sec) + "s";
  return str;

}


/*
  String uv = "";
  if (uvAvg <= 2){
    uv = "low";
  }
  else if (uvAvg > 2 && uvAvg <= 5){
    uv = "moderate";
  }
  else if (uvAvg > 5 && uvAvg <= 7){
    uv = "high";
  }
  else {
    uv = "very high";
  }

  String light = "";
  if (lightAvg > 0 && lightAvg <= 750) { //0 - 750
    light = "indoors";
  }
  else if (lightAvg > 750 && lightAvg <= 1500){ // 750 - 1500
    light = "by window inside";
  }
  else { // 1500 to max
    light = "outside";
  }
  */

  /*
  //read light sensor
  float lux = lightMeter.readLightLevel();
  
  //read UV sensor
  float sensorValue;
  float sensorVoltage; 
 
  sensorValue = analogRead(A0);
  sensorVoltage = sensorValue/1024*5.0;

  //print light sensor info
  Serial.print(lux);
  Serial.println(" lx");
  delay(2000);

  //print UV sensor info
  Serial.print("sensor reading = ");
  Serial.print(sensorValue);
  Serial.print("        sensor voltage = ");
  Serial.print(sensorVoltage);
  Serial.println(" V");
  delay(2000);
  */