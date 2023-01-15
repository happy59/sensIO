#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#include "index2.h" //Our HTML webpage contents with javascripts

#define pinCapteurTasse D7 //pin module Sharp
#define moteur D6 //relay du moteur
#define sortiePTasse D3 //bouton petite tasse
#define sortieGTasse D2 //bouton grosse tasse
#define power D5 //bouton ONOFF

#define ONE_WIRE_BUS D1
#define TEMPERATURE_PRECISION 10
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);
// Tableaux contenant l'adresse de chaque sonde OneWire | arrays to hold device addresses
DeviceAddress insideThermometer = { 0x28,  0x6D,  0xFB,  0x96,  0xF0,  0x1,  0x3C,  0xFD };

//SSID and Password of your WiFi router
const char* ssid = "[ENTER YOUR SSID]"; const char* password = "[ENTER YOUR PASSWORD]";
bool enTouillage = false;
bool etat = true;

ESP8266WebServer server(80); //Server on port 80

//===============================================================
// This routine is executed when you open its IP in browser
//===============================================================
void handleRoot() {
  String s = MAIN_page; //Read HTML contents
  server.send(200, "text/html", s); //Send web page
}

void handleTasse() {
  String tasse = "0";
  if (digitalRead(pinCapteurTasse) == LOW) {
    tasse = "0";

  }
  else if (digitalRead(pinCapteurTasse) == HIGH) {
    tasse = "1";

  }
  delay(50);
  server.send(200, "text/plane", tasse); //Send tasse value only to client ajax request
}

void handleTemp() {
  sensors.requestTemperatures();
  float temp = sensors.getTempC(insideThermometer);
  String tempString = (String)temp + "°C";
  Serial.println(tempString);
  //Serial.println(tempString);
  delay(50);
  server.send(200, "text/plane", tempString); //Send ADC value only to client ajax request
}

void handleEtat(){
  String retour = "N/A";
  if(etat==true){retour = "allume";}
  else{retour = "eteint";}
  delay(50);

  server.send(200, "text/plane", retour);
}

void handleTouillage(){
  String aEnvoyer = "N/A";
  if(enTouillage){aEnvoyer = "allume";}  
  else{aEnvoyer = "eteint";}

  delay(50);
  server.send(200, "text/plane", aEnvoyer);
}

void allumeMachine() 
{
 String boutonAppuye = server.arg("state");
 if(boutonAppuye == "0")//Bouton Power
 {
  etat = !etat;
  digitalWrite(power, HIGH); //LED ON
  delay(500);
  digitalWrite(power, LOW);
 }
 if(boutonAppuye == "1")//Bouton PT
 {
  digitalWrite(sortiePTasse, HIGH); //LED ON
  delay(500);
  digitalWrite(sortiePTasse, LOW);
 }
 if(boutonAppuye == "2")//Bouton GT
 {
  digitalWrite(sortieGTasse, HIGH); //LED ON
  delay(500);
  digitalWrite(sortieGTasse, LOW);
 }
 if(boutonAppuye == "3")//Bouton stir
 {
  if(enTouillage == false){digitalWrite(moteur, HIGH); enTouillage = !enTouillage;}
  else {digitalWrite(moteur, LOW); enTouillage = !enTouillage;}
 }
 if(boutonAppuye == "4"){
  faitLCaf();
 }

 server.send(200);
}

void faitLCaf(){
  if(!etat){
    etat = !etat;
    digitalWrite(power, HIGH); //LED ON
    delay(500);
    digitalWrite(power, LOW);    
  }
  
  if(digitalRead(pinCapteurTasse) == HIGH){
    delay(90000);
    digitalWrite(sortiePTasse, HIGH); //LED ON
    delay(500);
    digitalWrite(sortiePTasse, LOW);
    delay(20000);
  }
  etat=!etat;
  digitalWrite(power, HIGH); //LED ON
  delay(500);
  digitalWrite(power, LOW);
}

//==============================================================
//                  SETUP
//==============================================================
void setup(void) {
  Serial.begin(115200);

  // Start up the library
  sensors.begin();

  // locate devices on the bus
  Serial.print("Locating devices...");
  Serial.print("Found ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" devices.");

  // report parasite power requirements
  Serial.print("Parasite power is: ");
  if (sensors.isParasitePowerMode()) Serial.println("ON");
  else Serial.println("OFF");

  // Vérifie si les capteurs sont connectés | check and report if sensors are conneted
  if (!sensors.getAddress(insideThermometer, 0)) Serial.println("Unable to find address for Device 0");//On parle du thermomètre là

  // set the resolution to 9 bit per device
  sensors.setResolution(insideThermometer, TEMPERATURE_PRECISION);


  // On vérifie que le capteur st correctement configuré | Check that ensor is correctly configured
  Serial.print("Device 0 Resolution: ");
  Serial.print(sensors.getResolution(insideThermometer), DEC);
  Serial.println();

  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");

  //Réglage des pin (GPIO)
  pinMode(pinCapteurTasse, INPUT);
  pinMode(sortiePTasse, OUTPUT);
  pinMode(sortieGTasse, OUTPUT);
  pinMode(power, OUTPUT);
  pinMode(moteur, OUTPUT);

  digitalWrite(power, LOW);
  digitalWrite(sortiePTasse, LOW);
  digitalWrite(sortieGTasse, LOW);
  digitalWrite(moteur, LOW);
  

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP

  server.on("/", handleRoot);      //Which routine to handle at root location. This is display page
  server.on("/readTasse", handleTasse);
  server.on("/readTemp", handleTemp);
  server.on("/allumage", allumeMachine);
  server.on("/readEtat", handleEtat);
  server.on("/readTouillage", handleTouillage);
  

  server.begin();                  //Start server
  Serial.println("HTTP server started");
}



//==============================================================
//                     LOOP
//==============================================================
void loop(void) {
  server.handleClient();          //Handle client requests

}
