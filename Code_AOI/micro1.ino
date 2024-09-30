#include <TimerOne.h>
#include "MotorControl.h"
#include <math.h>

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Definizione dei pin
const int stepPin = 9;  // Pin per il segnale di passo
const int dirPin = 5;   // Pin per il segnale di direzione
//const int fotoPin = A0; // Pin fotoresistenza
//const int finecorsaPin = A1; // Pin finecorsa
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Definizione dei pin per i canali A e B encoder
const int pinA = 2; // Canale A dell'encoder collegato al pin 2 (interrupt 0) cavo bianco
const int pinB = 3; // Canale B dell'encoder collegato al pin 3 (interrupt 1) cavo rosa
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Variabile per contare gli impulsi dell'encoder
volatile long encoderCount = 0;
volatile int lastEncoded = 0;
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Variabile per memorizzare la posizione attuale dell'encoder
long currentPosition = 0;
long save_position_encoder = 0;
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Variabili di spostamento
int steps = 0;
float new_steps = 0;
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Variabili per memoria
int save_position = 0;
float floatDegrees = 0.0; // Variabile per memorizzare il valore con la virgola fornito dall'interfaccia
int roundedDegrees = 0;   // Variabile per memorizzare il valore arrotondato
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//Variabili per inviare valori a esp32
unsigned long previousMillis = 0;  // Variabile per memorizzare il tempo dell'ultimo aggiornamento della fotoresistenza
unsigned long encoderpreviousMillis = 0; // Memorizza tempo ultimo aggiornamento encoder
const long interval = 330;         // Intervallo desiderato in millisecondi (350 millisecondi)
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//Variabili finecorsa
bool emergencyStopFlag = false;
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//Variabili fotoresistenza
int Value_fotoresistenza = 0;
int Max_value = 0;
const int Threshold = 640;
bool FotoresistenzaFlag = false;
volatile int Posizione_centro = 0;
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Oggetto MotorControl
MotorControl motor(stepPin, dirPin);

void setup() {
  Serial.begin(9600);
  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Inizializzazione dei pin come input
  pinMode(pinA, INPUT);
  pinMode(pinB, INPUT);

  // Abilitazione degli interrupt sui pin A e B
  attachInterrupt(digitalPinToInterrupt(pinA), updateEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(pinB), updateEncoder, CHANGE);
  /////////////////////////////////////////////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Timer1 per generare un interrupt ogni 100 ms
  Timer1.initialize(100000); // 100 ms in microsecondi
  Timer1.attachInterrupt(checkInduttivo);

  motor.begin();
}

