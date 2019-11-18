#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <DHTesp.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

DHTesp dht;

const char *ssid =  "AP02";
const char *pass =  "fabio123eu";
IPAddress ipaddr;
WiFiClient client;
int ENV = 0;
 
Adafruit_SSD1306 dsp(-1);//cria o objeto do display para i2c 

void setup()
{
  dht.setup(2, DHTesp::DHT11); 
  
  dsp.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  dsp.clearDisplay();
  dsp.setTextColor(WHITE);
  dsp.setTextSize(1);
  dsp.clearDisplay();
  dsp.println("Conectando Wifi");
  dsp.display();
  
  Serial.begin(9600);
  delay(10);
  Serial.println("Connecting to ");
  Serial.println(ssid); 
  WiFi.begin(ssid, pass); 
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
    dsp.print(".");
    dsp.display();
  }
  
  Serial.println("");
  Serial.println("WiFi connected"); 
  Serial.println(WiFi.localIP());
  ipaddr = WiFi.localIP();
}
 
void loop()
{
  String station, getData, Link, ip;
  station = "A";
  delay(dht.getMinimumSamplingPeriod());

  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();

  HTTPClient http;
  ip = WiFi.localIP().toString();
  getData = "?temp=" + String(temperature) + "&hum=" + String(humidity) + "&ip=" + ip + "&estacao=" + station ;
  Link = "http://192.168.1.111/projetos/temp/inserir.php" + getData;
  http.begin(Link); 
  int httpCode = http.GET();
  String payload = http.getString();
  ENV = payload.toInt();
  Serial.println(httpCode);
  Serial.println(payload);
  http.end();
  
  exibeLed(temperature,humidity);
  delay(5000);
  
}

void exibeLed(float TEMP, float HUM) {
  dsp.setCursor(0,0);
  dsp.clearDisplay();
  dsp.print("IP: ");
  dsp.println(ipaddr);
  dsp.print("Temperatura: ");
  dsp.print(TEMP);
  dsp.println(" C");
  dsp.print("Humidade: ");
  dsp.print(HUM);
  dsp.println("%");
  dsp.print("Envio: ");
  if (ENV == 1){
    dsp.println("OK");
  }
  else {
    dsp.println("Falha");
  }
  dsp.display();
}
