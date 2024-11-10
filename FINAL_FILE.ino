#define BLYNK_TEMPLATE_ID "TMPL6x5DaJws1"
#define BLYNK_TEMPLATE_NAME "GrowTech"
#define BLYNK_AUTH_TOKEN "pQnwF6MH7Uh9a8gT2kQeL_c3NvK-D6Nw"


#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <BlynkSimpleEsp8266.h>
#include <ThingSpeak.h>

#define DHTPIN 2  // D4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
  int soilMoistureThreshold = 30;
  int relay=12; // D6

#define SOIL_SENSOR_PIN A0

LiquidCrystal_I2C lcd(0x3F, 16, 2);  
unsigned long channelNumber = 2438538;  
char writeAPIKey[] = "UZTYTNFE99ZFN2GD"; 
char auth[] = "pQnwF6MH7Uh9a8gT2kQeL_c3NvK-D6Nw"; 

void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, "JnU Students", "stdwifi154");
  pinMode(relay,OUTPUT);

  delay(100);
  lcd.init();
  lcd.clear();
  lcd.backlight(); 
  dht.begin();
}

void loop() {
  Blynk.run();

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  int soilMoisture = analogRead(SOIL_SENSOR_PIN);
    soilMoisture = map(soilMoisture, 0, 1023, 100, 0);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(temperature);
  lcd.print("C");

  lcd.setCursor(10, 0);
  lcd.print("H:");
  lcd.print(humidity);
  lcd.print("%");

  lcd.setCursor(0, 1);
  lcd.print("SM:");
  lcd.print(soilMoisture);
  lcd.print("%");

  Blynk.virtualWrite(V1, temperature);
  Blynk.virtualWrite(V2, humidity);
  Blynk.virtualWrite(V0, soilMoisture);
   Serial.print("temperature:");
    Serial.println(temperature);
     Serial.print("humidity:");
      Serial.println(humidity);
       Serial.print("soilMoisture:"); 
       Serial.println(soilMoisture);

   WiFiClient client;

 
  ThingSpeak.begin(client); 

  ThingSpeak.setField(1,soilMoisture); 
  ThingSpeak.setField(2, temperature);   
  ThingSpeak.setField(3, humidity);   


  ThingSpeak.writeFields(channelNumber, writeAPIKey);

  if (soilMoisture > soilMoistureThreshold) {
    Serial.print("realy on");
    digitalWrite(relay,LOW);
      lcd.setCursor(7, 1);
  lcd.print("MOTOR:OFF");
 
  } 
  else if ((soilMoisture < soilMoistureThreshold)) {
    Serial.print("realy off");
    digitalWrite(relay,HIGH);
       lcd.setCursor(7, 1);
  lcd.print("MOTOR:ON");

  }

  delay(1000);  
}
