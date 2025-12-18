// Lord of Robots - LoR Core V3 - AUG 3 2025
// Sample MiniBot Control Program with Bluetooth GamePad Interface

#include <Bluepad32.h>
#include "esp_task_wdt.h"
#include <ESP32Servo.h>
#include <FastLED.h>

const uint8_t AUX_PINS[9] = { 0, 5, 18, 23, 19, 22, 21, 1, 3 };
const uint8_t IO_PINS[13] = { 0, 32, 25, 26, 27, 14, 12, 13, 15, 2, 4, 22, 21 };

#define User_BTN_A 35
#define User_BTN_B 39
#define User_BTN_C 38
#define User_BTN_D 37
#define User_SW 36

#define VIN_SENSE 34
#define VOLT_SLOPE 0.0063492
#define VOLT_OFFSET 1.079

#define LED_PIN 33
#define LED_COUNT 4
#define BRIGHTNESS 255
#define COLOR_ORDER GRB
#define CHIPSET WS2812B
CRGB leds[LED_COUNT];
uint8_t rainbowHue = 0;

#define WDT_TIMEOUT 3
Servo MotorOutput[13];

ControllerPtr myController = nullptr;

void INIT_InternalFeatures() {
  Serial.begin(115200);
  esp_task_wdt_init(WDT_TIMEOUT, true);
  esp_task_wdt_add(NULL);

  pinMode(User_BTN_A, INPUT);
  pinMode(User_BTN_B, INPUT);
  pinMode(User_BTN_C, INPUT);
  pinMode(User_BTN_D, INPUT);
  pinMode(User_SW, INPUT);
  pinMode(VIN_SENSE, INPUT);

  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, LED_COUNT);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  FastLED.show();
  delay(100);
}

void onConnectedController(ControllerPtr ctl) {
  if (myController == nullptr) {
    Serial.println("! GamePad connected !");
    myController = ctl;
    ctl->playDualRumble(0x00, 0xc0, 0xc0, 0xc0);
    ctl->setColorLED(0, 255, 0);
    BP32.enableNewBluetoothConnections(false);
    fill_solid(leds, LED_COUNT, CRGB(0, 255, 0));
    FastLED.show();
    delay(500);
  } else Serial.println("Another controller tried to connect but is rejected");
}

void onDisconnectedController(ControllerPtr ctl) {
  if (myController == ctl) {
    Serial.println("! GamePad disconnected !");
    myController = nullptr;
    fill_solid(leds, LED_COUNT, CRGB(255, 0, 0));
    FastLED.show();
    delay(1000);
  }
}

static unsigned long lastBatteryUpdate = 0;
void GamePad_BatteryMonitor() {
  if (millis() - lastBatteryUpdate > 1000) {
    int battery = myController->battery();
    if (battery == 0) myController->setColorLED(255, 0, 0);
    else if (battery <= 64) {
      myController->setColorLED(255, 0, 0);
      Serial.println("! GamePad Low Battery !");
      myController->playDualRumble(0x00, 0xc0, 0xc0, 0xc0);
      fill_solid(leds, LED_COUNT, CRGB(255, 0, 0));
      FastLED.show();
      delay(100);
    } else if (battery <= 128) myController->setColorLED(255, 255, 0);
    else myController->setColorLED(0, 255, 0);
    lastBatteryUpdate = millis();
  }
}

void INIT_BluetoothGamepad_PairMode() {
  if (!digitalRead(User_BTN_A) && !digitalRead(User_BTN_D)) {
    BP32.forgetBluetoothKeys();
    Serial.println("Gamepad Unpaired!");
    BP32.enableNewBluetoothConnections(true);
    BP32.setup(&onConnectedController, &onDisconnectedController);
    while (!(myController && myController->isConnected())) {
      esp_task_wdt_reset();
      fill_solid(leds, LED_COUNT, CRGB(0, 0, 255));
      FastLED.show();
      delay(100);
      fill_solid(leds, LED_COUNT, CRGB(255, 255, 255));
      FastLED.show();
      delay(100);
      BP32.update();
    }
    BP32.enableNewBluetoothConnections(false);
  } else BP32.setup(&onConnectedController, &onDisconnectedController);
  BP32.enableVirtualDevice(false);
}

enum MotorType { MG90_CR, MG90_Degree, N20Plus, STD_SERVO, Victor_SPX, Talon_SRX, SPARK_MAX, CUSTOM };
struct MotorTypeConfig {
  MotorType type;
  float pwmFreq;
  int minPulseUs;
  int maxPulseUs;
  float inputMin;
  float inputMax;
};
MotorTypeConfig motorTypeConfigs[] = {
  { N20Plus, 50, 1000, 2000, -1, 1 }, // This sets up the n20 motors
  { MG90_Degree, 50, 500, 2500, 1, 180 }, // This sets up the servo motors
};

