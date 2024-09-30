#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "Hotspot-AOI";
const char* password = "12345678";

// Indirizzo IP statico
IPAddress local_IP(192, 168, 137, 219);
IPAddress gateway(10, 14, 96, 1);
IPAddress subnet(255, 255, 240, 0);


WebServer server(80);

int esp32Value = 0; // Variabile per memorizzare il valore

/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\*/
int esp32EncoderValue = 0; // variabile per memorizzare il valore dell'encoder
/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\*/

void setup() {
  Serial.begin(9600); // Seriale per la comunicazione con l'Arduino Uno

  // Configura IP statico
  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("STA Failed to configure");
  }

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());

  server.on("/command1", []() {
    Serial.println("command1");
    server.send(200, "text/plain", "Command 1 received");
  });

  server.on("/command2", []() {
    Serial.println("command2");
    server.send(200, "text/plain", "Command 2 received");
  });

  server.on("/command3", []() {
    Serial.println("command3");
    server.send(200, "text/plain", "Command 3 received");
  });

  server.on("/command4", []() {
    Serial.println("command4");
    server.send(200, "text/plain", "Command 4 received");
  });

  server.on("/command5", []() {
    Serial.println("command5");
    server.send(200, "text/plain", "Command 5 received");
  });

  server.on("/command6", []() {
    Serial.println("command6");
    server.send(200, "text/plain", "Command 6 received");
  });

  server.on("/command7", []() {
    Serial.println("command7");
    server.send(200, "text/plain", "Command 7 received");
  });

  server.on("/command8", []() {
    Serial.println("command8");
    server.send(200, "text/plain", "Command 8 received");
  });

  server.on("/command9", []() {
    Serial.println("command9");
    server.send(200, "text/plain", "Command 9 received");
  });

  server.on("/command10", []() {
    Serial.println("command10");
    server.send(200, "text/plain", "Command 10 received");
  });

  server.on("/command11", []() {
    Serial.println("command11");
    server.send(200, "text/plain", "Command 11 received");
  });

  server.on("/command12", []() {
    Serial.println("command12");
    server.send(200, "text/plain", "Command 12 received");
  });

  server.on("/command13", []() {
    Serial.println("command12");
    server.send(200, "text/plain", "Command 13 received");
  });
   server.on("/sendDegrees", []() {
    if (server.hasArg("degrees")) {
      String degrees = server.arg("degrees");
      Serial.println("Received degrees: " + degrees);
      server.send(200, "text/plain", "Degrees received: " + degrees);
    } else {
      server.send(400, "text/plain", "Degrees not provided");
    }
  });
  server.on("/getValue", []() { // Valore della fotoresistenza
    String response = "VALUE: " + String(esp32Value);
    server.send(200, "text/plain", response);
  });
  /*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\*/
  server.on("/getValueEncoder", []() { // Valore dell'encoder
    String response = "VALUE EN: " + String(esp32EncoderValue);
    server.send(200, "text/plain", response);
  });
  /*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\*/
  server.begin();
}

void loop() {
  server.handleClient();

  ////////////////////////////////////////////////////
  // Parte per visualizzare il grafico della fotoresistenza

  // Controlla se ci sono dati disponibili sulla seriale
  if (Serial.available() > 0) {
    // Leggi la stringa ricevuta
    String receivedString = Serial.readStringUntil('\n');
    // Controlla se il messaggio inizia con "VALUE:"
    if (receivedString.startsWith("VALUE:")) { // Valore fotoresistenza
      // Estrai il valore numerico dalla stringa ricevuta
      int value = receivedString.substring(6).toInt();  // 6 è la lunghezza di "VALUE:"
      // Salva il valore ricevuto nella variabile globale
      esp32Value = value;
      Serial.print("Received value from Arduino Uno: ");
      Serial.println(value);
    }  
    /*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\*/
    // Controlla se il messaggio inizia con "VALUE EN:"
    else if (receivedString.startsWith("VALUE EN:")) { // Valore encoder
      // Estrai il valore numerico dalla stringa ricevuta
      int value_en = receivedString.substring(9).toInt();  // 9 è la lunghezza di "VALUE EN:"
      // Salva il valore ricevuto nella variabile globale
      esp32EncoderValue = value_en;
      Serial.print("Received value encoder from Arduino Uno: ");
      Serial.println(value_en);
    }
    /*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\*/
  }
}



