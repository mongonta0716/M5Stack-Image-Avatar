// Copyright(c) 2020 Takao Akaki

// Please refer to the following three files for configuration information.
// src/avatarconfig.h
// src/colorpalette.h
// src/optionconfig.h

// ---------- Options -------------------------
#define USE_TTS     // AquesTalk is necessary.(https://www.a-quest.com/products/aquestalk_esp32.html)
#define USE_MIC     // M5Go Bottom's MIC is necessary.
//#define USE_WIFI    // M5StackFire is required when using 8bit color.Because Gray and Basic don't have enough memory.
// ---------- Options -------------------------

// ----- for DEBUG -----
// #define DEBUG
// ---------------------

#include <M5StackUpdater.h>
#include <LovyanGFX.hpp>
#include <SD.h>
#include <Wire.h>
#include "src/M5StackImageAvatar.hpp"
#include "src/optionconfig.h"

static LGFX tft;
ImageAvatar *avatar;

#ifdef USE_WIFI
  #include <WiFi.h>
#endif

#ifdef USE_TTS
  #include "src/AquesTalkTTS.h"
  #include "driver/dac.h"
  static int  serifno = 0;
#endif


#ifdef USE_MIC
  #include <arduinoFFT.h>
  arduinoFFT FFT = arduinoFFT();
  // ---------- Mic sampling ----------
  #define SIGNAL_LENGTH 512
  #define SAMPLINGFREQUENCY 40000
  #define SAMPLING_TIME_US     ( 1000000UL/SAMPLINGFREQUENCY )
  double micOffset = 2100;
  double adcBuffer[SIGNAL_LENGTH];
  double vImag[SIGNAL_LENGTH];
  double AdcMeanValue = 0;
  int   maxAmplitudeDB = 0;
  int lastDB = 0;
#endif

// Start----- Avatar static variables ----------
static uint8_t expression = NORMAL;
static float   last_mouth_ratio = 0.0;
static bool    eyeball_direction = RIGHT;
static int     eyeballX = 0;
static int     eyeballY = 0;
static bool    isTTS = false;

// Multi Threads
TaskHandle_t drawTaskHandle;
TaskHandle_t blinkTaskHandle;
TaskHandle_t breathTaskHandle;
TaskHandle_t lipsyncTaskHandle;
SemaphoreHandle_t xMutex = NULL;

// Start----- Task functions ----------
void drawLoop(void *args) {
  BaseType_t xStatus;
  const TickType_t xTicksToWait = 1000UL;
  xSemaphoreGive(xMutex);
  for(;;) {
    xStatus = xSemaphoreTake(xMutex, xTicksToWait);
    if (xStatus == pdTRUE) {
      avatar->drawAll();
    }
    xSemaphoreGive(xMutex);
    vTaskDelay(33);
  }
}

void breath(void *args) {
  uint32_t c = 0;
  for(;;) {
    c = c + 1 % 100;
    float f = sin(c) * 2;
    avatar->setBreath(f);
    vTaskDelay(1000);
  }
}

void blink(void *args) {
  for(;;) {
    // まぶたの動きをリアルにしたいのでfor文を使用
    for(float f=0.0; f<=1; f=f+0.1) {
        avatar->setBlink(f);
        delay(10);
    }
    vTaskDelay(2000 + 100 * random(20));
    for(float f=1.0; f>=0; f=f-0.1) {
        avatar->setBlink(f);
        delay(10);
    }
    vTaskDelay(300 + 10 * random(20));
  }
}

void lipsync(void *args) {
  for(;;) {
    if (isTTS) {
#ifdef USE_TTS
      // AquesTalkTTSを利用する場合の口の動き
      int level = TTS.getLevel();
      float f = level / 12000.0;
      float openratio = min(1.0, last_mouth_ratio + f / 2.0);
      last_mouth_ratio = f;
      avatar->setMouthOpen(f);      
#endif
      vTaskDelay(50);
    } else {
#ifdef USE_MIC
      int increase = maxAmplitudeDB - lastDB; // db
    
      Serial.printf("increase:%d\n", increase);
      double d = (increase / 10.0); 
      Serial.printf("ratio:%lf\n", d);
      float ratio = avatar->getMouthOpen();
      ratio = ratio + d;
      if (ratio >= 1.0) {
        ratio = 1.0;
      } else if (ratio < 0.0) {
        ratio = 0.0;
      }
      if (maxAmplitudeDB <=19) {
        ratio = 0.0;
      }
      avatar->setMouthOpen(ratio);
      lastDB = maxAmplitudeDB;
#else
      // マイク未使用時の動きをカスタマイズしたい場合は処理を記述する。
      avatar->setMouthOpen(0.0); 

#endif
      vTaskDelay(33);
    }
  }
}