void loop() {
  // Disabilitare gli interrupt per leggere in modo sicuro la variabile encoderCount
  noInterrupts();
  currentPosition = encoderCount;
  sendEncoderValueToESP32(currentPosition);
  interrupts();

  // Stampare la posizione attuale dell'encoder sulla seriale
  Serial.print("Posizione attuale: ");
  Serial.println(currentPosition);
  sendValueToESP32(analogRead(A2));

  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim(); // Rimuove spazi bianchi iniziali e finali, incluso '\n'
    
    Serial.println("Command received: " + command); // Stampa il comando ricevuto
    
    if (command.equals("command1")) { // Comando per 10°
      if(save_position < 1667){
        steps = 1667 - save_position;
        motor.setDirection(true);
        motor.moveSteps(steps, 100); 
        while (motor.isRunning()) {
         sendValueToESP32(analogRead(A2));
         sendEncoderValueToESP32(encoderCount);
         motor.update();
         finecorsa();
        }
        save_position = 1667;
        control_with_feedback();
      }
      if(save_position > 1667){
        steps = save_position - 1667;
        motor.setDirection(false);
        motor.moveSteps(steps, 100); 
        while (motor.isRunning()) {
         sendValueToESP32(analogRead(A2));
         sendEncoderValueToESP32(encoderCount);
         motor.update();
         finecorsa();
        }
        save_position = 1667;
        control_with_feedback();
      }   
    } else if (command.equals("command2")) { // Comando per 20°
      if(save_position < 3333){
        steps = 3333 - save_position;
        motor.setDirection(true);
        motor.moveSteps(steps, 100); 
        while (motor.isRunning()) {
         sendValueToESP32(analogRead(A2));
         sendEncoderValueToESP32(encoderCount);
         motor.update();
         finecorsa();
        }
        save_position = 3333;
        control_with_feedback();
      }
      if(save_position > 3333){
        steps = save_position - 3333;
        motor.setDirection(false);
        motor.moveSteps(steps, 100); 
        while (motor.isRunning()) {
         sendValueToESP32(analogRead(A2));
         sendEncoderValueToESP32(encoderCount);
         motor.update();
         finecorsa();
        }
        save_position = 3333;
        control_with_feedback();
      } 
    } else if (command.equals("command3")) { // Comando per 30°
      if(save_position < 5000){
        steps = 5000 - save_position;
        motor.setDirection(true);
        motor.moveSteps(steps, 100); 
        while (motor.isRunning()) {
         sendValueToESP32(analogRead(A2));
         motor.update();
         finecorsa();
        }
        save_position = 5000;
        control_with_feedback();
      }
      if(save_position > 5000){
        steps = save_position - 5000;
        motor.setDirection(false);
        motor.moveSteps(steps, 100); 
        while (motor.isRunning()) {
         sendValueToESP32(analogRead(A2));
         motor.update();
         finecorsa();
        }
        save_position = 5000;
        control_with_feedback();
      } 
    } else if (command.equals("command4")) { // Comando per 40°
      if(save_position < 6667){
        steps = 6667 - save_position;
        motor.setDirection(true);
        motor.moveSteps(steps, 100); 
        while (motor.isRunning()) {
         sendValueToESP32(analogRead(A2));
         motor.update();
         finecorsa();
        }
        save_position = 6667;
        control_with_feedback();
      }
      if(save_position > 6667){
        steps = save_position - 6667;
        motor.setDirection(false);
        motor.moveSteps(steps, 100); 
        while (motor.isRunning()) {
         sendValueToESP32(analogRead(A2));
         motor.update();
         finecorsa();
        }
        save_position = 6667;
        control_with_feedback();
      } 
    } else if (command.equals("command5")) { // Comando per 50°
      if(save_position < 8333){
        steps = 8333 - save_position;
        motor.setDirection(true);
        motor.moveSteps(steps, 100); 
        while (motor.isRunning()) {
         sendValueToESP32(analogRead(A2));
         motor.update();
         finecorsa();
        }
        save_position = 8333;
        control_with_feedback();
      }
      if(save_position > 8333){
        steps = save_position - 8333;
        motor.setDirection(false);
        motor.moveSteps(steps, 100); 
        while (motor.isRunning()) {
         sendValueToESP32(analogRead(A2));
         motor.update();
         finecorsa();
        }
        save_position = 8333;
        control_with_feedback();
      } 
    } else if (command.equals("command6")) { // Comando per 60°
      if(save_position < 10000){
        steps = 10000 - save_position;
        motor.setDirection(true);
        motor.moveSteps(steps, 100); 
        while (motor.isRunning()) {
         sendValueToESP32(analogRead(A2));
         motor.update();
         finecorsa();
        }
        save_position = 10000;
        control_with_feedback();
      }
      if(save_position > 10000){
        steps = save_position - 10000;
        motor.setDirection(false);
        motor.moveSteps(steps, 100); 
        while (motor.isRunning()) {
         sendValueToESP32(analogRead(A2));
         motor.update();
         finecorsa();
        }
        save_position = 10000;
        control_with_feedback();
      } 
    } else if (command.equals("command7")) { // Comando per 65°
      if(save_position < 10833){
        steps = 10833 - save_position;
        motor.setDirection(true);
        motor.moveSteps(steps, 100); 
        while (motor.isRunning()) {
         sendValueToESP32(analogRead(A2));
         motor.update();
         finecorsa();
        }
        save_position = 10833;
        control_with_feedback();
      }
      if(save_position > 10833){
        steps = save_position - 10833;
        motor.setDirection(false);
        motor.moveSteps(steps, 100); 
        while (motor.isRunning()) {
         sendValueToESP32(analogRead(A2));
         motor.update();
         finecorsa();
        }
        save_position = 10833;
        control_with_feedback();
      } 
    } else if (command.equals("command8")) { // Comando per 70°
      if(save_position < 11667){
        steps = 11667 - save_position;
        motor.setDirection(true);
        motor.moveSteps(steps, 100); 
        while (motor.isRunning()) {
         sendValueToESP32(analogRead(A2));
         motor.update();
         finecorsa();
        }
        save_position = 11667;
        control_with_feedback();
      }
      if(save_position > 11667){
        steps = save_position - 11667;
        motor.setDirection(false);
        motor.moveSteps(steps, 100); 
        while (motor.isRunning()) {
         sendValueToESP32(analogRead(A2));
         motor.update();
         finecorsa();
        }
        save_position = 11667;
        control_with_feedback();
      } 
    } else if (command.equals("command9")) { // Comando per 75°
      if(save_position < 12500){
        steps = 12500 - save_position;
        motor.setDirection(true);
        motor.moveSteps(steps, 100); 
        while (motor.isRunning()) {
         sendValueToESP32(analogRead(A2));
         motor.update();
         finecorsa();
        }
        save_position = 12500;
        control_with_feedback();
      }
      if(save_position > 12500){
        steps = save_position - 12500;
        motor.setDirection(false);
        motor.moveSteps(steps, 100); 
        while (motor.isRunning()) {
         sendValueToESP32(analogRead(A2));
         motor.update();
         finecorsa();
        }
        save_position = 12500;
        control_with_feedback();
      } 
    } else if (command.equals("command10")) { // Comando per 80°
      if(save_position < 13333){
        steps = 13333 - save_position;
        motor.setDirection(true);
        motor.moveSteps(steps, 100); 
        while (motor.isRunning()) {
         sendValueToESP32(analogRead(A2));
         motor.update();
         finecorsa();
        }
        save_position = 13333;
        control_with_feedback();
      }
      if(save_position > 13333){
        steps = save_position - 13333;
        motor.setDirection(false);
        motor.moveSteps(steps, 100); 
        while (motor.isRunning()) {
         sendValueToESP32(analogRead(A2));
         motor.update();
         finecorsa();
        }
        save_position = 13333;
        control_with_feedback();
      } 
    } else if (command.equals("command11")) { // Comando per 85°
      if(save_position < 14167){
        steps = 14167 - save_position;
        motor.setDirection(true);
        motor.moveSteps(steps, 100); 
        while (motor.isRunning()) {
         sendValueToESP32(analogRead(A2));
         motor.update();
         finecorsa();
        }
        save_position = 14167;
        control_with_feedback();
      }
      if(save_position > 14167){
        steps = save_position - 14167;
        motor.setDirection(false);
        motor.moveSteps(steps, 100); 
        while (motor.isRunning()) {
         sendValueToESP32(analogRead(A2));
         motor.update();
         finecorsa();
        }
        save_position = 14167;
        control_with_feedback();
      } 
    } else if (command.equals("command12")) { // Comando per 90°
      if(save_position < 15000){
        steps = 15000 - save_position;
        motor.setDirection(true);
        motor.moveSteps(steps, 100); 
        while (motor.isRunning()) {
         sendValueToESP32(analogRead(A2));
         motor.update();
         finecorsa();
        }
        save_position = 15000;
        control_with_feedback();
      }
      if(save_position > 15000){
        steps = save_position - 15000;
        motor.setDirection(false);
        motor.moveSteps(steps, 100); 
        while (motor.isRunning()) {
         sendValueToESP32(analogRead(A2));
         motor.update();
         finecorsa();
        }
        save_position = 15000;
        control_with_feedback();
      } 
    } else if (command.startsWith("Received degrees: ")) { // Comando per ricevere valore con virgola
        String degreeValue = command.substring(18); // Ottiene il valore dei gradi dopo "Received degrees: "
        degreeValue.trim(); // Rimuove eventuali spazi bianchi
        degreeValue.replace(',', '.'); // Sostituisce la virgola con il punto decimale
        Serial.print("Extracted degree value: ");
        Serial.println(degreeValue);
        
        floatDegrees = degreeValue.toFloat();
        Serial.print("Degrees received and stored: ");
        Serial.println(floatDegrees);

        // Calcola i passi necessari per raggiungere il grado ricevuto
        new_steps = (60000 * floatDegrees) / 360;
        roundedDegrees = round(new_steps);
        Serial.print("Calculated steps (rounded degrees): ");
        Serial.println(roundedDegrees);

        if((save_position < roundedDegrees) && (roundedDegrees <= 15000)){
          Serial.println("Entering the 'Received degrees' block"); // Debug
          steps = roundedDegrees - save_position;
          motor.setDirection(true);
          motor.moveSteps(steps, 100); 
          while (motor.isRunning()) {
            sendValueToESP32(analogRead(A2));
            motor.update();
            finecorsa();
          }
          save_position = roundedDegrees;
/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\*/
          control_with_feedback();
/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\*/
        }
        else if((save_position > roundedDegrees) && (roundedDegrees <= 15000)){
          steps = save_position - roundedDegrees;
          motor.setDirection(false);
          motor.moveSteps(steps, 100); 
          while (motor.isRunning()) {
            sendValueToESP32(analogRead(A2));
            motor.update();
            finecorsa();
          }
          save_position = roundedDegrees;
/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\*/
          control_with_feedback();
/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\*/
        }
        else {
          Serial.println("Invalid steps range or already at position");
        }
    } else {
        Serial.println("Unknown command");
    }
  }
  finecorsa();
}

