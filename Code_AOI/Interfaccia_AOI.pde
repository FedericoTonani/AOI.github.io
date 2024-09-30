import java.io.*;
import java.net.*;
import controlP5.*;

PImage img;  // Dichiarazione della variabile per l'immagine

ControlP5 cp5;
String inputText = "";
int sendButtonX = 50;
int sendButtonY = 650;
int sendButtonW = 50;
int sendButtonH = 50;
boolean sendFlag = false;

int buttonWidth = 250;
int buttonHeight = 100;
int leftColumnX = 50;
int rightColumnX = 200;

ArrayList<Float> values = new ArrayList<Float>();  // Lista per memorizzare i valori
float newValue = 0;
float max_value = 0;
float live_angle = 0; // Angolo del encoder

int maxValues = 100;  // Numero massimo di valori da visualizzare sul grafico
int graphWidth = 300;
int graphHeight = 200;

// Nuova posizione del grafico
int graphX = 1200;
int graphY = 700;

int interval = 330;  // Intervallo di aggiornamento in millisecondi (mezzo secondo)
int lastUpdateTime = 0;  // Tempo dell'ultimo aggiornamento

String esp32Value = ""; // Variabile globale per memorizzare il valore ricevuto
int esp32ValueInt = 0; // Variabile per memorizzare il valore come intero

/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\*/
int esp32ValueEncoder = 0; // Variabile per memorizzare il valore live dell'encoder
/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\*/

volatile boolean running = true; // Variabile per controllare l'esecuzione del thread

// Variabili per lampeggiamento del cerchio di Live Angle
boolean isCircleOn = true;
int blinkInterval = 500; // Intervallo di lampeggio in millisecondi
int lastBlinkTime = 0;

