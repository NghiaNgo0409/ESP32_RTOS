/* Basic Multi Threading Arduino Example
   This example code is in the Public Domain (or CC0 licensed, at your option.)
   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
// Please read file README.md in the folder containing this example.

#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

#define ANALOG_INPUT_PIN 1


// Define two tasks for Blink & AnalogRead.
void TaskBlink(void *pvParameters);
void TaskBlinkD3(void *pvParameters);
void TaskBlinkD5(void *pvParameters);
void TaskBlinkD7(void *pvParameters);
void TaskAnalogRead(void *pvParameters);
void TaskHumidityAndLCD(void *pvParameters);


TaskHandle_t analog_read_task_handle;  // You can (don't have to) use this to be able to manipulate a task from somewhere else.

// The setup function runs once when you press reset or power on the board.
void setup() {

  // Initialize serial communication at 115200 bits per second:
  Serial.begin(115200);
  // Set up two tasks to run independently.
  uint32_t blink_delay = 1000;  // Delay between changing state on LED pin
  
  xTaskCreate( TaskBlink, "Task Blink" ,2048  ,NULL  ,2 , NULL);
  xTaskCreate( TaskBlinkD3, "Task Blink" ,2048  ,NULL  ,2 , NULL);
  xTaskCreate( TaskBlinkD5, "Task Blink" ,2048  ,NULL  ,2 , NULL);
  xTaskCreate( TaskBlinkD7, "Task Blink" ,2048  ,NULL  ,2 , NULL);
  //xTaskCreatePinnedToCore(TaskBlink, "Toggle 1", 1024,NULL, 1,NULL, 0);
  //xTaskCreatePinnedToCore(TaskHumidityAndLCD, "Toggle 2", 1024,NULL, 1,NULL, 1);

  //TaskCreate(TaskHumidityAndLCD, "DHT LCD", 2048, NULL, 1, NULL);

  // This variant of task creation can also specify on which core it will be run (only relevant for multi-core ESPs)
  
  /*
  xTaskCreatePinnedToCore(
    TaskAnalogRead, "Analog Read", 2048  // Stack size
    ,
    NULL  // When no parameter is used, simply pass NULL
    ,
    1  // Priority
    ,
    &analog_read_task_handle  // With task handle we will be able to manipulate with this task.
    ,
    0  // Core on which the task will run
  );

  xTaskCreatePinnedToCore(
    TaskBlink, "Blink", 2048  // Stack size
    ,
    NULL  // When no parameter is used, simply pass NULL
    ,
    1  // Priority
    ,
    NULL  // With task handle we will be able to manipulate with this task.
    ,
    1  // Core on which the task will run
  );
  xTaskCreatePinnedToCore(
    TaskBlinkD3, "Blink3", 2048  // Stack size
    ,
    NULL  // When no parameter is used, simply pass NULL
    ,
    1  // Priority
    ,
    NULL  // With task handle we will be able to manipulate with this task.
    ,
    1  // Core on which the task will run
  );
  */
  Serial.printf("Basic Multi Threading Arduino Example\n");
  
  // Now the task scheduler, which takes over control of scheduling individual tasks, is automatically started.
}