void sendValueToESP32(int value) {

  unsigned long currentMillis = millis();  // Leggi il tempo attuale
  
  // Controlla se è trascorso l'intervallo desiderato
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;  // Salva il tempo dell'ultimo aggiornamento
    Serial.print("VALUE:");
    Serial.println(value);
  }
}
/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\*/
/*             FUNZIONI ENCODER             */
/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\/*\*/

void updateEncoder() {
  // Lettura dei segnali A e B
  int MSB = digitalRead(pinA); // Most significant bit
  int LSB = digitalRead(pinB); // Least significant bit

  // Combinazione dei segnali A e B in una singola variabile
  int encoded = (MSB << 1) | LSB;
  int sum = (lastEncoded << 2) | encoded;

  // Determinazione della direzione di rotazione e aggiornamento del conteggio
  if (sum == 0b0010 || sum == 0b0100 || sum == 0b1101 || sum == 0b1011) encoderCount++;
  if (sum == 0b0001 || sum == 0b0111 || sum == 0b1110 || sum == 0b1000) encoderCount--;

  // Memorizzazione dell'ultimo valore codificato
  lastEncoded = encoded;
}

void sendEncoderValueToESP32(int value_en) {

  unsigned long currentMillis = millis();  // Leggi il tempo attuale
  
  // Controlla se è trascorso l'intervallo desiderato
  if (currentMillis - encoderpreviousMillis >= interval) {
    encoderpreviousMillis = currentMillis;  // Salva il tempo dell'ultimo aggiornamento
    Serial.print("VALUE EN:");
    Serial.println(value_en);
  }
}

