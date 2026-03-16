#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>      
#include <ArduinoOTA.h>   
#include <Wire.h>
#include <Adafruit_BME280.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// --- CONFIGURAÇÕES ---
#define LED_VERDE 2
#define LED_VERMELHO 4
const char* ssid     = "Nome da Rede Ficticia";// Nome do Wifi Local
const char* password = "Senha Ficticia"; //Senha do Wifi Local
const char* hgKey    = "Chave Ficticia"; // Chave da API

// --- OBJETOS ---
Adafruit_BME280 bme; 
WebServer server(80);

// --- VARIÁVEIS GLOBAIS (LOCAL) ---
float altitudeSJP = 905.0; 
float tempMaxima = -100.0, tempMinima = 100.0, ultimaPressao = 0;
String previsaoLocal = "Calculando...";

// --- VARIÁVEIS GLOBAIS (AEROPORTO SBCT) ---
float tempAero = 0;
int umidAero = 0;
float pressaoAero = 0;
String ventoAero = "", ventoDir = "", nascerSol = "", porSol = "", condicaoAero = "";
bool alertaRaios = false;
String statusAPI = "Aguardando...";

void verificarAeroporto() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    // Buscando dados de SJP (Aeroporto Afonso Pena - SBCT)
    String url = "http://api.hgbrasil.com/weather?key=" + String(hgKey) + "&city_name=Sao_Jose_dos_Pinhais,PR";
    
    http.begin(url);
    int httpCode = http.GET();
    
    if (httpCode > 0) {
      String payload = http.getString();
      DynamicJsonDocument doc(2048);
      deserializeJson(doc, payload);
      
      JsonObject results = doc["results"];
      tempAero     = results["temp"];
      umidAero     = results["humidity"];
      pressaoAero  = results["pressure"];
      ventoAero    = results["wind_speedy"].as<String>();
      ventoDir     = results["wind_direction"].as<String>();
      nascerSol    = results["sunrise"].as<String>();
      porSol       = results["sunset"].as<String>();
      condicaoAero = results["description"].as<String>();

      // Lógica de Detecção de Raios/Tempestade
      int code = results["condition_code"];
      // Códigos 3, 4 e 37-47 na HG Brasil referem-se a tempestades/raios
      if (code == 3 || code == 4 || (code >= 37 && code <= 47)) {
        alertaRaios = true;
      } else {
        alertaRaios = false;
      }
      statusAPI = "✅ SBCT Atualizado às " + results["time"].as<String>();
    } else {
      statusAPI = "❌ Erro API: " + http.errorToString(httpCode);
    }
    http.end();
  }
}