void setup() {
    size(1900, 1050);
    img = loadImage("computer-microchip-processor-wallpaper.jpg");  // Caricamento dell'immagine
    img.resize(width, height);  // Ridimensiona l'immagine alla dimensione della finestra
    //background(200);

    drawInterface();
    cp5 = new ControlP5(this);

    cp5.addTextfield("Input °")
        .setPosition(340, 810)
        .setSize(140, 40)
        .setFont(createFont("Arial", 28))
        .setFocus(true)
        .setColor(color(0, 0, 0))
        .setColorBackground(color(255, 255, 255))
        .setColorForeground(color(255, 20, 100))
        .setColorActive(color(0))
        .setColorValueLabel(color(0, 0, 0))
        .getCaptionLabel()
        .setFont(createFont("Arial", 24))
        .toUpperCase(false);

    cp5.addLabel("infoLabel")
        .setText("°")
        .setPosition(420, 816)
        .setColor(color(0))
        .setFont(createFont("Arial", 32));

    cp5.addBang("send")
        .setPosition(500, 810)
        .setSize(45, 40)
        .setLabel("Send")
        .setColorBackground(color(255, 0, 255))
        .setColorForeground(color(255, 255, 255))
        .setColorActive(color(0, 250, 100))
        .getCaptionLabel()
        .setFont(createFont("Arial", 18))
        .toUpperCase(false);

    // Crea e avvia un thread per chiamare getESP32Value periodicamente
    Thread thread = new Thread(new Runnable() {
        public void run() {
            while (running) {
                getESP32Value();
                getESP32EncoderValue();
                try {
                    Thread.sleep(350); // Pausa di 1 secondo
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }
    });
    thread.start();
}

void draw() {
   // background(0, 0, 0);
    // background(img);  // Imposta l'immagine come sfondo
    image(img, 0, 0);  // Disegna l'immagine come sfondo
    
    stroke(255);
    strokeWeight(10);
    line(900, 30, 900, 970);
    line(30, 105, 1770, 105);
    
    stroke(255);
    strokeWeight(5);
    line(1393, 935, 1510, 935);
    line(1109, 935, 1218, 935);

    stroke(255);
    strokeWeight(9);
    int posX = (900) / 2 - buttonWidth / 2;
    fill(255, 255, 255);
    rect(posX, 50, buttonWidth, buttonHeight);
    fill(0);
    textAlign(CENTER, CENTER);
    textFont(createFont("Bauhaus 93", 55));
    text("Manuale", posX + buttonWidth / 2, 50 + buttonHeight / 2);

    int posX1 = (900 + 1800) / 2 - buttonWidth / 2;
    fill(255, 255, 255);
    rect(posX1, 50, buttonWidth, buttonHeight);
    fill(0);
    textAlign(CENTER, CENTER);
    textFont(createFont("Bauhaus 93", 50));
    text("Dati Live", posX1 + buttonWidth / 2, 50 + buttonHeight / 2);

    //textFont(createFont("Arial", 32));
    textFont(createFont("Bauhaus 93", 40));
    stroke(0,0,0);
    strokeWeight(0);
    drawInterface();

    //Angolo live letto dall'encoder
    fill(0,250,100);
    textAlign(CENTER, CENTER);
    textFont(createFont("Bauhaus 93", 35));
    text("Live Angle: ", 1300, 295);
    live_angle = (float(esp32ValueEncoder) * 360.0) / 40000.0;
    //drawCircleWithValue(1450, 295, 115, esp32ValueInt);
    drawCircleWithValue(1450, 295, 135, live_angle);

    stroke(0,0,0);
    strokeWeight(0);
    
    // Lampeggio del cerchio di live angle
    if (millis() - lastBlinkTime > blinkInterval) {
        isCircleOn = !isCircleOn;
        lastBlinkTime = millis();
    }

    if (isCircleOn) {
        fill(0, 255, 0); // Verde
    } else {
        fill(255, 255, 255, 0); // Nero (colore di sfondo)
    }
    ellipse(1195, 295, 15, 15); // Piccolo cerchio verde lampeggiante
    
    fill(0, 255, 0); // Verde
    ellipse(1080, 920, 15, 15); // Piccolo cerchio verde
    ellipse(1365, 920, 15, 15); // Piccolo cerchio verde
    
    fill(255); // Ripristina il colore di riempimento bianco per il resto del testo e degli elementi

    fill(0,250,100);
    textAlign(CENTER, CENTER);
    textFont(createFont("Bauhaus 93", 35));
    text("Self - Centering System Plot", 1345, 445);
    
    if (sendFlag) {
        sendInputValue();
        sendFlag = false;
    }
    grafico();
}

void drawInterface() {
    float raggio = 93 / 2.0;
    drawButton(380, 230, raggio, "10°");
    drawButton(520, 230, raggio, "20°");
    drawButton(380, 330, raggio, "30°");
    drawButton(520, 330, raggio, "40°");
    drawButton(380, 430, raggio, "50°");
    drawButton(520, 430, raggio, "60°");
    drawButton(380, 530, raggio, "65°");
    drawButton(520, 530, raggio, "70°");
    drawButton(380, 630, raggio, "75°");
    drawButton(520, 630, raggio, "80°");
    drawButton(380, 730, raggio, "85°");
    drawButton(520, 730, raggio, "90°");
}

void drawButton(float x, float y, float raggio, String label) {
    float d = dist(mouseX, mouseY, x, y);
    if (d < raggio) {
        fill(0, 250, 100);
    } else {
        fill(255, 255, 255);
    }
    ellipse(x, y, raggio * 2, raggio * 2);
    fill(0);
    textAlign(CENTER, CENTER);
    text(label, x, y);
}

void mousePressed() {
    float raggio = 85 / 2.0;
    String inputText = cp5.get(Textfield.class, "Input °").getText();

    if (dist(mouseX, mouseY, 380, 210) < raggio) {
        sendCommand("command1");
    } else if (dist(mouseX, mouseY, 520, 210) < raggio) {
        sendCommand("command2");
    } else if (dist(mouseX, mouseY, 380, 310) < raggio) {
        sendCommand("command3");
    } else if (dist(mouseX, mouseY, 520, 310) < raggio) {
        sendCommand("command4");
    } else if (dist(mouseX, mouseY, 380, 410) < raggio) {
        sendCommand("command5");
    } else if (dist(mouseX, mouseY, 520, 410) < raggio) {
        sendCommand("command6");
    } else if (dist(mouseX, mouseY, 380, 510) < raggio) {
        sendCommand("command7");
    } else if (dist(mouseX, mouseY, 520, 510) < raggio) {
        sendCommand("command8");
    } else if (dist(mouseX, mouseY, 380, 610) < raggio) {
        sendCommand("command9");
    } else if (dist(mouseX, mouseY, 520, 610) < raggio) {
        sendCommand("command10");
    } else if (dist(mouseX, mouseY, 380, 710) < raggio) {
        sendCommand("command11");
    } else if (dist(mouseX, mouseY, 520, 710) < raggio) {
        sendCommand("command12");
    }
}

void send() {
    sendFlag = true;
}

void sendInputValue() {
    String degrees = cp5.get(Textfield.class, "Input °").getText();
    println("Degrees to send: " + degrees);
    sendCommand("sendDegrees", degrees);
}

void sendCommand(String command) {
    sendCommand(command, "");
}

void sendCommand(String command, String degrees) {
    String server = "192.168.137.219";
    String url = "http://" + server + "/" + command;
    if (!degrees.equals("")) {
        url += "?degrees=" + degrees;
    }
    try {
        URL serverUrl = new URL(url);
        HttpURLConnection connection = (HttpURLConnection) serverUrl.openConnection();
        connection.setRequestMethod("GET");
        connection.connect();

        BufferedReader in = new BufferedReader(new InputStreamReader(connection.getInputStream()));
        String inputLine;
        while ((inputLine = in.readLine()) != null) {
            println(inputLine);
        }
        in.close();
    } catch (Exception e) {
        e.printStackTrace();
    }
}

void getESP32Value() {
    String server = "192.168.137.219";
    String url = "http://" + server + "/getValue";

    try {
        URL serverUrl = new URL(url);
        HttpURLConnection connection = (HttpURLConnection) serverUrl.openConnection();
        connection.setRequestMethod("GET");
        connection.connect();

        int responseCode = connection.getResponseCode();
        println("Response Code: " + responseCode);

        if (responseCode == 200) {
            BufferedReader in = new BufferedReader(new InputStreamReader(connection.getInputStream()));
            String inputLine;
            while ((inputLine = in.readLine()) != null) {
                println("Received from ESP32: " + inputLine);
                if (inputLine.startsWith("VALUE:")) {
                    esp32Value = inputLine.substring(6).trim(); // Estrae il valore dopo "VALUE:"
                    esp32ValueInt = Integer.parseInt(esp32Value); // Converte la stringa in un intero
                    println("ESP32 Value: " + esp32ValueInt);
                }
            }
            in.close();
        } else {
            println("HTTP error code: " + responseCode);
        }

        connection.disconnect();
    } catch (Exception e) {
        e.printStackTrace();
    }
}

void stop() {
    running = false;
    super.stop();
}

void drawCircleWithValue(float x, float y, float diameter, float value) {
    fill(255, 255, 255);
    ellipse(x, y, diameter, diameter); // Disegna la circonferenza
    fill(0);
    textAlign(CENTER, CENTER);
    textSize(32);
    text(String.format("%.3f°", live_angle), x, y);  // Mostra 2 cifre decimali
}

void grafico() {
    // Intervallo di aggiornamento
    if (millis() - lastUpdateTime > interval) {
        lastUpdateTime = millis();
        
        values.add(float(esp32ValueInt));
        
        // Aggiornamento del massimo valore
        if (max_value < esp32ValueInt) {
            max_value = esp32ValueInt;
        }
        
        // Rimozione del valore più vecchio se la lista supera maxValues
        if (values.size() > maxValues) {
            values.remove(0);
        }
    }
  
    // Dimensioni del grafico
    int enlargedGraphWidth = 550; // Larghezza del grafico ingrandito
    int enlargedGraphHeight = 425; // Altezza del grafico ingrandito
    int xOffset = 125; // Spostamento a destra
    int yOffset = 200; // Spostamento verso il basso
  
    // Disegno del rettangolo del grafico
    fill(255);
    rect(graphX - xOffset, graphY - enlargedGraphHeight + yOffset, enlargedGraphWidth, enlargedGraphHeight);
  
    // Impostazioni del tratto
    stroke(0);
    strokeWeight(3); // Imposta lo spessore della linea
    noFill();
  
    // Inizio della forma
    beginShape();
    for (int i = 0; i < values.size(); i++) {
        // Mapping delle coordinate x e y con le dimensioni del grafico ingrandito e lo spostamento
        float x = map(i, 0, maxValues, graphX - xOffset, graphX - xOffset + enlargedGraphWidth);
        float y = map(values.get(i), 1000, 000, graphY - enlargedGraphHeight + yOffset, graphY + yOffset);
        vertex(x, y);
    }
    // Fine della forma
    endShape();
  
    // Impostazioni del testo
    fill(255);
    textSize(25);
    // Testo dell'ultimo valore
    text("Last Value: " + esp32ValueInt, graphX - xOffset + 100, graphY + yOffset + 20);
    // Testo del valore massimo
    text("Max Value: " + max_value, graphX - xOffset + 400, graphY + yOffset + 20);
}

/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\*/
void getESP32EncoderValue() {
    String server = "192.168.137.219";
    String url = "http://" + server + "/getValueEncoder";

    try {
        URL serverUrl = new URL(url);
        HttpURLConnection connection = (HttpURLConnection) serverUrl.openConnection();
        connection.setRequestMethod("GET");
        connection.connect();

        int responseCode = connection.getResponseCode();
        println("Response Code: " + responseCode);

        if (responseCode == 200) {
            BufferedReader in = new BufferedReader(new InputStreamReader(connection.getInputStream()));
            String inputLine;
            while ((inputLine = in.readLine()) != null) {
                println("Received from ESP32: " + inputLine);
                if (inputLine.startsWith("VALUE EN:")) {
                    esp32Value = inputLine.substring(9).trim(); // Estrae il valore dopo "VALUE:"
                    esp32ValueEncoder = Integer.parseInt(esp32Value); // Converte la stringa in un intero
                    println("ESP32 Value: " + esp32ValueEncoder);
                }
            }
            in.close();
        } else {
            println("HTTP error code: " + responseCode);
        }
        connection.disconnect();
    } catch (Exception e) {
        e.printStackTrace();
    }
}
/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\*/
