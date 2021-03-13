#include <Arduino.h>
#include <FirebaseESP8266.h>
#include <ESP8266WiFi.h>
#include <Ultrasonic.h>

/* 1. Define the WiFi credentials */
#define WIFI_SSID "MRH"
//"Im_a_Fridge"
#define WIFI_PASSWORD "Kellulu1234"
//"thereisnopassword"

/* 2. Define the Firebase project host name (required) */
#define FIREBASE_HOST "b2smartdrv-esp-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "77fY8j8RaleP8oZqMenefL4LkqDSsivh4PCfjfJH"

/* 3. Define the Firebase Data object */
FirebaseData fbdo;

/* 4, Define the FirebaseAuth data for authentication data */
FirebaseAuth auth;

/* Define the FirebaseConfig data for config data */
FirebaseConfig config;

/* Define FirebaseESP8266 data object */

FirebaseData firebaseData;
FirebaseData Front;
FirebaseJson json;

String path = "/B2SmartDRV/";
int count = 0;

/* Esp8266 pins to GPIO */
/* The follow Pins are declared under Arduino.h
int D0 = 16;    
int D1 = 5;
int D2 = 4;
int D3 = 0;
int D4 = 2;     // Built-in LED PIN
int D5 = 14;
int D6 = 12;
int D7 = 13;
int D8 = 15;
*/

int SD2 = 9;
int SD3 = 10;

/* Define all ultrasonic sensors (trig, echo)*/
Ultrasonic USFront(D0, D1);
Ultrasonic USLeft(D2, D3);
Ultrasonic USRight(D5, D6);
Ultrasonic USBack(D7, D8);

/* Define all PIR sensors */
int PIRFront = D4;
int PIRLeft = PIN_A0;
int PIRRight = SD2;
int PIRBack = SD3;

/* Define arrays to store data */
float Distance[4];
bool PIR[4];

String US_Final;
String PIR_Final;



void setup(){
    Serial.begin(115200);

    WiFi.hostname("B2SmartDRV");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED){
        Serial.print(".");
        delay(300);
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();

    config.host = FIREBASE_HOST;
    config.signer.tokens.legacy_token = FIREBASE_AUTH;

    Firebase.reconnectWiFi(true);

    /* Initialize the library with the Firebase authen and config */
    Firebase.begin(&config, &auth);

    /* Ultrasonic Sensor (trig) pin mode */
    pinMode(D0, OUTPUT);
    pinMode(D2, OUTPUT);
    pinMode(D5, OUTPUT);
    pinMode(D7, OUTPUT);

    /* Ultrasonic Sensor (echo) pin mode */
    pinMode(D1, INPUT);
    pinMode(D3, INPUT);
    pinMode(D6, INPUT);
    pinMode(D8, INPUT);

    /* PIR pin mode */
    pinMode(PIRFront, INPUT);
    pinMode(PIRLeft, INPUT);
    pinMode(PIRRight, INPUT);
    pinMode(PIRBack, INPUT);

}

void errorMessage(){
  Serial.println("FAILED");
  Serial.println("REASON: " + firebaseData.errorReason());
  Serial.println("------------------------------------\n");
}

void sensorUpdate(){
  US_Final = "";
  PIR_Final = "";

  Distance[0] = USFront.read();
  Distance[1] = USLeft.read();
  Distance[2] = USRight.read();
  Distance[3] = USBack.read();

  Serial.print("Ultrasonic Reading: ");
  Serial.println(Distance[0]);

  //US_Final = Distance;

  PIR[0] = digitalRead(PIRFront);
  PIR[1] = digitalRead(PIRLeft);
  PIR[2] = digitalRead(PIRRight);
  PIR[3] = digitalRead(PIRBack);

  for (int i = 0; i < 4; i++){
    US_Final += Distance[i];
    US_Final += "*";
    PIR_Final += PIR[i];
    PIR_Final += "*";
  }

  //PIR_Final = PIR[0] + "," + PIR[1] + ',' + PIR[2] + ',' + PIR[3];

  if (Firebase.setString(firebaseData, "/B2SmartDRV/US_Final", US_Final)){
    Serial.println("USFront SUCCESS");
    }else{errorMessage();};

  if (Firebase.setString(firebaseData, "/B2SmartDRV/PIR_Final", PIR_Final)){
    Serial.println("PIRFront SUCCESS");
    }else{errorMessage();};

  /*
  if (Firebase.setInt(firebaseData, "/B2SmartDRV/Front", count++)){
    Serial.println("UPLOAD SUCCESS");
    Serial.println("------------------------------------\n");
  }else{
    Serial.println("FAILED");
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println("------------------------------------\n");
  }
  */
}
void loop(){
  sensorUpdate();

  /*
  if(Firebase.getString(Front, "/B2SmartDRV/US_Final")){
    //Success
    Serial.print("Get int data success, US_Final = ");
    Serial.println(Front.stringData());
    Serial.println("------------------------------------\n");
  }else{
    Serial.print("Error in getInt (USFront), ");
    Serial.println(Front.errorReason());
    Serial.println("------------------------------------\n");
  }
  */

  //delay(100);
}