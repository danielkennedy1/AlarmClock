#include <Wire.h>
#include <TimeLib.h>
#include <LiquidCrystal.h>
#include <DS1307RTC.h>

const char *monthName[12] = {
  "Jan", "Feb", "Mar", "Apr", "May", "Jun",
  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
tmElements_t tm;
int H, M;
void setup() {
  //tone(8, 4000);
  bool parse=false;
  bool config=false;
  // get the date and time the compiler was run
  if (getDate(__DATE__) && getTime(__TIME__)) {
    parse = true;
    // and configure the RTC with this info
    if (RTC.write(tm)) {
      config = true;
    }
    lcd.begin(16, 2);
    lcd.print("Time: ");
  }

  Serial.begin(9600);
  digitalWrite(7, LOW);
  while (!Serial) ; // wait for Arduino Serial Monitor
  delay(200);
  H = 15; // define alarm time
  M = 11;
  Serial.println(digitalRead(7));
  if (parse && config) {
    Serial.print("DS1307 configured Time=");
    Serial.print(__TIME__);
    Serial.print(", Date=");
    Serial.println(__DATE__);
  } else if (parse) {
    Serial.println("DS1307 Communication Error :-{");
    Serial.println("Please check your circuitry");
  } else {
    Serial.print("Could not parse info from the compiler, Time=\"");
    Serial.print(__TIME__);
    Serial.print("\", Date=\"");
    Serial.print(__DATE__);
    Serial.println("\"");
  }
}

void loop() {
  tmElements_t tm;
  //display time on screen
  RTC.read(tm);
  lcd.setCursor(0, 1);
  lcd.print(show2digits(tm.Hour) + ":" + show2digits(tm.Minute) + ":" + show2digits(tm.Second));
  //if current time is alarm time, send signal and sound buzzer 
  if(tm.Hour == H && tm.Minute == M){
    digitalWrite(7, HIGH);
    tone(8, 400);
    }
  delay(100);
}


//used to configure the RTC chip to time on computer during setup
bool getTime(const char *str)
{
  int Hour, Min, Sec;

  if (sscanf(str, "%d:%d:%d", &Hour, &Min, &Sec) != 3) return false;
  tm.Hour = Hour;
  tm.Minute = Min;
  tm.Second = Sec;
  return true;
}
//same but for date
bool getDate(const char *str)
{
  char Month[12];
  int Day, Year;
  uint8_t monthIndex;

  if (sscanf(str, "%s %d %d", Month, &Day, &Year) != 3) return false;
  for (monthIndex = 0; monthIndex < 12; monthIndex++) {
    if (strcmp(Month, monthName[monthIndex]) == 0) break;
  }
  if (monthIndex >= 12) return false;
  tm.Day = Day;
  tm.Month = monthIndex + 1;
  tm.Year = CalendarYrToTm(Year);
  return true;
}
//put a 0 in front of numbers smaller than 10 when displaying on the LCD
String show2digits(int number) {
  String val = String(number, DEC);
  if (number >= 0 && number < 10) {
    return "0" + val;
  }
  return val;
}
