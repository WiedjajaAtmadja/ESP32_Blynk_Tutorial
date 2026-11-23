/*************************************************************

  This is a simple demo of sending and receiving some data.
  Be sure to check out other examples!
 *************************************************************/

/* Fill-in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID           "TMPL6nWBqfn6T"
#define BLYNK_TEMPLATE_NAME         "Quickstart Template"
#define BLYNK_AUTH_TOKEN            "R8tv5SzKqKePUChX6YCykrb7MdwWFRtM"

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>

#define PIN_LED_RED 2
#define PIN_LED_GREEN 0
#define PIN_DHT 23
#define DHT_TYPE DHT22
#define PIN_PIR 22


#define BLYNK_LED_RED V0
#define BLYNK_LED_GREEN V1
#define BLYNK_TEMPERATURE V2
#define BLYNK_HUMIDITY V3
#define BLYNK_PIR_SENSOR V4

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Wokwi-GUEST";
char pass[] = "";

BlynkTimer timer;
DHT dht(PIN_DHT, DHT_TYPE);
int lastPirState = LOW;

// This function is called every time the Virtual Pin 0 state changes
BLYNK_WRITE(BLYNK_LED_RED)
{
  // Set incoming value from pin V0 to a variable
  int value = param.asInt();
  Serial.printf("V0 value: %d\r\n", value);
  digitalWrite(PIN_LED_RED, value);
}

BLYNK_WRITE(BLYNK_LED_GREEN)
{
  // Set incoming value from pin V0 to a variable
  int value = param.asInt();
  Serial.printf("V1 value: %d\r\n", value);
  digitalWrite(PIN_LED_GREEN, value);
}

// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED()
{
  // Request the latest state from the server
  Blynk.syncVirtual(BLYNK_LED_RED);
  Blynk.syncVirtual(BLYNK_LED_GREEN);
}

// This function reads the DHT22 every 2 seconds and sends the values
// to Virtual Pin V2 (temperature) and V3 (humidity).
void onTimerEvent()
{
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature(); // Celsius

  if (isnan(humidity) || isnan(temperature))
  {
    Serial.println("Failed to read from DHT22 sensor!");
    return;
  }

  Serial.printf("Temperature: %.1f C, Humidity: %.1f %%\r\n", temperature, humidity);

  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(BLYNK_TEMPERATURE, temperature);
  Blynk.virtualWrite(BLYNK_HUMIDITY, humidity);
}

// Polls the PIR sensor and reports state changes to Virtual Pin V4:
// 1 when motion is detected, 0 when the motion has stopped.
void onPirCheck()
{
  int state = digitalRead(PIN_PIR);
  if (state == lastPirState)
  {
    return;
  }
  lastPirState = state;

  if (state == HIGH)
  {
    Serial.println("Motion detected!");
  }
  else
  {
    Serial.println("Motion ended.");
  }
  Blynk.virtualWrite(BLYNK_PIR_SENSOR, state);
}

void setup()
{
  // Debug console
  pinMode(PIN_LED_RED, OUTPUT);
  pinMode(PIN_LED_GREEN, OUTPUT);
  pinMode(PIN_PIR, INPUT);
  Serial.begin(115200);
  dht.begin();

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  // You can also specify server:
  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, IPAddress(192,168,1,100), 8080);

  // Setup a function to be called every second
  timer.setInterval(2000L, onTimerEvent);
  // Check the PIR sensor 5 times per second
  timer.setInterval(200L, onPirCheck);
}

void loop()
{
  Blynk.run();
  timer.run();
  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!
}

