#define BLYNK_PRINT Serial
#include <SoftwareSerial.h>
#include <BlynkSimpleEsp8266.h>
#include <ESP8266WiFi.h>
#include <Servo.h>

char auth[] = "Beu7zoWenS-M9EdmtCDE1GsjOSp4B1B5"; //token dapat dicek di email
char ssid[] = "S-NETWORK!"; //nama wifi
char pass[] = "siregar990410"; //password wifi

Servo servo;
SoftwareSerial DataSerial(12, 13);

unsigned long previousMillis = 0;
const long interval = 3000;

String arrData[2];

void setup() {
  // put your setup code here, to run once:
 Serial.begin(9600);
 DataSerial.begin(9600);
 Blynk.begin(auth, ssid, pass);
 servo.attach(2);

}

void loop() {
  // put your main code here, to run repeatedly:
 unsigned long currentMillis = millis();
 if(currentMillis - previousMillis >= interval)
 {
  previousMillis = currentMillis;

  String data = "";
  while(DataSerial.available()>0)
  {
    data += char(DataSerial.read());
  }
  data.trim();

  if(data != "")
  {

    int index = 0;
    for(int i=0; i<= data.length(); i++)
    {
      char delimiter = '#' ;
      if(data[i] != delimiter)
         arrData[index] += data[i];
      else
         index++;
    }

    if(index == 1)
    {
      Serial.println("Tetes Per Menit : " + arrData[0]);
      Serial.println();
      Serial.println("Berat Infus : " + arrData[1]);
      Serial.println();
      Blynk.virtualWrite(V2, arrData[0]);
      Blynk.virtualWrite(V3, arrData[1]);
    }

    arrData[0] = "";
    arrData[1] = "";
  }

  DataSerial.println("Ya");
 }
 Blynk.run();

}

BLYNK_WRITE(V5)
{
  servo.write(param.asInt());
}
