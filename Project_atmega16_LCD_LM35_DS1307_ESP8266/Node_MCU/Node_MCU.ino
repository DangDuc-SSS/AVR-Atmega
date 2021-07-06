#include <BlynkSimpleEsp8266.h>
#include<SoftwareSerial.h>
SoftwareSerial s(3, 1);
String apiWritekey = "YM1MRU723YRG9N4W"; 
char auth[]="7V-HVrHAd2Ed7kGgauEHMddGvwEbPzBe";
//const char* ssid = "TP-LINK";
//const char* password = "016524499";
const char* ssid = "A";
const char* password = "Khongcho";
const char* server ="api.thingspeak.com";
String d="";
WiFiClient client;
BlynkTimer timer;
WidgetLED led1(V0);
WidgetLED led2(V1);
WidgetLCD lcd(V2);
//led1.on();
//BlynkTimer timer;

void setup()
{
  pinMode(D4,OUTPUT);
  //Serial.begin(9600);
  s.begin(9600);
  WiFi.disconnect();
  delay(10);
  WiFi.begin(ssid, password);
  Blynk.begin(auth, ssid, password);
  lcd.clear();
  lcd.print(5,0,"Nhóm:6");
  lcd.print(2,1,"Đo nhiệt độ");
  //timer.setInterval(1000L, Led);
  while(WiFi.status()!=WL_CONNECTED){
   delay(100);

  }
}

void loop()
{
 
  if(s.available())
  { 
  while(s.available())
  {
  String c=s.readString();
  int t=c.toInt();
  d+=c;
    Blynk.virtualWrite(V6, t);
    Blynk.virtualWrite(V1,t);
    Blynk.virtualWrite(V0,t);
    
    led1.on();
    if(t>=22)
  {
    led2.on();
  
  }
  else led2.off();
    Blynk.run();
    d="";
      if (client.connect(server, 80)) {

       client.print("GET /update?api_key=YM1MRU723YRG9N4W&field1=0");     //YOUR URL
       // client.print(received_data);
       client.print(c);
       client.print(" ");      //SPACE BEFORE HTTP/1.1
       client.print("HTTP/1.1");
       client.println();
       client.println("Host: api.thingspeak.com");
       client.println("Connection: close");
       client.println();
       digitalWrite(D4,LOW);
      
      }
    
  }
  }
  else 
  {
    digitalWrite(D4,HIGH);

  }
}
