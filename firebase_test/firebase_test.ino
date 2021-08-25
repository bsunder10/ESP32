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

// Json files
FirebaseJson test_json;

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

  test_json.add("test1", "test1");
  test_json.add("test2", "test2");
  test_json.add("test3", "test3");
  
  String test_node = databasePath + "/test";
  if(Firebase.set(fbdo, test_node.c_str(), test_json)){
    Serial.println("passed");
  }else{
    Serial.println("failed");
  }


  FirebaseData fbdo;
  if (Firebase.getString(fbdo, "/test/test1")){
    String json = fbdo.to<const char *>();
    Serial.println(json);
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
