#include <OneWire.h>
#include <DallasTemperature.h>
 
// Data wire is plugged into pin 2 on the Arduino
#define ONE_WIRE_BUS 2
 
// Setup a oneWire instance to communicate with any OneWire devices
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
 
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

int sensorPin = A0;
int sensorValue = 0;

int moteurPin = 13;                     // Sortie où sera branchée la moteur
int moteurState = LOW;                  // Etat de la MOTEUR (LOW par défaut)


void setup(void)
{
  // start serial port
  Serial.begin(9600);

  // Start up the library
  sensors.begin();

  // MOTEUR
  pinMode(moteurPin, OUTPUT);           // Et la sortie de la moteur
}


void loop(void)
{
  ////////////////////// TEMPERATURE
  sensors.requestTemperatures(); // Send the command to get temperatures

  Serial.print(sensors.getTempCByIndex(0)); 

  Serial.print(";");
 
  ////////////////////// // HUMIDITE
 
  sensorValue = analogRead(sensorPin);
  //Serial.print("Moisture = " );
  Serial.println(sensorValue);

  ////////////// Control MOTEUR
  int received;                     // Variable servant à récupérer les données reçues
  if (Serial.available()>0) {       // Si des données sont disponibles
  received = Serial.read();       // On les récupère
      if(received == 'a') {           // Si "a" est reçu
        if (moteurState==HIGH) {          // Et que la Moteur est allumée
              moteurState = LOW;              // On l'éteint
        }
        else {                          //Sinon
              moteurState = HIGH;             // On l'allume
        }
        digitalWrite(moteurPin, moteurState);  // Enfin on change l'état de la Moteur
      }
  }
  delay(1000); 
}
