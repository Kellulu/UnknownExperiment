/* Esp8266 pins to GPIO */
/* The follow Pins are declared under Arduino.h
int D0 = 16;    // On chip LED
int D1 = 5;     // Left Red Button
int D2 = 4;     // Left Yellow Button
int D3 = 0;     // FLASH PIN - DO NOT USE!!
int D4 = 2;     // On Board LED
int D5 = 14;    // Middle Big Button
int D6 = 12;    // Right Yellow Button
int D7 = 13;    // Right Red Button
int D8 = 15;
*/

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>

const String Device_Col = "RED";
const int debounce = 150;
const bool print_enable = 1;

/* Define types of functions */
const String GameMode[3] = {
    "PAPAPA",
    "QuickAns",
    "PatternDance1",
};

/* 1. Define the WiFi credentials */
const int WIFI_SELECT = 0;
const String WIFI_SSID[3] = {
    "spcs-teacher",
    "MRH",
    "Im_a_Fridge",
};
const String WIFI_PASSWORD[3] = {
    "v3zg3s2w",
    "Kellulu1234",
    "thereisnopassword",
};

/* 2. Define the Firebase project host name (required) */
#define FIREBASE_HOST "https://pushthebutton2021-default-rtdb.firebaseio.com/"
//#define FIREBASE_AUTH "Eh9G7DtDKocR1fvN0kVNA0BHtlvZovmu4B6hADRp"
#define API_KEY "AIzaSyCqDJFzrC_Bmrl8n8DXex0emiP5wzJx3jw"

/* 3. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "pushthebutton@ptb.io"
#define USER_PASSWORD "PushTheButton"

/* 4. Define the Firebase Data object */
FirebaseData fbdo;

/* 5, Define the FirebaseAuth data for authentication data */
FirebaseAuth auth;

/* Define the FirebaseConfig data for config data */
FirebaseConfig config;

/* Device Name */
const String DEVICENAME = "PushTheButton-io";

FirebaseData firebaseData;
FirebaseData Front;
FirebaseJson json;

/* Define data storing path */
String path = "/" + Device_Col + "/";
String Command_Path = "/Order/";

int i = 0;

/* Declare button pins in array */
const int btnArray[5] = {5, 4, 14, 12, 13};

/* Declare press counter */
int btnPressCount_old[5];
int btnPressCount_new[5];

/* Not in use */
bool bState_old[5];
bool bState_new[5];

/* Interrup ISR function */
void ICACHE_RAM_ATTR btnCounter() {
    unsigned long startTime = millis();
    for (int btn = 0; btn < 5; btn++) {
        if (digitalRead(btnArray[btn])) {
            btnPressCount_new[btn]++;
            while ((millis() - startTime) < debounce) {
            }
        }
    }
}

void setup() {
    Serial.begin(115200);

    /* Pin mode */
    pinMode(D0, OUTPUT); //  On chip LED
    pinMode(D4, OUTPUT); //  On Board LED
    digitalWrite(D0, HIGH);
    digitalWrite(D4, HIGH);

    /* Attach all button pins as interrup */
    attachInterrupt(digitalPinToInterrupt(btnArray[0]), btnCounter, RISING);
    attachInterrupt(digitalPinToInterrupt(btnArray[1]), btnCounter, RISING);
    attachInterrupt(digitalPinToInterrupt(btnArray[2]), btnCounter, RISING);
    attachInterrupt(digitalPinToInterrupt(btnArray[3]), btnCounter, RISING);
    attachInterrupt(digitalPinToInterrupt(btnArray[4]), btnCounter, RISING);

    /* Initializing all press counter */
    for (int b = 0; b < 6; b++) {
        btnPressCount_old[b] = 0;
        btnPressCount_new[b] = 0;
        digitalWrite(btnArray[b], LOW);
    }

    Serial.println("\n");
    Serial.print("Device Name: ");
    Serial.println(DEVICENAME);
    Serial.print("Connecting to SSID: ");
    Serial.println(WIFI_SSID[WIFI_SELECT]);
    Serial.println();

    WiFi.hostname(DEVICENAME);

    WiFi.begin(WIFI_SSID[WIFI_SELECT], WIFI_PASSWORD[WIFI_SELECT]);

    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(300);
    }

    digitalWrite(D4, LOW);

    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();

    config.host = FIREBASE_HOST;
    config.api_key = API_KEY;

    /* Enable the below line to use Secret key */
    //config.signer.tokens.legacy_token = FIREBASE_AUTH;

    /* Assign the user sign in credentials */
    auth.user.email = USER_EMAIL;
    auth.user.password = USER_PASSWORD;

    /* Initialize the library with the Firebase authen and config */
    Firebase.begin(&config, &auth);

    digitalWrite(D0, LOW);

    /* Enable auto reconnect the WiFi when connection lost */
    Firebase.reconnectWiFi(true);

    Serial.println("READY.");
}

void errorMessage() {
    Serial.println("FAILED");
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println("------------------------------------\n");
}

void PressCount_InTime() {
    for (int b = 0; b < 5; b++) {
        if (btnPressCount_old[b] != btnPressCount_new[b]) {
            btnPressCount_old[b] = btnPressCount_new[b];

            if (print_enable) {
                Serial.print("Button ");
                Serial.print(b + 1);
                Serial.print(" is pressed: ");
                Serial.print(btnPressCount_new[b]);
                Serial.println(" Times.");
            }
        }
    }
}

void PAPAPA() {
}

void QuickAns() {
}

void PatternDance1() {
}

void loop() {
  //Serial.println("In Loop");
  if (Firebase.RTDB.get(&fbdo, "/Order/")) {
      //Success
      Serial.print("The Order is ");
      Serial.println(fbdo.stringData());
  }
    /*
  Serial.print("Button is pressed: ");
  Serial.println(btnPressCount_new[0]);
  Serial.println(" Times.");
  */

    /*
  if (Firebase.RTDB.set(&fbdo, path.c_str(), i)){
    //Serial.println("Upload SUCCESS");
    i++;
    }else{errorMessage();};

  if (Firebase.RTDB.set(&fbdo, "/PressCount/", btnPressCount[2])){
    //Serial.println("Upload SUCCESS");
    i++;
    }else{errorMessage();};

   for (int btn = 0; btn < 5; btn++){
    if (digitalRead(btnArray[btn])){
      Serial.print("Btn");
      Serial.print(btn);
      Serial.print(": ");
      Serial.println(digitalRead(btnArray[btn]));
    }
   }
  */

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