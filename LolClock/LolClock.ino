#define countof(a) (sizeof(a) / sizeof(a[0]))

#if defined(ESP8266)
#include <pgmspace.h>
#else
#include <avr/pgmspace.h>
#endif

#include <Wire.h>
#include <RtcDS1307.h>
RtcDS1307<TwoWire> Rtc(Wire);

const char data[] = "what time is it";
int clockValues[4] = {11, 11, 11, 11};

int numbers[12][20] = {
  {0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0},
  {0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0},
  {1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 1},
  {1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0},
  {1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0},
  {1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0},
  {0, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0},
  {1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0},
  {0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0},
  {0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 1, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

int values[4][20] = {
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};


int lds[4][20][2] = {
  {{6, 4}, {8, 1}, {5, 4}, {8, 2}, {6, 3}, {11, 1}, {5, 3}, {11, 2}, {6, 2}, {10, 1}, {6, 1}, {10, 2}, {7, 0}, {13, 1}, {6, 0}, {13, 2}, {9, 1}, {12, 1}, {9, 2}, {12, 2}},
  {{4, 4}, {8, 3}, {4, 3}, {8, 4}, {5, 2}, {11, 3}, {4, 2}, {11, 4}, {5, 1}, {10, 3}, {4, 1}, {10, 4}, {5, 0}, {13, 3}, {4, 0}, {13, 4}, {9, 3}, {12, 3}, {9, 4}, {12, 4}},
  {{3, 3}, {8, 5}, {2, 3}, {8, 6}, {3, 2}, {11, 5}, {2, 2}, {11, 6}, {3, 1}, {10, 5}, {2, 1}, {10, 6}, {3, 0}, {13, 5}, {2, 0}, {13, 6}, {9, 5}, {12, 5}, {9, 6}, {12, 6}},
  {{1, 3}, {8, 7}, {0, 3}, {8, 8}, {1, 2}, {11, 7}, {0, 2}, {11, 8}, {1, 1}, {10, 7}, {0, 1}, {10, 8}, {1, 0}, {13, 7}, {0, 0}, {13, 8}, {9, 7}, {12, 7}, {9, 8}, {12, 8}}
};

#include <avr/pgmspace.h>
#include <Charliplexing.h>

void setup() {
  LedSign::Init(GRAYSCALE);
  setupClock();
}

void setupClock() {
  Rtc.Begin();
  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
  if (!Rtc.IsDateTimeValid()) {
    Rtc.SetDateTime(compiled);
  }
  if (!Rtc.GetIsRunning()) {
    Rtc.SetIsRunning(true);
  }
  RtcDateTime now = Rtc.GetDateTime();
  if (now < compiled) {
    Rtc.SetDateTime(compiled);
  }
  Rtc.SetSquareWavePin(DS1307SquareWaveOut_Low);
  Rtc.SetMemory(0, 13);
  uint8_t written = Rtc.SetMemory(13, (const uint8_t*)data, sizeof(data) - 1); // remove the null terminator strings add
  Rtc.SetMemory(1, written);
}

void updateClock() {
  RtcDateTime now = Rtc.GetDateTime();

  String hour = String(now.Hour());
  String minute = String(now.Minute());

  if (hour.length() == 2) {
    clockValues[0] = hour.substring(0, 1).toInt();
    clockValues[1] =  hour.substring(1, 2).toInt();
  } else {
    clockValues[0] = 0;
    clockValues[1] =  hour.substring(0, 1).toInt();
  }

  if (minute.length() == 2) {
    clockValues[2] = minute.substring(0, 1).toInt();
    clockValues[3] =  minute.substring(1, 2).toInt();
  } else {
    clockValues[2] = 0;
    clockValues[3] =  minute.substring(0, 1).toInt();
  }
  
  uint8_t address = Rtc.GetMemory(0);
  if (address != 13)  {
  } else {
    uint8_t count = Rtc.GetMemory(1);
    uint8_t buff[20];
    uint8_t gotten = Rtc.GetMemory(address, buff, count);
    while (gotten > 0) {
      gotten--;
    }
  }
}

void loop() {
  for (int digit = 0; digit < 4; digit++) {
    for (int dot = 0; dot < 20; dot++) {
      values[digit][dot] = numbers[clockValues[digit]][dot];
    }
  }
  for (int digit = 0; digit < 4; digit++) {
    for (int dot = 0; dot < 20; dot++) {
      LedSign::Set(lds[digit][dot][0], lds[digit][dot][1], values[digit][dot] == 1 ? 7 : 0);
    }
  }
  updateClock();
  delay(5000);
}
