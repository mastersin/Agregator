#include "LCD.h"
#include "Config.h"
#include "Interval.h"

#define DEBUG

using namespace ACRobot;

const uint8_t columnPin = A0;
const uint8_t coolerPin = A1;
const uint8_t cubePin   = A2;

const uint8_t valvePin   = 8;
const uint8_t heater1Pin = 9;
const uint8_t heater2Pin = 10;

uint16_t columnTemp;
uint16_t coolerTemp;
uint16_t cubeTemp;

struct Settings
{
  uint8_t status;
  uint32_t time;
  uint32_t finish;
};

enum State
{
  StartHeat,
  WaitCooler,
  WaitHeater,
  StopHeat,
  RestartProcess
};

Settings settings = { 0, 0, 0 };
Config<Settings> config(0x02, settings);

enum IntervalType {
    GLOBAL,
    SCREEN,
    BLINK,
    CONFIG,
    NUMBER_OF_INTERVALS
};

const unsigned long GLOBAL_INTERVAL   = 50;
const unsigned long SCREEN_INTERVAL   = 100;
const unsigned long BLINK_INTERVAL    = 1000;
const unsigned long CONFIG_INTERVAL   = 10000;

Intervals<NUMBER_OF_INTERVALS> intervals;

LCD<LCD_1602A> lcd;

int poll()
{
  return intervals.status();
}

void setup()
{
#ifdef DEBUG
  Serial.begin(9600);
#endif

  settings = config();

  intervals[GLOBAL]   = GLOBAL_INTERVAL;
  intervals[BLINK]    = BLINK_INTERVAL;
  intervals[SCREEN]   = SCREEN_INTERVAL;
  intervals[CONFIG]   = CONFIG_INTERVAL;

#ifdef DEBUG
  Serial.println("wait");
#endif

//  waitForStartWithLED(button);

#ifdef DEBUG
  Serial.println("start");
#endif
}

void blink()
{
  static bool led = true;

  if (led)
    clearDigitalPin(13);
  else
    setDigitalPin(13);

  settings.time++;  

#ifdef DEBUG
  Serial.println("blink");
#endif
}

void screen()
{
  lcd.clear();
  lcd.print(settings.status);
  lcd.setCursor(2, 0);
  lcd.print(settings.time);
  lcd.setCursor(0, 1);
  lcd.print(columnTemp);
  lcd.setCursor(4, 1);
  lcd.print(coolerTemp);
  lcd.setCursor(8, 1);
  lcd.print(cubeTemp);
}

uint16_t toTemp(uint16_t raw)
{
  const uint16_t minRaw = 554;

  if (raw < minRaw)
    raw = minRaw;

  return (raw - 554) / 2;
}

void setState(uint8_t currentState)
{
  switch(currentState)
  {
    case StartHeat:
    case WaitCooler:
      digitalWrite(valvePin,   LOW);
      digitalWrite(heater1Pin, HIGH);
      digitalWrite(heater2Pin, HIGH);
      break;
    case WaitHeater:
      digitalWrite(valvePin,   HIGH);
      digitalWrite(heater1Pin, HIGH);
      digitalWrite(heater2Pin, HIGH);
      break;
    case StopHeat:
    case RestartProcess:
    default:
      digitalWrite(valvePin,   LOW);
      digitalWrite(heater1Pin, LOW);
      digitalWrite(heater2Pin, LOW);
      break;
  }
}

uint8_t fsm(uint8_t currentState)
{
  uint8_t newState;
  
  switch(currentState)
  {
    case StartHeat:
      newState = WaitCooler;
      break;
    case WaitCooler:
      if (coolerTemp > 50)
        newState = WaitHeater;
      break;
    case WaitHeater:
      if (cubeTemp > 98) {
        newState = StopHeat;
        settings.finish = settings.time;
      }
      break;
    case StopHeat:
      if ((settings.finish + 60) < settings.time)
        newState = RestartProcess;
      break;
    case RestartProcess:
    default:
      newState = StartHeat;
      settings.finish = 0;
      settings.time = 0;
      break;
  }

  return newState;
}

void logic()
{
  setState(settings.status);

  columnTemp = toTemp(analogRead(columnPin));
  coolerTemp = toTemp(analogRead(coolerPin));
  cubeTemp   = toTemp(analogRead(cubePin));

#ifdef DEBUG
  Serial.print(settings.time);
  Serial.print(" (");
  Serial.print(settings.finish);
  Serial.print("): ");
  Serial.print(columnTemp);
  Serial.print(", ");
  Serial.print(coolerTemp);
  Serial.print(", ");
  Serial.println(cubeTemp);
#endif

  uint8_t state = fsm(settings.status);
  settings.status = state;
}

void loop()
{
  switch (poll())
  {
    case GLOBAL:
      logic();
      break;
    case SCREEN:
      screen();
      break;
    case BLINK:
      blink();
      break;
    case CONFIG:
      config(settings);
      config.poll();
      break;
  }
}