void startThreads() {
  printDebug("----- startThreads -----");
  xMutex = xSemaphoreCreateMutex();
  if (xMutex != NULL) {
    xTaskCreateUniversal(drawLoop,
                         "drawLoop",
                         4096,
                         NULL,
                         5,
                         &drawTaskHandle,
                         1);// tskNO_AFFINITY); // Core 1を指定しないと不安定
    xTaskCreateUniversal(breath,
                         "breath",
                         2048,
                         NULL,
                         6,
                         &breathTaskHandle,
                         tskNO_AFFINITY);
    xTaskCreateUniversal(blink,
                         "blink",
                         2048,
                         NULL,
                         7,
                         &blinkTaskHandle,
                         tskNO_AFFINITY);
    xTaskCreateUniversal(lipsync,
                         "lipsync",
                         2048,
                         NULL,
                         8,
                         &lipsyncTaskHandle,
                         tskNO_AFFINITY);
  }
  printDebug("----- End of startThreads -----");
}

void suspendAllTasks() {
    vTaskSuspend(drawTaskHandle);
    vTaskSuspend(breathTaskHandle);
    vTaskSuspend(blinkTaskHandle);
    vTaskSuspend(lipsyncTaskHandle);
}

void resumeAllTasks() {
    vTaskResume(drawTaskHandle);
    vTaskResume(breathTaskHandle);
    vTaskResume(blinkTaskHandle);
    vTaskResume(lipsyncTaskHandle);
}
// End------- xTask functions ----------


// Start----- setup & loop ----------
// for M5Stack 
#define BUTTON_A_PIN 39
#define BUTTON_B_PIN 38
#define BUTTON_C_PIN 37
#define FIRE_MICROPHONE_PIN 34
#define FIRE_SPEAKER_PIN 25

void setup() {

  pinMode(BUTTON_A_PIN, INPUT_PULLUP);
  pinMode(BUTTON_B_PIN, INPUT_PULLUP);
  pinMode(BUTTON_C_PIN, INPUT_PULLUP);
  dacWrite(FIRE_SPEAKER_PIN, 0); // make sure that the speaker is quite
  pinMode(FIRE_MICROPHONE_PIN, INPUT_PULLUP);

#ifdef _SD_H_
  SD.begin(4, SPI, 20000000);
#endif

#ifdef __M5STACKUPDATER_H
  Wire.begin();
  if(digitalRead(BUTTON_A_PIN) == 0) {
    Serial.println("Will Load menu binary");
    updateFromFS(SD);
    ESP.restart();
  }
#endif
  Serial.begin(115200);

  while (!Serial);
  printDebug(""); printDebug("");
  printDebug("M5Stack Image Avatar Start!");

#ifdef USE_WIFI
  printDebug("WiFi connect");
  if (!strcmp(ssid,"")) {
    Serial.println("Connecting to the previous WiFiAP.");
    Serial.println("If you don't connect to WiFi. Check src/WiFiconfig.h");
    WiFi.begin();
  } else {
    Serial.printf("Connentng to SSID:%s\n", ssid);
    WiFi.begin(ssid, pass);
  }
  while (WiFi.status() != WL_CONNECTED) {
    printDebug(".");
    delay(500);
  }
  printDebug("WiFi connected");
#endif

  
  printDebug("---------- TFT Init ----------");
  // tft hardware init 
  tft.init();
  tft.setRotation(1);
  tft.setBrightness(25);
  tft.fillScreen(0x000000U);
  delay(100);
#ifdef USE_TTS
  int iret = TTS.create(NULL);
#endif
  avatar = new ImageAvatar(&tft, NORMAL);
  startThreads();
  printDebug("----- End of Setup -----");
}

