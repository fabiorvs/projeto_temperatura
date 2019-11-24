#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <DHTesp.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

//VARIAVEIS DHT11
DHTesp dht;

//VARIAVEIS WIFI
const char *ssid =  "AP02";
const char *pass =  "fabio123eu";
IPAddress ipaddr;
WiFiClient client;

//VARIAVEIS ENVIO DE DADOS
int ENV = 0;

//VARIAVEIS OLED
Adafruit_SSD1306 dsp(-1);//cria o objeto do display para i2c 

//VARIAVEIS RELE
int status = 0;
const int pinoRele = 16;

void setup()
{
  //INICIALIZACAO DHT11 
  dht.setup(2, DHTesp::DHT11); 

  //INICIALIZACAO OLED
  dsp.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  dsp.clearDisplay();
  dsp.setTextColor(WHITE);
  dsp.setTextSize(1);
  dsp.clearDisplay();
  dsp.println("Conectando Wifi");
  dsp.display();


  //INICIALIZACAO WIFI
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

  //INICIALIZACAO RELE
  pinMode(pinoRele, OUTPUT);
  digitalWrite(pinoRele, HIGH);
  
}


void loop()
{
  
  String station, ip;
  station = "A";

  //RECUPERA AS INF. DO DHT11
  delay(dht.getMinimumSamplingPeriod());
  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();

  //PEGA O IP ATUAL
  ip = WiFi.localIP().toString();

  //ENVIA OS DADOS PARA A API
  enviaDados(String(temperature), String(humidity), ip, station );

  //VERIFICA TEMP / RELE
  verificaTemp(temperature);

  //ATUALIZA O DISPLAY
  exibeLed(temperature,humidity);

  //AGUARDA 5 SEGUNDOS PARA BUSCAR NOVAS INFORMACOES
  delay(5000);
}

//FUNCAO DE ENVIO DE DAOS A API
void enviaDados(String TEMP, String HUM, String IP, String EST){
  String getData, Link;
  HTTPClient http;
  getData = "?temp=" + TEMP + "&hum=" + HUM + "&ip=" + IP + "&estacao=" + EST ;
  Link = "http://192.168.1.111:8080/api/inserir.php" + getData;
  http.begin(Link); 
  int httpCode = http.GET();
  String payload = http.getString();
  ENV = payload.toInt();
  Serial.println(httpCode);
  Serial.println(payload);
  http.end();
}


///FUNCAO DE ATUALIZACAO DISPLAY
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
    dsp.print("OK");
  }
  else {
    dsp.print("Falha");
  }
  dsp.print(" | RELE: ");
  if (status == 0){
    dsp.print("OFF");
  }
  else {
    dsp.print("ON");
  }
  dsp.display();
}

void verificaTemp(float TEMP) {
  int TEMP_MIN = 19; //TEMPERATURA MINIMA PARA DESLIGAR O RELE
  int TEMP_MAX = 30; //TEMPERATURA MAXIMA PARA LIGAR O RELE

  if (TEMP >= TEMP_MAX && status == 0){
    digitalWrite(pinoRele, LOW); //ESP8266 RELE INVERTIDO
    status = 0;
  }

  if (TEMP <= TEMP_MIN && status == 1 ){
    digitalWrite(pinoRele, HIGH); //ESP8266 RELE INVERTIDO
    status = 1;
  }
}