void loop() {
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

void TaskHumidityAndLCD(void *pvParameters) {  // This is a task.
  (void)pvParameters;
  
  for (;;) {
    // read the input on analog pin:
    int sensorValue = analogRead(A1);
    // print out the value you read:
    Serial.print("TaskAnalogRead: ");
    Serial.println(sensorValue);
    delay(5000);  // 100ms delay
  }
}


void TaskBlink(void *pvParameters) {  // This is a task.
  //uint32_t blink_delay = *((uint32_t *)pvParameters);

  /*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  If you want to know what pin the on-board LED is connected to on your ESP32 model, check
  the Technical Specs of your board.
*/

  // initialize digital LED_BUILTIN on pin 13 as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  

  for (;;) {                          // A Task shall never return or exit.
    //Serial.println("TaskBlink: LED_ON");
    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
    
    // arduino-esp32 has FreeRTOS configured to have a tick-rate of 1000Hz and portTICK_PERIOD_MS
    // refers to how many milliseconds the period between each ticks is, ie. 1ms.
    delay(200);
    //Serial.println("TaskBlink: LED_OFF");
    digitalWrite(LED_BUILTIN, LOW);  // turn the LED off by making the voltage LOW
   
    delay(200);
  }
}


void TaskBlinkD3(void *pvParameters) {  // This is a task.
  //uint32_t blink_delay = *((uint32_t *)pvParameters);

  /*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  If you want to know what pin the on-board LED is connected to on your ESP32 model, check
  the Technical Specs of your board.
*/

  // initialize digital LED_BUILTIN on pin 13 as an output.
  
  pinMode(D3, OUTPUT);

  for (;;) {                          // A Task shall never return or exit.
    //Serial.println("TaskBlink: LED_ON");
    
    digitalWrite(D3, HIGH);
    // arduino-esp32 has FreeRTOS configured to have a tick-rate of 1000Hz and portTICK_PERIOD_MS
    // refers to how many milliseconds the period between each ticks is, ie. 1ms.
    delay(500);
    //Serial.println("TaskBlink: LED_OFF");
    
    digitalWrite(D3, LOW);
    delay(500);
  }
}


void TaskBlinkD5(void *pvParameters) {  // This is a task.
  //uint32_t blink_delay = *((uint32_t *)pvParameters);

  /*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  If you want to know what pin the on-board LED is connected to on your ESP32 model, check
  the Technical Specs of your board.
*/

  // initialize digital LED_BUILTIN on pin 13 as an output.
  
  pinMode(D5, OUTPUT);

  for (;;) {                          // A Task shall never return or exit.
    //Serial.println("TaskBlink: LED_ON");
    
    digitalWrite(D5, HIGH);
    // arduino-esp32 has FreeRTOS configured to have a tick-rate of 1000Hz and portTICK_PERIOD_MS
    // refers to how many milliseconds the period between each ticks is, ie. 1ms.
    delay(1000);
    //Serial.println("TaskBlink: LED_OFF");
    
    digitalWrite(D5, LOW);
    delay(1000);
  }
}


void TaskBlinkD7(void *pvParameters) {  // This is a task.
  //uint32_t blink_delay = *((uint32_t *)pvParameters);

  /*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  If you want to know what pin the on-board LED is connected to on your ESP32 model, check
  the Technical Specs of your board.
*/

  // initialize digital LED_BUILTIN on pin 13 as an output.
  
  pinMode(D7, OUTPUT);

  for (;;) {                          // A Task shall never return or exit.
    //Serial.println("TaskBlink: LED_ON");
    
    digitalWrite(D7, HIGH);
    // arduino-esp32 has FreeRTOS configured to have a tick-rate of 1000Hz and portTICK_PERIOD_MS
    // refers to how many milliseconds the period between each ticks is, ie. 1ms.
    delay(5000);
    //Serial.println("TaskBlink: LED_OFF");
    
    digitalWrite(D7, LOW);
    delay(5000);
  }
}


void TaskAnalogRead(void *pvParameters) {  // This is a task.
  (void)pvParameters;
  // Check if the given analog pin is usable - if not - delete this task
  if (digitalPinToAnalogChannel(ANALOG_INPUT_PIN) == -1) {
    Serial.printf("TaskAnalogRead cannot work because the given pin %d cannot be used for ADC - the task will delete itself.\n", ANALOG_INPUT_PIN);
    analog_read_task_handle = NULL;  // Prevent calling vTaskDelete on non-existing task
    vTaskDelete(NULL);               // Delete this task
  }

  /*
  AnalogReadSerial
  Reads an analog input on pin A3, prints the result to the serial monitor.
  Graphical representation is available using serial plotter (Tools > Serial Plotter menu)
  Attach the center pin of a potentiometer to pin A3, and the outside pins to +5V and ground.

  This example code is in the public domain.
*/

  for (;;) {
    // read the input on analog pin:
    int sensorValue = analogRead(A1);
    // print out the value you read:
    Serial.print("TaskAnalogRead: ");
    Serial.println(sensorValue);
    delay(5000);  // 100ms delay
  }
}