void loop() {

#ifdef USE_MIC
  printDebug("----- Start FFT -----");
  int n;
  uint32_t nextTime = 0;
  for (n = 1; n < SIGNAL_LENGTH; n++)
  {
    double v = analogRead( FIRE_MICROPHONE_PIN );
    AdcMeanValue += (v - AdcMeanValue) * 0.001;
    adcBuffer[n] = v - AdcMeanValue;
    // wait for next sample
    while (micros() < nextTime);
    nextTime = micros() + SAMPLING_TIME_US;
  }

  FFT.Windowing(adcBuffer, SIGNAL_LENGTH, FFT_WIN_TYP_HAMMING, FFT_FORWARD);  /* Weigh data */
  FFT.Compute(adcBuffer, vImag, SIGNAL_LENGTH, FFT_FORWARD); /* Compute FFT */
  FFT.ComplexToMagnitude(adcBuffer, vImag, SIGNAL_LENGTH); /* Compute magnitudes */
  //int x = FFT.MajorPeak(adcBuffer, SIGNAL_LENGTH, 1000000UL / SAMPLING_TIME_US);//SAMPLINGFREQUENCY
  int x = FFT.MajorPeak(adcBuffer, SIGNAL_LENGTH, SAMPLINGFREQUENCY);

  maxAmplitudeDB = 0;
  for (n = 1; n < SIGNAL_LENGTH; n++)
  {
    int a = log10(adcBuffer[n]) * 20 - 54.186; // convert amplitude to dB scale, dB relative to log10(512samples)*20=54.186dB
    if (a > maxAmplitudeDB) maxAmplitudeDB = a;
    adcBuffer[n] = (a + 30) * 5; // scale for TFT display
    vImag[n] = 0; // clear imaginary part
  }
  printDebug("----- End FFT -----");
  Serial.printf("DB:%d\n", maxAmplitudeDB);
#endif  
  
  if(digitalRead(BUTTON_A_PIN) == 0) {
    isTTS = true;
#ifdef USE_TTS
    TTS.play(serif[serifno], 50);
    serifno++;
    if(serifno >= MAX_SERIF) {serifno = 0;}
    Serial.printf("Free Heap Size = %d\n", esp_get_free_heap_size());
#endif
    swing(20, 3);
    wink(LEFT, 2, 5);
    isTTS = false;
  }
  if(digitalRead(BUTTON_B_PIN) == 0) {
      eyeballmove();
      vTaskDelay(100);
  }
  if(digitalRead(BUTTON_C_PIN) == 0) {
      // change expression
      expression++;
      if (expression >= MAX_EXPRESSION) {
          expression = 0;
      }
      vTaskSuspend(drawTaskHandle);
      vTaskDelay(100);
      avatar->setExpression(expression);
      eyeballX = avatar->getEyeballX();
      eyeballY = avatar->getEyeballY();
      vTaskDelay(100);
      vTaskResume(drawTaskHandle);
  }
}

// Start----- Avatar Actions ----------
void swing(int count, int angle) {
    int c = 0;
    for (int i=0; i<count; i++) {
        c = c + 1 % 100;
        float f = sin(c) * angle;
        avatar->setAngle(f);
        vTaskDelay(100);
    }
    avatar->setAngle(0.0);
}
void wink(bool isRight, int count, int angle) {
  vTaskSuspend(blinkTaskHandle);
  for (int i=0; i<count; i++) {
    if (isRight) {
      avatar->setBlink(0.0, RIGHT);
      avatar->setBlink(1.0, LEFT);
      avatar->setAngle(angle);
    } else {
      avatar->setBlink(1.0, RIGHT);
      avatar->setBlink(0.0, LEFT);
      avatar->setAngle(angle);
    }
    vTaskDelay(500);
    avatar->setBlink(1.0);
    avatar->setAngle(0.0);
    vTaskDelay(500);
  }
  vTaskResume(blinkTaskHandle);
}

void eyeballmove() {
    if (eyeball_direction) {
        eyeballX++;
        if (eyeballX >= EYEBALL_MAX) {
            eyeball_direction = LEFT;
        }
    } else {
        eyeballX--;
        if (eyeballX <= (EYEBALL_MAX * -1)) {
            eyeball_direction = RIGHT;
        }
    }
    avatar->setEyeBall(eyeballX, eyeballY);
}

void printDebug(String str) {
#ifdef DEBUG
  Serial.println(str);
#endif
}
