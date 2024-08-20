/**@file final_project.ino */



#include <ArduinoJson.h>

/** 
 * @def TRIG_PIN
 * @brief Pin connected to the Trig pin of the Ultrasonic Sensor.
 */
#define TRIG_PIN 9

/** 
 * @def ECHO_PIN
 * @brief Pin connected to the Echo pin of the Ultrasonic Sensor.
 */
#define ECHO_PIN 10

/** 
 * @def LIGHT_SENSOR_PIN
 * @brief Analog pin used for the light sensor.
 */
#define LIGHT_SENSOR_PIN A2

/** 
 * @def MAGNETIC_SENSOR_D0_PIN
 * @brief Digital pin used for the magnetic sensor.
 */
#define MAGNETIC_SENSOR_D0_PIN 3

/** 
 * @def RED_PIN
 * @brief Pin connected to the Red pin of the RGB LED.
 */
#define RED_PIN 7

/** 
 * @def GREEN_PIN
 * @brief Pin connected to the Green pin of the RGB LED.
 */
#define GREEN_PIN 6

/** 
 * @def BLUE_PIN
 * @brief Pin connected to the Blue pin of the RGB LED.
 */
#define BLUE_PIN 5

/** 
 * @def BUZZER_PIN
 * @brief Pin connected to the buzzer.
 */
#define BUZZER_PIN 4

/** 
 * @def BAUD_RATE
 * @brief Baud rate for serial communication.
 */
#define BAUD_RATE 9600

/** 
 * @def SOUND_SPEED
 * @brief Constant for ultrasonic sensor calculations. 
 * Represents the speed of sound in cm/us.
 */
#define SOUND_SPEED 0.034

/** 
 * @def PULSE_HIGH
 * @brief Pulse duration for triggering ultrasonic sensor.
 */
#define PULSE_HIGH 2

/** 
 * @def PULSE_LOW
 * @brief Pause duration between pulses for ultrasonic sensor.
 */
#define PULSE_LOW 10

/** 
 * @def GREEN_DISTANCE
 * @brief Distance threshold for green color change in cm.
 */
#define GREEN_DISTANCE 100

/** 
 * @def BLUE_DISTANCE
 * @brief Distance threshold for blue color change in cm.
 */
#define BLUE_DISTANCE 20

/** 
 * @def RED_DISTANCE
 * @brief Distance threshold for red color change and buzzer activation in cm.
 */
#define RED_DISTANCE 20

/** 
 * @def BUZZER_FREQUENCY
 * @brief Frequency of the buzzer when activated.
 */
#define BUZZER_FREQUENCY 1000

/** 
 * @def SENSOR_DELAY
 * @brief Delay between sensor readings in milliseconds.
 */
#define SENSOR_DELAY 100

/**
 * Setup function to initialize the hardware.
 */
void setup() {
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LIGHT_SENSOR_PIN, INPUT);
  pinMode(MAGNETIC_SENSOR_D0_PIN, INPUT);
  Serial.begin(BAUD_RATE);
}

/**
 * Main loop function. 
 * Reads sensors and adjusts LED and buzzer output accordingly.
 */
void loop() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(PULSE_HIGH);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(PULSE_LOW);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  int distance = duration * SOUND_SPEED / 2;

  if(distance >= GREEN_DISTANCE) {
    setColor(0, 255, 0); // Green
    noTone(BUZZER_PIN);  // Turn off the buzzer
  } else if(distance >= BLUE_DISTANCE && distance < GREEN_DISTANCE) {
    setColor(0, 0, 255); // Blue
    noTone(BUZZER_PIN);  // Turn off the buzzer
  } else if(distance < RED_DISTANCE) {
    setColor(255, 0, 0); // Red
    tone(BUZZER_PIN, BUZZER_FREQUENCY); // Turn the buzzer on at 1kHz
  }

  int lightValue = analogRead(LIGHT_SENSOR_PIN);
  int magneticValue = digitalRead(MAGNETIC_SENSOR_D0_PIN);
  int lightValueKY018 = analogRead(LIGHT_SENSOR_PIN);  // Read the light level

  // Create a JSON document with capacity for the data
  StaticJsonDocument<200> doc;

  // Add the sensor data to the document
  doc["Ultrasonic Distance"] = distance;
  doc["Light Sensor Value"] = lightValue;
  doc["Magnetic Sensor Value"] = magneticValue;
  doc["KY-018 Light sensor value"] = lightValueKY018;

  // Serialize the document as JSON and send it to the Serial
  serializeJson(doc, Serial);
  Serial.println();

  delay(SENSOR_DELAY);  // Wait 1 second before the next reading
}

/** 
 * @brief Set the color of the RGB LED.
 * 
 * @param red Intensity of the red component (0-255).
 * @param green Intensity of the green component (0-255).
 * @param blue Intensity of the blue component (0-255).
 */
void setColor(int red, int green, int blue) {
  analogWrite(RED_PIN, constrain(red, 0, 255));
  analogWrite(GREEN_PIN, constrain(green, 0, 255));
  analogWrite(BLUE_PIN, constrain(blue, 0, 255));
}
