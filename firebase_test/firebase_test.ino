#include <WiFi.h>
#include <FirebaseESP32.h>

// Firebase Helpers
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

// WiFi Credentials
#define WIFI_SSID "SUNDER"
#define WIFI_PASSWORD "getBackToWork"

// Firebase project API
#define API_KEY "AIzaSyA3ASRUnPKruHAMPFCu7o0vs6G8DalnrAI"
#define DATABASE_URL "https://esp32-test-f01c5-default-rtdb.asia-southeast1.firebasedatabase.app/"

// OBJECTS FOR FIREBASE
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
String databasePath = "";
String fuid = "";
bool isAuthenticated = false;

void WiFi_init(){
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected to IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
}

void firebase_init(){
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  Firebase.reconnectWiFi(true);
  Serial.println("Sign up new user..");

  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("Success");
    isAuthenticated = true;
    databasePath = "/a";
    fuid = auth.token.uid.c_str();
  }else{
    Serial.printf("Failed, %s \n", config.signer.signupError.message.c_str());
    isAuthenticated = false;
  }

  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config, &auth);
  
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi_init();
  firebase_init();

  String node = databasePath + "/value";
  if(Firebase.set(fbdo, node.c_str(), "Hello")){
    Serial.println("passed");
  }else{
    Serial.println("failed");
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
