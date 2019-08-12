 /*
  Lora Send And Receive
  This sketch demonstrates how to send and receive data with the MKR WAN 1300 LoRa module.
  This example code is in the public domain.
*/

#include <MKRWAN.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <string.h>


#define ONE_WIRE_BUS 2

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
LoRaModem modem;

int sensorPin = A0;
int sensorValue = 0;
String result;
int moteurPin = 13;                     // Sortie où sera branchée la moteur
int moteurState = LOW; 
// Uncomment if using the Murata chip as a module
// LoRaModem modem(Serial1);

#include "arduino_secrets.h"
// Please enter your sensitive data in the Secret tab or arduino_secrets.h
String appEui = SECRET_APP_EUI;
String appKey = SECRET_APP_KEY;

void setup() {
  Serial.begin(115200);
    sensors.begin();
    pinMode(moteurPin, OUTPUT);
  while (!Serial);
  // selection de la frequence correspondante à la zone (eg. US915, AS923, ...)
  if (!modem.begin(EU868)) {
    Serial.println("Echec de demarage du module");
    while (1) {}
  };
  //_____________________________________________________________ Recuperation de l'identifiant de l'objet
  Serial.print("Votre version est : ");
  Serial.println(modem.version());
  Serial.print("L\'identifiant EIU de votre materiel est : ");
  Serial.println(modem.deviceEUI());
//________________________________________________________________Connection au serveur TTN
  int connected = modem.joinOTAA(appEui, appKey);
  if (!connected) {
    Serial.println("Une erreur s\'est produite. Etes vous dans un batiment? Deplacez vous vers la fenêtre");
    while (1) {}
  }
  // Fixe linterval de temps à 120 secs.
  modem.minPollInterval(120);
}

void loop() {
  //______________________________________________________________Recuperation des données de temperature
  delay(30000);
  Serial.println();
  Serial.println("Entrer le message à envoyer au reseau");
  Serial.println("(taper entrer apres avoir renseigner la donnée)");
  Serial.println("La temperature est: ");
  sensors.requestTemperatures();
 
  Serial.print(sensors.getTempCByIndex(0));
  ////////////////////// // HUMIDITE
  int temperature;
  temperature=sensors.getTempCByIndex(0);

  result = result + temperature;
  //______________________________________________________________Recuperation des données de l'humidité
  sensorValue = analogRead(sensorPin);
  Serial.print("Moisture = " );

  
  sensorValue =map(sensorValue, 0,1023,0,100);
  Serial.println(sensorValue);
  Serial.print("\nHumidity ___________\n");
  Serial.print(sensorValue);
  result = result + sensorValue;
   if(sensorValue < 20) 
      {           // Si "A" est reçu
          Serial.print("Et que la Moteur est allumée");
        if (moteurState==HIGH) 
        {          // Et que la Moteur est allumée
          Serial.print("Et que la Moteur est allumée");
              moteurState = LOW; 
              digitalWrite(moteurPin, moteurState);    
              delay(60000);
              moteurState = HIGH; 
              digitalWrite(moteurPin, moteurState);// On l'éteint
        }
        else 
        {                          //Sinon
              moteurState = HIGH;  
              digitalWrite(moteurPin, moteurState); 
               delay(60000);
               moteurState = LOW; 
               digitalWrite(moteurPin, moteurState);// On l'allume
        }
        digitalWrite(moteurPin, moteurState);  // Enfin on change l'état de la Moteur
      }
 
  Serial.println(result);
  
  //temperature =sensors.getTempCByIndex(0);

//   int received;                     // Variable servant à récupérer les données reçues
//  if (Serial.available()>0) 
//  {       // Si des données sont disponibles
//  received = Serial.read();       // On les récupère
//      if(received == 'a') 
//      {           // Si "a" est reçu
//        if (moteurState==HIGH) 
//        {          // Et que la Moteur est allumée
//              moteurState = LOW;              // On l'éteint
//        }
//        else 
//        {                          //Sinon
//              moteurState = HIGH;             // On l'allume
//        }
//        digitalWrite(moteurPin, moteurState);  // Enfin on change l'état de la Moteur
//      }
//  }
//____________________________________________________lecture des données reçues sur la liaison serie
  if (!Serial.available());
  String msg = Serial.readStringUntil('\n');

  Serial.println();
  Serial.print("Sending: " + msg + " - ");
  for (unsigned int i = 0; i < msg.length(); i++) {
    Serial.print(msg[i] >> 4, HEX);
    Serial.print(msg[i] & 0xF, HEX);
    Serial.print(" ");
  }
  Serial.println();
  
//____________________________________________________Envoie des données vers le serveur TTN en UPLINK
  int err;
  modem.beginPacket();
  modem.print(result);
  result="";
  
  err = modem.endPacket(true);
  if (err > 0) {
    Serial.println("Message envoyé avec succès!");
  } else {
    Serial.println("Erreur lors de l'envoie du message :(");
    Serial.println("(Vous pouvez envoyer un nombre limité de message par minute, en fonce de la force du signal");
    Serial.println("Cela peux varier d'un message chaque dizaines de secondes à un message chaque minute)");
  }

//____________________________________________________Traitement des données reçues du serveur TTN en DOWNLINK 
  if (!modem.available()) {
    Serial.println("Aucun message reçu en downlink pour le moment.");
    return;
  }
  char rcv[64];
  int i = 0;
  while (modem.available()) {
    rcv[i++] = (char)modem.read();
  }
  Serial.print("Received: ");

 Serial.print("\nLe caractère 1 reçu est : ");    // Variable servant à récupérer les données reçues
  Serial.print(rcv[0]);
   char rcv1;
   Serial.print("\nLe caractère 2 reçu est : "); 
   rcv1=rcv[0] >> 4;
   Serial.print(rcv1);
   char received;
   received=rcv[0];
   Serial.print(received);
// Serial.print(test);
     delay(60000);
 //____________________________________________________Pilotage de la valve electrique
      if(rcv[0] == 'A' or temperature > 20 or sensorValue < 20) 
      {           // Si "A" est reçu
          Serial.print("Et que la Moteur est allumée");
        if (moteurState==HIGH) 
        {          // Et que la Moteur est allumée
          Serial.print("Et que la Moteur est allumée");
              moteurState = LOW; 
              digitalWrite(moteurPin, moteurState);    
              delay(60000);
              moteurState = HIGH; 
              digitalWrite(moteurPin, moteurState);// On l'éteint
        }
        else 
        {                          //Sinon
              moteurState = HIGH;  
              digitalWrite(moteurPin, moteurState); 
               delay(60000);
               moteurState = LOW; 
               digitalWrite(moteurPin, moteurState);// On l'allume
        }
        digitalWrite(moteurPin, moteurState);  // Enfin on change l'état de la Moteur
      }
//  }
  Serial.print(rcv);
  Serial.println();
  delay(60000);
}