void handleRoot() {
  float t = bme.readTemperature();
  float h = bme.readHumidity();
  float pRaw = bme.readPressure() / 100.0F;
  float pMar = pRaw * pow(1 - ((0.0065 * altitudeSJP) / (t + (0.0065 * altitudeSJP) + 273.15)), -5.257);

  if (t > tempMaxima && t < 60) tempMaxima = t;
  if (t < tempMinima && t > -40) tempMinima = t;

  String html = "<html><head><meta charset='UTF-8' http-equiv='refresh' content='30'>";
  html += "<style>body{font-family:sans-serif; background:#0f172a; color:#e2e8f0; text-align:center; padding:10px;}";
  html += ".container{display:flex; justify-content:center; gap:15px; flex-wrap:wrap; margin-top:20px;}";
  html += ".card{background:#1e293b; padding:20px; border-radius:15px; width:320px; border-top: 5px solid;}";
  html += ".local{border-color:#10b981;} .aero{border-color:#3b82f6;}";
  html += ".alert{background:#ef4444; color:white; padding:10px; border-radius:10px; margin-bottom:10px; animation:blink 1s infinite;}";
  html += "@keyframes blink{0%{opacity:1;} 50%{opacity:0.4;} 100%{opacity:1;}}";
  html += "h1{margin-bottom:0; color:#10b981;} .status{font-size:0.7em; color:#94a3b8;}</style></head><body>";

  html += "<h1>Estação Adad SJP</h1><p class='status'>" + statusAPI + "</p>";

  // Alerta de Raios no topo se detectado usando como referencia os dados do Aeroporto
  if (alertaRaios) {
    html += "<div class='container'><div class='alert'>⚡ ALERTA: RAIOS E TEMPESTADE EM SBCT</div></div>";
  }

  html += "<div class='container'>";
  
  // COLUNA 1: DADOS LOCAIS (ADAD)
  html += "<div class='card local'><h2>🏠 Minha Casa</h2>";
  html += "<p style='font-size:1.5em; color:#10b981;'><b>" + String(t) + " °C</b></p>";
  html += "<p>💧 Umidade: " + String(h, 1) + "%</p>";
  html += "<p>⚓ Pressão: " + String(pMar, 1) + " hPa</p>";
  html += "<hr><p>🔮 " + previsaoLocal + "</p>";
  html += "<p style='font-size:0.8em;'>Mín: " + String(tempMinima, 1) + "° | Máx: " + String(tempMaxima, 1) + "°</p></div>";

  // COLUNA 2: DADOS AEROPORTO (SBCT)
  html += "<div class='card aero'><h2>✈️ Aeroporto</h2>";
  html += "<p style='font-size:1.5em; color:#3b82f6;'><b>" + String(tempAero, 0) + " °C</b></p>";
  html += "<p>🌬️ Vento: " + ventoAero + " (" + ventoDir + "°)</p>";
  html += "<p>⚓ Pressão: " + String(pressaoAero, 0) + " hPa</p>";
  html += "<p>☁️ Céu: " + condicaoAero + "</p>";
  html += "<hr><p>🌅 " + nascerSol + " | 🌇 " + porSol + "</p></div>";

  html += "</div></body></html>";
  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);

  if (!bme.begin(0x76)) { while (1); }

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_VERMELHO, !digitalRead(LED_VERMELHO));
    delay(500);
  }
  digitalWrite(LED_VERMELHO, LOW);
  digitalWrite(LED_VERDE, HIGH);

  ArduinoOTA.setHostname("adad-sjp");
  ArduinoOTA.setPassword("adad123");
  ArduinoOTA.begin();
  
  MDNS.begin("adad");
  server.on("/", handleRoot); 
  server.begin();

  verificarAeroporto(); // Primeira carga de dados
}

void loop() {
  ArduinoOTA.handle();   
  server.handleClient(); 

  // Lógica de 60 segundos: Previsão Local + Verificação de Alertas
  static unsigned long timerGeral = 0;
  if (millis() - timerGeral > 60000) {
    // 1. Previsão Barométrica Local
    float pAtual = bme.readPressure() / 100.0F;
    if (ultimaPressao > 0) {
      float delta = pAtual - ultimaPressao;
      if (delta < -0.15) previsaoLocal = "⛈️ Queda de Pressão (Chuva)";
      else if (delta > 0.15) previsaoLocal = "☀️ Pressão Subindo (Sol)";
      else previsaoLocal = "☁️ Estável";
    }
    ultimaPressao = pAtual;

    // 2. Contador para atualizar API a cada 15 minutos (900.000 ms)
    static int contMinutos = 0;
    contMinutos++;
    if (contMinutos >= 15) {
      verificarAeroporto();
      contMinutos = 0;
    }
    timerGeral = millis();
  }

  // 3. Alarme Físico (LED Vermelho pisca se houver raios ou chuva iminente)
  if (alertaRaios || previsaoLocal.indexOf("⛈️") >= 0) {
    digitalWrite(LED_VERMELHO, (millis() / 500) % 2); // Pisca a cada 500ms
  } else {
    digitalWrite(LED_VERMELHO, LOW);
  }
  
  delay(10);
}