void ConfigureMotorOutput(uint8_t slot, MotorType motorType, int startupPositionDeg = 90) {
  float pwmFreq = 50;
  int minPulseUs = 1000;
  int maxPulseUs = 2000;
  for (auto &cfg : motorTypeConfigs)
    if (cfg.type == motorType) {
      pwmFreq = cfg.pwmFreq;
      minPulseUs = cfg.minPulseUs;
      maxPulseUs = cfg.maxPulseUs;
      break;
    }

  uint8_t pin = IO_PINS[slot];
  pinMode(pin, OUTPUT);
  MotorOutput[slot].setPeriodHertz(pwmFreq);
  MotorOutput[slot].attach(pin, minPulseUs, maxPulseUs);
  MotorOutput[slot].writeMicroseconds(1500);
  Serial.printf("Motor slot %d configured on pin %d as type %d\n", slot, pin, motorType);
}

void INIT_LoRcore() {
  INIT_InternalFeatures();
  INIT_BluetoothGamepad_PairMode();
}

void setup() {
  INIT_LoRcore();
  Serial.println("Motors Startup");

  ConfigureMotorOutput(1, N20Plus, 90);
  ConfigureMotorOutput(2, N20Plus, 90);
  ConfigureMotorOutput(3, N20Plus, 90);
  ConfigureMotorOutput(4, N20Plus, 90);
  ConfigureMotorOutput(5, MG90_Degree, 90);
  ConfigureMotorOutput(6, MG90_Degree, 90);
  

  Serial.println("LoRcore V3 System Ready!");
}

// ---------- Flipping Global Variables ----------- //

// Define servo motor pins
const int LEFT_SERVO = 5;
const int RIGHT_SERVO = 6;

// Flipping Logic
bool aFlipPressedPrev = false; 
bool flipActivate = false;

void loop() {
  esp_task_wdt_reset();
  BP32.update();

  if (myController && myController->isConnected()) {
    GamePad_BatteryMonitor();

    // ---------- ARCADE DRIVE LOGIC ----------
    int forward = -myController->axisY(); // Left joystick Y = forward/back
    int turn    = myController->axisRX();  // Right joystick X = turn

    // Flipping mechanism logic
    bool aFlipPressed = myController->r1();
    bool bFlipPressed = myController->l1();

    // Deadzone
    if (abs(forward) < 40) forward = 0;
    if (abs(turn) < 40) turn = 0;

    // Combine for left/right wheels
    int currentLeft  = forward + turn;
    int currentRight = forward - turn;

    // Constrain values
    currentLeft  = constrain(currentLeft, -512, 512);
    currentRight = constrain(currentRight, -512, 512);

    // Map to servo angles
    int MappedLeft  = map(currentLeft,  -512, 512, 0, 180);
    int MappedRight = map(currentRight, -512, 512, 0, 180);

    // Flipping values for the flpper
    const int SERVO_FLIP = 180;
    const int SERVO_HOME = 90;

    // Invert left motors to match right side direction
    MappedLeft = 180 - MappedLeft;

    // Apply to motors
    MotorOutput[1].write(MappedLeft);
    MotorOutput[2].write(MappedLeft);
    MotorOutput[3].write(MappedRight);
    MotorOutput[4].write(MappedRight);

    // Rainbow LED animation
    fill_rainbow(leds, LED_COUNT, rainbowHue++, 20);
    FastLED.show();
    delay(50);

    // ---------- Flipping Mechanism Logic ----------

    //////////////////////////////////////////////
    //      Flipping mechanism that stays       //
    //////////////////////////////////////////////
    if (aFlipPressed && !aFlipPressedPrev) {
        flipActivate = !flipActivate; // toggle the flip
    }
    aFlipPressedPrev = aFlipPressed;

    if (flipActivate) {
        MotorOutput[LEFT_SERVO].write(SERVO_FLIP);
        MotorOutput[RIGHT_SERVO].write(0); 
    } else {
        MotorOutput[LEFT_SERVO].write(SERVO_HOME);
        MotorOutput[RIGHT_SERVO].write(SERVO_HOME);
    }

    //////////////////////////////////////////////
    //      Instant Flipping mechanism          //
    //////////////////////////////////////////////    
    if (bFlipPressed){
        MotorOutput[LEFT_SERVO].write(SERVO_FLIP);
        MotorOutput[RIGHT_SERVO].write(0); 
        delay(1500);
        MotorOutput[LEFT_SERVO].write(SERVO_HOME);
        MotorOutput[RIGHT_SERVO].write(SERVO_HOME);        
    }

  } else {
    for (int i = 1; i <= 4; i++) MotorOutput[i].write(90); // stop motors
    fill_solid(leds, LED_COUNT, CRGB(0, 80, 255));
    FastLED.show();
  }

}