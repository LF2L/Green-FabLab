#define LED 5 // DEFINIR LE NUMERO DE PIN POUR LA LED
#define VIN 6 // DEFINIR LE NUMERO DE PIN POUR VIN (SERVANT A ALIMENTER LA LED ON/OFF)
#define BUTTON 2 //DEFINIR LA PIN D'ENTREE OU LE BOUTTON EST CONECTE

#include "Stepper.h"// INCLURE LA PROCEDURE STEPPER DANS LE CODE POUR DECLARER LE MOTEUR



int potPin1 = A3;    // SELECTIONNER L'ENTREE A3 comme PIN pour le potentiomètre 1 (ANGLE)
long valPin1 = 0;       // Variable de stockage venant du potentiomètre
int potPin2 = A4;    // SELECTIONNER L'ENTREE A4 comme PIN pour le potentiomètre 2 (TEMPS)
long valPin2 = 0;       // Variable de stockage venant du potentiomètre

int ENA=12; //Connecté sur votre Arduino, Pin 12
int IN1=8; //Connecté sur votre Arduino, Pin 8
int IN2=9; //Connecté sur votre Arduino, Pin 9
int ENB=13; //Connecté sur votre Arduino, Pin 13
int IN3=10; //Connecté sur votre Arduino, Pin 10
int IN4=11; //Connecté sur votre Arduino, Pin 11
int VLED=3; //Connecté sur votre Arduino, Pin 3

int val = 0; // VAL EST UNE VARIABLE UTILISEE POUR STOCKER LA VALEUR RETOURNE PAR LA PIN ON/OFF
int old_val = 0; // OLD_VAL STOCK LA VALEUR PRECEDENTE DE VAL POUR VERIFIER L'ETAT ACTUEL
int state = 0; // 0 = LED OFF ET 1 = LED ON
int var = 0;
int valeur2=0;// valeur2 contient la valeur retourné par le capteur photorésistance
int SeuilBas=A2;// SeuilBas contient également la valeur retourné par le capteur photorésistance

long ANGLE = 0; //VALEUR DE L'ANGLE 
long TEMPS = 0; //VALEUR DE L'ANGLE

//pour un moteur de 200 pas par tour et brancher sur les broches 2, 4, 7, 8
Stepper moteur(200, 8,  9, 10, 11);

void setup() {
  // Initialise la communication avec le PC
   Serial.begin(9600);
   pinMode(ENA,OUTPUT); // ENA = SORTIE
   pinMode(ENB,OUTPUT); // ENB = SORTIE
   pinMode(IN1,OUTPUT); // IN1 = SORTIE
   pinMode(IN2,OUTPUT); // IN2 = SORTIE
   pinMode(IN3,OUTPUT); // IN3 = SORTIE
   pinMode(IN4,OUTPUT); // IN4 = SORTIE
   pinMode(LED, OUTPUT); // DIRE à ARDUINO QUE LED EST UNE SORTIE
   pinMode(VIN, OUTPUT); // DIRE à ARDUINO QUE VIN EST UNE SORTIE
   pinMode(BUTTON, INPUT); // DIRE à ARDUINO QUE BUTTON EST UNE SORTIE
   pinMode(VLED,OUTPUT);//METTRE SOUS TENBSION LES LED DE NIVEAU DE RESERVOIR
   
   moteur.setSpeed(30); //30 tours par minute
   //(rappel : ici le moteur fait 200 pas par tour, on fera donc 6000 pas par minute)
   
   digitalWrite(VIN, HIGH); // METTRE VIN SOUS TENSION POUR ALIMENTER LA LED ON/OFF
   digitalWrite(ENA,HIGH);// Activer moteur A
   digitalWrite(ENB,HIGH);// Activer moteur B
   digitalWrite(VLED,HIGH);//METTRE SOUS TENBSION LES LED DE NIVEAU DE RESERVOIR
}

void loop(){
  
  valPin1 = analogRead(potPin1);    // READ LA VALEUR DU POTARD 1 ( VALEUR D'ANGLE}
  ANGLE = (valPin1*150)/1024;
 // ANGLE = valPin1/10;
  
  Serial.println(valPin1); 
  Serial.println(ANGLE);
  valPin2 = analogRead(potPin2);    // READ LA VALEUR DU POTARD 2 ( VALEUR DE TEMPS}
  TEMPS = (valPin2*300)/1024;
  Serial.println(valPin2); 
  Serial.println(TEMPS); 
  delay(500);
  
  
val = digitalRead(BUTTON); // LIRE LA VALEUR D'ENTREE BUTTON ET LA STOCKER DANS VAL
   
if ((val == HIGH) && (old_val == LOW)) //VERIFIER SI LE CAS DE FIGURE SUIVANT EST VRAI
  {
   state = 1 - state; // SI C'EST VRAI ALORS MISE A 1 DE STATE POUR ALLUMER LED ON/OFF
   delay(10); // PAUSE
  }
  
old_val = val; // SAUVEGARDER L'ETAT PRECEDENT DE VAL DANS OLD_VAL

if (state == 1) 
  {
  digitalWrite(LED, HIGH); // METTRE LED ON/OFF EN ON
  
  //----LECTURE VALEUR SEUIL BAS (si SB<800 = OBJET sinon PAS OBJET) 
  valeur2 = analogRead(SeuilBas); // LECTURE VALEUR CAPTEUR PHOTORESISTANCE
  //Serial.println(valeur2); //RECUPERATION DE LA VALEUR LU DANS LA VARIABLE VALEUR 2
  delay(10);
    
  if (valeur2 > 800)
      {
          moteur.step(-(ANGLE*200)/360);//choisir le nombre de pas à faire (Moteur à 200 pas)
          var = 0;
          while(var < TEMPS){
            val = digitalRead(BUTTON); // LIRE LA VALEUR D'ENTREE BUTTON ET LA STOCKER DANS VAL
            if ((val == HIGH) && (old_val == LOW))
            {
              state = 1 - state; // SI C'EST VRAI ALORS MISE A 1 DE STATE POUR ALLUMER LED ON/OFF
              delay(10); // PAUSE
            }
              
            old_val = val; // SAUVEGARDER L'ETAT PRECEDENT DE VAL DANS OLD_VAL
            delay(10);
            var++;
          }
          moteur.step((ANGLE*200)/360);//choisir le nombre de pas à faire (Moteur à 200 pas)(Angle*200)/360
          delay(10);//délai d'attente entre deux mouvements
      }
      
  }
  else if (state == 0)
  {
     digitalWrite(LED, LOW); // METTRE LED ON/OFF EN OFF
     delay(10);
  }
}
