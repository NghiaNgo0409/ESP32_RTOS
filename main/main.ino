#include <WiFi.h>
#include "Adafruit_NeoPixel.h"
#include "DHT20.h"
#include "LiquidCrystal_I2C.h"
#include <ThingsBoard.h>
#include <Arduino_MQTT_Client.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>

#define WIFI_USERNAME "RD-SEAI_2.4G"
#define WIFI_PASS ""

#define TB_SERVER "demo.thingsboard.io"
#define TOKEN "U3zf9sqRoyPGxcFQRkph"

constexpr uint16_t MAX_MESSAGE_SIZE = 128U;

WiFiClient espClient;
PubSubClient client(espClient);
Arduino_MQTT_Client mqttClient(espClient);
ThingsBoard tb(mqttClient, MAX_MESSAGE_SIZE);

// Define your tasks here
void TaskBlink(void *pvParameters);
void TaskTemperatureHumidity(void *pvParameters);
void TaskSoilMoistureAndRelay(void *pvParameters);
void TaskLightAndLED(void *pvParameters);
void TaskPrintLCD(void *pvParameters);
void sendDataToThingsBoard(float temp, int hum, int soil, int light);
void sendSoilMoistureToThingsBoard(int soil);
void sendLightToThingsBoard(int light);
void onTelemetryReceived(char* topic, byte* payload, unsigned int length);
// RPC_Response processGetValue(const RPC_Data &data);
// void processTime(const JsonVariantConst& data);

// const std::array<RPC_Callback, 1U> callbacks = {
//   RPC_Callback{ "setLedStatus", processSetLedStatus }
// };

void connectToWifi();
void connectToThingsBoard();

//Define your components here
Adafruit_NeoPixel pixels3(4, D5, NEO_GRB + NEO_KHZ800);
DHT20 dht20;
LiquidCrystal_I2C lcd(33,16,2);

// bool subscribed = false; // Indicates if RPC subscription is done


void setup() {

  // Initialize serial communication at 115200 bits per second:
  Serial.begin(115200);
  dht20.begin();
  lcd.begin(); 
  pixels3.begin();
  xTaskCreate( TaskBlink, "Task Blink" ,2048  ,NULL  ,2 , NULL);
  xTaskCreate( TaskTemperatureHumidity, "Task Temperature" ,2048  ,NULL  ,2 , NULL);
  xTaskCreate( TaskSoilMoistureAndRelay, "Task Soil & Moisture" ,2048  ,NULL  ,2 , NULL);
  xTaskCreate( TaskLightAndLED, "Task Light LED" ,2048  ,NULL  ,2 , NULL);
  xTaskCreate( TaskPrintLCD, "Task LCD" ,2048  ,NULL  ,2 , NULL);
  
  //Now the task scheduler is automatically started.
  Serial.printf("Basic Multi Threading Arduino Example\n");
  connectToWifi();
  connectToThingsBoard();

  // Configure MQTT client
  client.setServer(TB_SERVER, 1883);
  client.setCallback(onTelemetryReceived);

  // Connect to ThingsBoard MQTT broker
  if (client.connect("ESP32_Client", TOKEN, NULL)) {
    Serial.println("Connected to ThingsBoard 123");

    // Subscribe to telemetry topic
    client.subscribe("v1/devices/me/telemetry");
    Serial.println("After subscribe");
  } else {
    Serial.println("Failed to connect to ThingsBoard");
  }
}

void loop() {
  if (!tb.connected()) {
    connectToThingsBoard();
  }

  delay(3000);

  // Reconnect if the connection is lost
  if (!client.connected()) {
    if (client.connect("ESP32_Client", TOKEN, NULL)) {
      client.subscribe("v1/devices/me/telemetry");
    } else {
      delay(5000);  // Wait and retry
    }
  }
  client.loop();

    // // Subscribe to RPC callbacks
    // Serial.println("Subscribing for RPC...");
    // if (!tb.RPC_Subscribe(callbacks.cbegin(), callbacks.cend())) {
    //   Serial.println("Failed to subscribe for RPC");
    //   return;
    // }

    // // Request the current time if not already subscribed
    // if (!subscribed) {
    // Serial.println("Requesting RPC...");
    // RPC_Request_Callback timeRequestCallback("getCurrentTime", processTime);
    // if (!tb.RPC_Request(timeRequestCallback)) {
    //   Serial.println("Failed to request for RPC");
    //   return;
    // }
    // Serial.println("Request done");
    // subscribed = true;

  tb.loop();
}

void connectToWifi() {
  Serial.println("Connecting to WiFi...");
  int attempts = 0;
  WiFi.begin(WIFI_USERNAME, WIFI_PASS);
  
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("\nFailed to connect to WiFi.");
  } else {
    Serial.println("\nConnected to WiFi");
  }
}

