#include <TickerSchedlr.h>
#include <Adafruit_NeoPixel.h>
#include <particulate_PPD42.h>
#include <barometer_BMP085.h>
#include <humidity_HTU21D.h>
//#include <gps_SoftwareSerial.h>

#define NEOPIXEL_PIN 5
#define I2C_SDA_PIN 2
#define I2C_SCL_PIN 14
#define SERIAL_RX_PIN 0
#define SERIAL_TX_PIN 4

#define SERIAL_BAUD 74880

BMP085Sensor barometer;
HTU21DFSensor humidity;
Adafruit_NeoPixel neopixel = Adafruit_NeoPixel(1, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);
PPD42Sensor particulate;
TickerSchedlr *schedule = 0; //TickerSchedlr::Instance(200);

int count = 0;
int count2 = 0;
int count3 = 0;
TickerTask * T1 = NULL;
TickerTask * T2 = NULL;
TickerTask * T3 = NULL;
int pin1 = 0;
int pin2 = 14;
int pin3 = 13;

void flip(void *)
{
//  int state = digitalRead(12);  // get the current state of GPIO1 pin
//  digitalWrite(12, !state);     // set pin to the opposite state
  __LOG(count);
  __LOG(" :flip1: ");
  __LOGLN(state);
  ++count;
  Serial.println(humidity.report());
  Serial.println(barometer.report());
  Serial.println(particulate.report());
}

void flip2(void *ptr)
{
  int pin = 14;
  //int *iptr = (int *)ptr;
  //int state = digitalRead(*iptr);  // get the current state of GPIO1 pin
  int state = digitalRead(pin);  // get the current state of GPIO1 pin
  digitalWrite(pin, !state);     // set pin to the opposite state
  __LOG(count2);
  __LOG(" :flip2: ");
  __LOGLN(state);
  ++count2;

}

void flip3(void *)
{
  int state = digitalRead(13);  // get the current state of GPIO1 pin
  digitalWrite(13, !state);     // set pin to the opposite state
  __LOG(count3);
  __LOG(" :flip3: ");
  __LOGLN(state);
  __LOG("last "); __LOG(T3->getLastTime()); __LOG(" next ");    __LOG(T3->getNextTime()); __LOG("  period "); __LOGLN(T3->getNextTime() - T3->getLastTime());
  ++count3;

  barometer.read();
  humidity.read();
  particulate.read();
}

void setup() {
  Serial.begin(SERIAL_BAUD);
  Serial.println("Setup started.");
  Serial.println("Serial communication initialized...");

  neopixel.begin();
  neopixel.setBrightness(63);
  neopixel.setPixelColor(0, 63, 0, 63);
  neopixel.show();
  Serial.println("NeoPixel initialized...");

//  gps_Serial.begin(GPS_BAUD);  
//  Serial.println("GPS initialized...");

  Serial.print("Initializing PPD42...");
  particulate.begin();
  Serial.println(" done");

  pinMode(pin2, OUTPUT);
  digitalWrite(pin2, LOW);
  pinMode(pin3, OUTPUT);
  digitalWrite(pin3, LOW);

  schedule = TickerSchedlr::Instance(200);
  T1 = TickerTask::createPeriodic(&flip, 5000);
  T2 = TickerTask::createIdle(&flip2);
  T3 = TickerTask::createPeriodic(&flip3, 1000);
  T1->name = "Task 0";
  T2->name = "Task 1";
  T3->name = "Task 2";
  schedule = TickerSchedlr::Instance();
  __LOG("Task 0 next time is "); __LOGLN(T1->getNextTime());
  __LOG("Task 0 period is "); __LOGLN(T1->interval);
  __LOG("Task 0 type is "); __LOGLN(T1->tasktype);
  __LOG("Schedule time "); __LOGLN(schedule->getTickTime());
  __LOG("Task 1 next time is "); __LOGLN(T2->getNextTime());
  __LOG("Task 1 period is "); __LOGLN(T2->interval);
  __LOG("Task 1 type is "); __LOGLN(T2->tasktype);
  //__LOG("Schedule time "); __LOGLN(schedule->getTickTime());
  __LOG("Task 2 next time is "); __LOGLN(T3->getNextTime());
  __LOG("Task 2 period is "); __LOGLN(T3->interval);
  __LOG("Schedule time "); __LOGLN(schedule->getTickTime());

  __LOGLN("schedule added");
  Serial.println("Tasks created...");

  humidity.begin();
  Serial.println("Humidity sensor initialized...");

  barometer.begin();
  Serial.println("Barometer sensor initialized...");

  Serial.println("Setup finished.");
}

// the loop function runs over and over again until power down or reset
void loop() {
  schedule->tick();
}
