// BLYNK & LIBRARY ESP8266
#define BLYNK_TEMPLATE_ID "TMPL6TzTKhl-7"
#define BLYNK_TEMPLATE_NAME "Detection Gas LPG and Fire"
#define BLYNK_AUTH_TOKEN "_O9UjSxs2lsI2VH9W10e4rfa91iwbCDZ"
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
// BLYNK & LIBRARY ESP8266 END

// CONNECTION WIFI
char ssid[] = "rymond";
char pass[] = "87654321";
// CONNECTION WIFI END

BlynkTimer timer;

//INISIALISASI VARIABEL SENSOR GAS
int mq2 = A0;
int sensormq2;
//INISIALISASI VARIABEL SENSOR GAS END

//INISIALISASI VARIABEL SENSOR API
WidgetLCD lcd(V1);
int flame = D4;
int sensorflame = HIGH;
//INISIALISASI VARIABEL SENSOR API END

//INISIALISASI VARIABEL RELAY
const int sirine = D3;
const int pompa = D2;
int buttonsirine;
int buttonpompa;
int relayoff = HIGH;
int relayon = LOW;
//INISIALISASI VARIABEL RELAY END

//INISIALISASI INDIKATOR START
WidgetLED indikatorsirine(V3);
WidgetLED indikatorpompa(V5);
//INISIALISASI INDIKATOR END

//BLYNK BUTTON SIRINE
BLYNK_WRITE(V2) {
  buttonsirine = param.asInt();
  digitalWrite(sirine, buttonsirine);
  if (buttonsirine) {
    digitalWrite(sirine, relayon);
    indikatorsirine.on();
    delay(5000);
  } else {
    indikatorsirine.off();
    digitalWrite(sirine, relayoff);
  }
}
//BLYNK BUTTON SIRINE END

//BLYNK BUTTON POMPA
BLYNK_WRITE(V4) {
  buttonpompa = param.asInt();
  digitalWrite(pompa, buttonpompa);
  if (buttonpompa) {
    digitalWrite(pompa, relayon);
    indikatorpompa.on();
    delay(5000);
  } else {
    indikatorpompa.off();
    digitalWrite(pompa, relayoff);
  }
}
//BLYNK BUTTON POMPA END

//BLYNK GAUGE SENSOR GAS
void sendSensor() {
  sensormq2 = analogRead(mq2);
  Blynk.virtualWrite(V0, sensormq2);
}
//BLYNK GAUGE OUTPUT SENSOR GAS END

void setup() {
  Serial.begin(115200);
  delay(50);
  pinMode(mq2, INPUT);
  pinMode(flame, INPUT);
  pinMode(sirine, OUTPUT);
  pinMode(pompa, OUTPUT);
  digitalWrite(sirine, relayoff);
  digitalWrite(pompa, relayoff);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  timer.setInterval(1000L, sendSensor);
  Blynk.syncVirtual(V2);
  Blynk.syncVirtual(V4);
}

void loop() {
  sensorflame = digitalRead(flame);
  
  Blynk.run();
  timer.run();

  Serial.println(sensormq2);
  Serial.print("");

  //LCD BLYNK SENSOR API
  lcd.print(0, 0, "CURRENT STATE ?");
  //LCD BLYNK SENSOR API END

  //KONDISI SENSOR GAS
  if (sensormq2 > 40) {
    Serial.println("WARNING THERE IS GAS LPG..!!");
    digitalWrite(sirine, relayon);
    indikatorsirine.on();
    delay(5000);
  } else {
    Serial.println("GAS LPG SAFETY");
    digitalWrite(sirine, relayoff);
    indikatorsirine.off();
    delay(1000);
  }
  //KONDISI SENSOR GAS END

  ////KONDISI SENSOR API
  if (sensorflame == LOW) {
    lcd.print(0, 1, "THERE IS FIRE.!!");
    Serial.println("DANGEROUS..!!");
    digitalWrite(sirine, relayon);
    digitalWrite(pompa, relayon);
    indikatorsirine.on();
    indikatorpompa.on();
    delay(5000);
  } else {
    lcd.print(0, 1, "NO FIRE");
    Serial.println("NO FIRE");
    digitalWrite(sirine, relayoff);
    digitalWrite(pompa, relayoff);
    indikatorsirine.off();
    indikatorpompa.off();
    delay(1000);
  }
  //KONDISI SENSOR API END
  lcd.clear();
}
