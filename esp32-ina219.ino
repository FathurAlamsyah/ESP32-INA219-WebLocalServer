//Library For Connect To WiFi and Send To Server
#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <INA219.h>

int interval = 20000;
long previousMillis = 0;
long currentMillis = 0;

INA219 ina219;

float busvoltage = 0;
float current = 0;

//Setting WiFi
const char* ssid = "Name of SSID/WiFi"; //Name of SSID or WiFi
const char* pass = "Password WiFi";        //Password of WiFi

void setup() {
  Serial.begin(115200);
  ina219.begin();

  Serial.println("\n");
  Serial.print("Connecting to :  ");
  Serial.println(ssid);
  WiFi.begin(ssid,pass);
  //Try To Connecting to WiFi
  while (WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.println(".");
  }
  Serial.print("WiFi Connected");
  delay(2000);
  previousMillis = millis();
}

void loop() {
  currentMillis = millis();
  
  //Read Voltage and Current from Sensor
  busvoltage = ina219.busVoltage();
  current = ina219.shuntCurrent();
  
  //Display Value of Voltage and Current from Sensor
  Serial.print("Bus Voltage:   "); Serial.print(busvoltage,2); Serial.println(" V");
  Serial.print("Current:       "); Serial.print(current,4); Serial.println(" mA");
  Serial.println("======================================");
  delay(2000);

  //Send Data if Millis reach Interval
  if(currentMillis - previousMillis > interval){
    HTTPClient http;
    String postData;
    postData = "volt="+String(busvoltage)+"&curr="+String(current,4); 
    //IP for Access Database Server
    //InsertDB is File php to Save/Insert Data to Database (PHPMySQL)
    http.begin("http://192.168.1.82/ina/InsertDB.php");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    int httpCode = http.POST(postData); //Send the request
    String payload = http.getString();  //Get the response payload
    Serial.println(httpCode);   //Print HTTP return code
    Serial.println(payload);    //Print request response payload
    Serial.println("========================\n");
    http.end();
    previousMillis = currentMillis;
  }
  
}
