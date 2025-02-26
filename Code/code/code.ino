/*
 * A simple sketch that maps a single pin on the ESP32 to a single button on the controller
 
 A -> 1
 B -> 2
 X -> 3
 Y -> 4
 SELECT -> 7
 START -> 8
 LEFT JOYSTICK BUTTON -> 9
 RIGHT JOYSTICK BUTTON -> 10 
 */

#include <Arduino.h>
#include <BleGamepad.h> // https://github.com/lemmingDev/ESP32-BLE-Gamepad

#define LEFT_PIN D0 // Pin button is attached to
#define UP_PIN D1
#define RIGHT_PIN D2
#define DOWN_PIN D3

#define SELECT_PIN D4
#define START_PIN D5
#define LED_PIN D6

#define A_PIN D8
#define B_PIN D10 // pushed down pin
#define X_PIN D7
#define Y_PIN D9

BleGamepad bleGamepad("Retroswitch Joycon", "INOBOTICA", 100);
int idleTime = 0;
int idleThreshold = 60;

int previousButton1State = HIGH;
/*
Method to print the reason by which ESP32
has been awaken from sleep
*/
void print_wakeup_reason(){
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch(wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
  }
}

void going_to_deepsleep(){
  Serial.println("Going to sleep now");
  esp_deep_sleep_start();
}

void update_idleTime(){
  idleTime = int(millis()/1000);
  Serial.println("Key pressed");
}

void setup() {
    Serial.begin(115200);
    delay(1000); //Take some time to open up the Serial Monitor

    pinMode(LEFT_PIN, INPUT_PULLUP);
    pinMode(UP_PIN, INPUT_PULLUP);
    pinMode(RIGHT_PIN, INPUT_PULLUP);
    pinMode(DOWN_PIN, INPUT_PULLUP);

    pinMode(A_PIN, INPUT_PULLUP);
    pinMode(X_PIN, INPUT_PULLUP);
    pinMode(Y_PIN, INPUT_PULLUP);
    pinMode(B_PIN, INPUT);

    pinMode(SELECT_PIN, INPUT_PULLUP);
    pinMode(START_PIN, INPUT_PULLUP);
    pinMode(LED_PIN, OUTPUT);
    analogWrite(LED_PIN, 0);
    print_wakeup_reason();
    esp_deep_sleep_enable_gpio_wakeup(BIT(UP_PIN), ESP_GPIO_WAKEUP_GPIO_LOW);
    
    bleGamepad.begin();
}

void loop()
{
    int delta = int(millis()/1000) - idleTime;

    if(delta>idleThreshold){
      going_to_deepsleep();
    }

    if (bleGamepad.isConnected()){
      idleThreshold = 120;
      analogWrite(LED_PIN, 128);

      bleGamepad.setAxes(16384, 16384, 16384, 16384, 16384, 16384, 16384, 16384); // All axes centered

      if(!digitalRead(UP_PIN) && !digitalRead(RIGHT_PIN)){
        bleGamepad.setHat1(HAT_UP_RIGHT);
        update_idleTime();
      } else if(!digitalRead(UP_PIN) && !digitalRead(LEFT_PIN)){
        bleGamepad.setHat1(HAT_UP_LEFT);
        update_idleTime();
      } else if(!digitalRead(DOWN_PIN) && !digitalRead(RIGHT_PIN)){
        bleGamepad.setHat1(HAT_DOWN_RIGHT);
        update_idleTime();
      } else if(!digitalRead(DOWN_PIN) && !digitalRead(LEFT_PIN)){
        bleGamepad.setHat1(HAT_DOWN_LEFT);
        update_idleTime();
      } else if(!digitalRead(UP_PIN)){
        bleGamepad.setHat1(HAT_UP);
        update_idleTime();
      } else if(!digitalRead(DOWN_PIN)){
        bleGamepad.setHat1(HAT_DOWN);
        update_idleTime();
      } else if(!digitalRead(LEFT_PIN)){
        bleGamepad.setHat1(HAT_LEFT);
        update_idleTime();
      } else if(!digitalRead(RIGHT_PIN)){
        bleGamepad.setHat1(HAT_RIGHT);
        update_idleTime();
      } else {
        bleGamepad.setHat1(HAT_CENTERED);
      }

      if(!digitalRead(A_PIN) && digitalRead(B_PIN)){
        bleGamepad.release(BUTTON_1);
        bleGamepad.release(BUTTON_2);

        bleGamepad.press(BUTTON_9);
        bleGamepad.press(BUTTON_10);
        update_idleTime();
      } else {
        bleGamepad.release(BUTTON_9);
        bleGamepad.release(BUTTON_10);
        
        // A button
        if(!digitalRead(A_PIN)){
          bleGamepad.press(BUTTON_2);
          update_idleTime();
        } else {
          bleGamepad.release(BUTTON_2);          
        }

        // B button
        if(digitalRead(B_PIN)){
          bleGamepad.press(BUTTON_1);
          update_idleTime();
        } else {
          bleGamepad.release(BUTTON_1);
        }
      }
      

      // X button
      if(!digitalRead(X_PIN)){
        bleGamepad.press(BUTTON_3);
        update_idleTime();
      } else {
        bleGamepad.release(BUTTON_3);
      }

      // Y button
      if(!digitalRead(Y_PIN)){
        bleGamepad.press(BUTTON_4);
        update_idleTime();
      } else {
        bleGamepad.release(BUTTON_4);
      }

      // Select
      if(!digitalRead(SELECT_PIN)){
        bleGamepad.press(BUTTON_7);
        update_idleTime();
      } else {
        bleGamepad.release(BUTTON_7);
      }

      // Start
      if(!digitalRead(START_PIN)){
        bleGamepad.press(BUTTON_8);
        update_idleTime();
      } else {
        bleGamepad.release(BUTTON_8);
      }
    } else {
      idleThreshold = 60;
      int led_state = int(millis()/1000)%2;
      analogWrite(LED_PIN, led_state*128);
    }
}