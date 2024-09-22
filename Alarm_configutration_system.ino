#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#define WIFI_SSID "123456789"
#define WIFI_PASSWORD "123456789"
#define API_KEY "AIzaSyC0gPSHesz3RxIsbFM48OkKK_zCBhfbtmc"
#define DATABASE_URL "https://test-26075-default-rtdb.firebaseio.com/"

// Define Buzzer pin
#define BUZZER_PIN D2  

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
bool signupOK = false;
String intValue;

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("ok");
    signupOK = true;
  } else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }
  
  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 1000 || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();
  
    if (Firebase.RTDB.getString(&fbdo, "/mainbucket1/status")) {
      intValue = fbdo.stringData();
      String mySubString = intValue.substring(2, 3);
      Serial.println(intValue);
      Serial.println(mySubString);
      
      // Control first LED based on value
      if (mySubString == "0") {
        digitalWrite(BUZZER_PIN, LOW);
        Serial.println("Buzzer OFF");
        delay(100);  
      } else if (mySubString == "1") {
        digitalWrite(BUZZER_PIN, HIGH);
        Serial.println("Buzzer ON");
        delay(100);  
      }
       else {
      Serial.println(fbdo.errorReason());
    }
    delay(100);
    }
  }
}