void control_with_feedback(){
          do{
                noInterrupts();
                currentPosition = encoderCount;
                interrupts();
                save_position_encoder = currentPosition * 1.5;
                Serial.print("sve : ");
                Serial.println(save_position_encoder);
                if(save_position_encoder > save_position){
                  steps = save_position_encoder - save_position;
                  motor.setDirection(false);
                  motor.moveSteps(steps, 5); 
                  while (motor.isRunning()) {
                    sendValueToESP32(analogRead(A2));
                    sendEncoderValueToESP32(encoderCount);
                    motor.update();
                  }
                }
                if(save_position_encoder < save_position){
                  steps = save_position - save_position_encoder;
                  motor.setDirection(true);
                  motor.moveSteps(steps, 5); 
                  while (motor.isRunning()) {
                    sendValueToESP32(analogRead(A2));
                    sendEncoderValueToESP32(encoderCount);
                    motor.update();
                  }
                }
                noInterrupts();
                currentPosition = encoderCount;
                interrupts();
                save_position_encoder = currentPosition * 1.5;
          }while(abs(save_position_encoder - save_position) > 1 );
}
// Funzione di interrupt software rilevamento finecorsa
void checkInduttivo() {
  if (analogRead(A1) < 350) { // Soglia per finecorsa
    emergencyStopFlag = true;
  } 
}

void checkFotoresistenza(){
  Value_fotoresistenza = analogRead(A2);
}

void finecorsa () {
 if (emergencyStopFlag) {
    motor.moveSteps(0,0);
  }
}

void self_centering () {
  save_position = 10000;
  motor.setDirection(true);
  motor.moveSteps(10000, 30); 
  while (motor.isRunning()) {
    sendValueToESP32(analogRead(A2));
    motor.update();
    finecorsa();
    if(Value_fotoresistenza > Max_value){
      Posizione_centro = lastEncoded;
    }
  }
  control_with_feedback();
  if(Max_value > 600){
    FotoresistenzaFlag = true;
  }

      if(FotoresistenzaFlag){
      steps = 10000 - Posizione_centro;
      motor.setDirection(false);
      motor.moveSteps(steps, 30);
      while (motor.isRunning()) {
        sendValueToESP32(analogRead(A2));
        motor.update();
        finecorsa();
      }
      save_position = Posizione_centro;
      control_with_feedback();
    }else{
      motor.setDirection(false);
      motor.moveSteps(10000, 30);
      while (motor.isRunning()) {
       sendValueToESP32(analogRead(A2));
       motor.update();
       finecorsa();
       if(Value_fotoresistenza > Max_value){
         Posizione_centro = lastEncoded;
        }
      }
      save_position = 10000;
      control_with_feedback();     
      steps = 10000 - Posizione_centro;
      motor.setDirection(true);
      motor.moveSteps(steps, 30);
      while (motor.isRunning()) {
        sendValueToESP32(analogRead(A2));
        motor.update();
        finecorsa();
      }
      save_position = Posizione_centro;
      control_with_feedback();
    }
}