void connectToThingsBoard() {
  if (!tb.connected()) {
    Serial.println("Connecting to ThingsBoard server");
    
    if (!tb.connect(TB_SERVER, TOKEN)) {
      Serial.println("Failed to connect to ThingsBoard");
    } else {
      Serial.println("Connected to ThingsBoard zzz");
    }
  }
}

// RPC_Response processGetValue(const RPC_Data &data) {
//   // Process the RPC request to change the LED state
//   int dataInt = data;
//   ledState = dataInt == 1;  // Update the LED state based on the received data
//   Serial.println(ledState ? "LED ON" : "LED OFF");
//   return RPC_Response("newStatus", dataInt);  // Respond with the new status
// }

// void processTime(const JsonVariantConst& data) {
//   // Process the RPC response containing the current time
//   Serial.print("Received time from ThingsBoard: ");
//   Serial.println(data["time"].as<String>());
// }

void onTelemetryReceived(char* topic, byte* payload, unsigned int length) {
  Serial.print("Received telemetry data on topic: ");
  Serial.println(topic);

  Serial.print("Payload: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

  // if (data["humidity"]) {
  //   float humidity = data["humidity"].as<float>();
  //   Serial.print("Humidity: ");
  //   Serial.println(humidity);
  // }


/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/



void TaskBlink(void *pvParameters) {  // This is a task.
  //uint32_t blink_delay = *((uint32_t *)pvParameters);

  // initialize digital LED_BUILTIN on pin 13 as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  

  while(1) {                          
    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED ON
    delay(2000);
    digitalWrite(LED_BUILTIN, LOW);  // turn the LED OFF
    delay(2000);
  }
}


void TaskTemperatureHumidity(void *pvParameters) {  // This is a task.
  //uint32_t blink_delay = *((uint32_t *)pvParameters);

  while(1) {                          
    // Serial.println("Task Temperature and Humidity");

    dht20.read();
    // Serial.println(dht20.getTemperature());
    // Serial.println(dht20.getHumidity());

    delay(5000);
  }
}

void TaskSoilMoistureAndRelay(void *pvParameters) {  // This is a task.

  pinMode(D3, OUTPUT);

  while(1) {                          
    // Serial.println("Task Soild and Relay");
    // Serial.println(analogRead(A0));
    
    if(analogRead(A0) > 50){
      digitalWrite(D3, LOW);
    }
    if(analogRead(A0) < 30){
      digitalWrite(D3, HIGH);
    }

    // sendSoilMoistureToThingsBoard(analogRead(A0));
    delay(1000);
                
  }
}

void TaskLightAndLED(void *pvParameters) {  // This is a task.

  while(1) {                          
    // Serial.println("Task Light and LED");
    // Serial.println(analogRead(A1));
    
    if(analogRead(A1) < 350){
      pixels3.setPixelColor(0, pixels3.Color(255,0,0));
      pixels3.setPixelColor(1, pixels3.Color(255,0,0));
      pixels3.setPixelColor(2, pixels3.Color(255,0,0));
      pixels3.setPixelColor(3, pixels3.Color(255,0,0));
      pixels3.show();
    }
    if(analogRead(A1) > 550){
      pixels3.setPixelColor(0, pixels3.Color(0,0,0));
      pixels3.setPixelColor(1, pixels3.Color(0,0,0));
      pixels3.setPixelColor(2, pixels3.Color(0,0,0));
      pixels3.setPixelColor(3, pixels3.Color(0,0,0));
      pixels3.show();
    }

    // sendLightToThingsBoard(analogRead(A1));
    delay(1000);
  }
     
}

void TaskPrintLCD(void *pvParameters) {
  
  while(1) {                          
    // Serial.println("Task LCD");

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("T: ");
    lcd.print(dht20.getTemperature());
    lcd.print(" S: ");
    lcd.print(analogRead(A0));
    lcd.setCursor(0, 1);
    lcd.print("H: ");
    lcd.print(dht20.getHumidity());
    lcd.print(" L: ");
    lcd.print(analogRead(A1));
    
    sendDataToThingsBoard(dht20.getTemperature(), dht20.getHumidity(), analogRead(A0), analogRead(A1));
    
    delay(5000);
  }
}

void sendDataToThingsBoard(float temp, int hum, int soil, int light) {
  StaticJsonDocument<200> jsonDoc;

  jsonDoc["temperature"] = temp;
  jsonDoc["humidity"] = hum;
  jsonDoc["soil"] = soil;
  jsonDoc["light"] = light;
  // String jsonData = "{\"temperature\":" + String(temp) + ", \"humidity\":" + String(hum) + "}";
  size_t jsonSize = measureJson(jsonDoc);
  // tb.sendTelemetryJson(jsonData.c_str(), sizeof(jsonData.c_str()));
  tb.sendTelemetryJson(jsonDoc, jsonSize);
  Serial.println("Data sent");
}