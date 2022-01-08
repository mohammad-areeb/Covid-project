//#include <DHT.h>
#include <dht.h>
//#include <DHT_U.h>
//#define DHTTYPE DHT22                              // Had DHT11 therefore tested the code practically on that with PIR sensor, so for DHT22 just uncomment the other lines
dht DHT;
#include "ThingSpeak.h"
//#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
//WiFiClient  client;

struct model {
  int Temperature_of_hall;
  bool Chiller_power_status = false;
  int No_of_people_in_hall;
} v;

char ssid[] = "xxxxxxxxx"; //SSID here
char pass[] = "yyyyyyyyy"; // Passowrd here

// assuming use of esp2866, or if we use esp32 then we would have to provide an external power source as all sensors used use 5V supply

// Used ThingSpeak Server since its online otherwise devices present on the home network could only access the data

unsigned long Channel_ID = 12345; // Channel ID
const char * myWriteAPIKey = "ABCDEF12345"; //Your write API key
const int field_for_temp = 1;
const int field_for_people = 2;

#define DHT_pin 37
#define buzzerPin 13
#define PIR_pin_entry 39
#define PIR_pin_exit 41

// for managing the data specifically at 1 minute
long int lt_temp = 0;
long int lt_alarm_open = 0;
long int lt_alarm_close = 0;
long int lt_print = 0;
bool buzz = true;
int temp = 0;
String str;
SoftwareSerial espSerial(5, 6);

// Used a pin to maybe use relay for practically using it with an appliance
int chiller_pin = 4;

int entry_counter = 0;
int last_PIR_value_entry = 0;
int exit_counter = 0;
int last_PIR_value_exit = 0;
int people_counter = 0;

//DHT dht(DHT_pin, DHTTYPE);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  //  dht.begin();
  pinMode(chiller_pin, OUTPUT);
  pinMode(PIR_pin_entry, INPUT);
  pinMode(PIR_pin_exit, INPUT);
  pinMode(43, OUTPUT);
  digitalWrite(43, HIGH);

  lcd.begin();                                                            // Used lcd earlier to diplay data as a sort of plotter or we can also use Arduino's Plotter but Thingspeak can do that now.
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Areeb");

  // For WiFi Connection
  //  WiFi.mode(WIFI_STA);
  //  ThingSpeak.begin(client);

  //  if (WiFi.status() != WL_CONNECTED)
  //  {
  //    while (WiFi.status() != WL_CONNECTED)
  //    {
  //      WiFi.begin(ssid, pass);
  //      delay(5000); // delay could be used here since our main function is in the loop
  //    }
  //  }
}



void loop() {
  // put your main code here, to run repeatedly:
  int chk = DHT.read11(DHT_pin);
  if (DHT.temperature > 0) {
    temp = DHT.temperature;
  }
  //  Serial.println(DHT.temperature);
  if (millis() - lt_temp > 4000) {
    lt_temp = millis();
    if (0 < temp) {
      v.Temperature_of_hall = temp;
    }
    Serial.print("Temperature= ");
    Serial.println(v.Temperature_of_hall);
    //    Serial.println("People= ");
    //    Serial.println(v.No_of_people_in_hall);
    if (v.Temperature_of_hall >= 25) {
      v.Chiller_power_status = true;
      digitalWrite(chiller_pin, HIGH);
      Serial.println("Power ON");
    }
    else {
      v.Chiller_power_status = false;
      digitalWrite(chiller_pin, LOW);
      Serial.println("Power OFF");
    }
  }

  if (digitalRead(PIR_pin_entry) == HIGH) {
    entry_counter++;
    //    Serial.println(entry_counter);
  }
  if (entry_counter > 0) {
    if (last_PIR_value_entry - entry_counter == 0) {                          // We are checking when the counter value becomes constant due to PIR and then we increase the poeple no. so that at a time only one count is increased
      if (people_counter < 100) {

        people_counter = people_counter + 1;
//        Serial.print("Now Move. ");
        entry_counter = 0;
      }
    }
  }

  last_PIR_value_entry = entry_counter;               // reinitialising the last_PIR_value_entry at every iteration so that we can track when the counter stops increasing and we can increase our no. of people

  if (digitalRead(PIR_pin_exit) == HIGH) {
    exit_counter++;
    //    Serial.println(entry_counter);
  }
  if (exit_counter > 0) {
    if (last_PIR_value_exit - exit_counter == 0) {                          // We are checking when the counter value becomes constant due to PIR and then we increase the poeple no. so that at a time only one count is increased
      if (people_counter < 100) {

        people_counter = people_counter - 1;
        Serial.print("people_counter= ");
        Serial.println(people_counter);
        exit_counter = 0;
      }
    }
  }

  last_PIR_value_exit = exit_counter;               // reinitialising the last_PIR_value_entry at every iteration so that we can track when the counter stops increasing and we can increase our no. of people


  if (v.No_of_people_in_hall == 90) {
    Serial.println("alarm ON");
    if (millis() - lt_alarm_open > 500) {
      if (buzz) {
        tone(buzzerPin, 4000);
        buzz = false;
      } else {
        noTone(buzzerPin);
        buzz = true;
      }
    }
  }

  else if (v.No_of_people_in_hall == 100) {
    if (millis() - lt_alarm_close > 1000) {
      if (buzz) {
        tone(buzzerPin, 4000);
        buzz = false;
      } else {
        noTone(buzzerPin);
        buzz = true;
      }
    }
  }
  // Assumed it as a frequency or AC buzzer otherwise simple digitalWrite HIGH would have used, and changed the alarm pattern for distinguishing between the two differnet condition alarms.



  if (millis() - lt_print > 4000) {
    lt_print = millis();
    //      ThingSpeak.writeField(Channel_ID, field_for_temp, v.Temperature_of_hall, myWriteAPIKey);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temp=");
    lcd.print(v.Temperature_of_hall, 1);
    lcd.setCursor(9, 0);
    lcd.print("AC= ");
    if (v.Chiller_power_status == true) {
      lcd.print("ON");
    }
    if (v.Chiller_power_status == false) {
      lcd.print("OFF");
    }
    lcd.setCursor(0, 1);
    lcd.print("People= ");
    lcd.print(v.No_of_people_in_hall, 1);
    //      ThingSpeak.writeField(Channel_ID, field_for_people, v.No_of_people_in_hall, myWriteAPIKey);
    Serial.print("People= ");
    //    Serial.println(people_counter);
    Serial.println(v.No_of_people_in_hall);
    str = String("coming from arduino: ") + String("Temp ") + String(v.Temperature_of_hall) + String("People= ") + String(v.No_of_people_in_hall);
    espSerial.println(str);
  }

  v.No_of_people_in_hall = people_counter;
}
