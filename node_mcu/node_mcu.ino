#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>

#define FIREBASE_HOST "https://monitoringair-933ce-default-rtdb.firebaseio.com/"                     //Your Firebase Project URL goes here without "http:" , "\" and "/"
#define FIREBASE_AUTH "Ak5bhlAXudSWV9oYneGghflYk1slHWX6onyoni1G" //Your Firebase Database Secret goes here

#define WIFI_SSID "Seamless@wifi.Id"                                               //WiFi SSID to which you want NodeMCU to connect
#define WIFI_PASSWORD "bbbbbbbb"

FirebaseData firebaseData;

float nilai_turbid, nilai_ph, nilai_tds;

String answer, data[4];
void setup() {
  Serial.begin(115200);
  Serial.println("Serial communication started\n\n");

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                     //try to connect with wifi
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);



  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }


  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());                                            //print local IP address
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);   // connect to firebase

  Firebase.reconnectWiFi(true);
  delay(1000);
}
void loop() {
  while (Serial.available() == 0);
  int q = 0;
  answer = Serial.readStringUntil('\n');
  //Contoh Answer = #Yusup#Firmansayah#Kece
  Serial.print("Diterima : ");
  Serial.println(answer);
  Serial.print("panjang : ");
  Serial.println(answer.length());
  for (int i = 0; i < answer.length(); i++) {
    if (answer[i] == '#') {
      q++;
      data[q] = "";
    }
    else {
      data[q] += answer[i];
    }
  }
  Serial.println("=======================================");
  Serial.println("NTU : " + data[1]);
  Serial.println("PH : " + data[2]);
  Serial.println("TDS : " + data[3]);
  Serial.println("=======================================");

  int nilai_ntu = data[1].toFloat();
  int nilai_tds = data[3].toFloat();
  FirebaseJson json;
  json.set("turbidity", nilai_ntu);
  json.set("ph", data[2].toFloat());
  json.set("tds", nilai_tds);
  if (Firebase.setJSON(firebaseData, "/", json))
  {
    Serial.println("PASSED");
    Serial.println();
  }
  else
  {
    Serial.println("FAILED");
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  }

//  if (Firebase.setFloat(firebaseData, "/turbidity", data[1].toFloat() && Firebase.setFloat(firebaseData, "/ph", data[2].toFloat()))) {   // On successful Write operation, function returns 1
//    Serial.println("Value Uploaded Successfully");
//    Serial.print("Val = ");
//    Serial.println(data[1] + "dan" + data[2]);
//    Serial.println("\n");
//    delay(10);
//  }
//
//  else {
//    Serial.println(firebaseData.errorReason());
//  }
}