// sensor kekeruhan air

#include <Wire.h>
#include <EEPROM.h>
#include "GravityTDS.h"

String kirim;
float ntu, Ntu, nilaiPh, nilaiTds; 

//====================== TDS =========================================
#define TdsSensorPin A2
GravityTDS gravityTds;

float temperature = 25, tdsValue = 0;
//===================================================================
//============================ TURBID ===============================
int pinSensor = A0; //pin Turbid

//Variabel data
float tegangan; //data untuk tegangan
//===================================================================

//=============================== PH ====================================
const int phSensorPin  = A1; //pin PH
float Po = 0;
//=======================================================================

void setup() {
  Serial.begin(115200);
  pinMode (phSensorPin, INPUT);
  //Serial.println("MULAI!!!");

  //=================================== TDS ==================================================
  gravityTds.setPin(TdsSensorPin);
  gravityTds.setAref(5.0);  //reference voltage on ADC, default 5.0V on Arduino UNO
  gravityTds.setAdcRange(1024);  //1024 for 10bit ADC;4096 for 12bit ADC
  gravityTds.begin();  //initialization
  //============================================================================================
}

void loop()
{
  Ntu = getNtu();
  nilaiPh = getPh();
  nilaiTds = getTds();
  kirim = "#" + String(Ntu) + "#" + String(nilaiPh) + "#" + String(nilaiTds);
  Serial.println(kirim);
  delay(1000);
}

int getTds() {
  //temperature = readTemperature();  //add your temperature sensor and read it
  gravityTds.setTemperature(temperature);  // set the temperature and execute temperature compensation
  gravityTds.update();  //sample and calculate
  tdsValue = gravityTds.getTdsValue();  // then get the value
//  Serial.print(tdsValue, 0);
//  Serial.println("ppm");

  return tdsValue;
}
float getNtu()
{
  tegangan = 00;
  for (int i = 00; i < 800; i++)
  {
    tegangan += ((float)analogRead(pinSensor) / 1023) * 5;
  }
  tegangan = tegangan / 800;
  tegangan = round_to_dp(tegangan, 1);

  if (tegangan < 2.5) {
    ntu = 3000;
  }
  else {
    ntu = -1120.4 * square(tegangan) + 5742.3 * tegangan - 4353.8;
  }
  //    Serial.print("Tegangan = ");
  //    Serial.print(tegangan);
  //    Serial.println(" V");
  //    Serial.println("");
  //    Serial.print("Kekeruhan = ");
  //    Serial.print(ntu);
  //    Serial.println(" NTU");
  return ntu;
}

float round_to_dp( float nilaibaca, int desimal)
{
  float multiplier = powf( 10.0f, desimal );
  nilaibaca = roundf( nilaibaca * multiplier ) / multiplier;
  return nilaibaca;
}

float getPh() {
  int nilaiPengukuranPh = analogRead(phSensorPin);
  //  Serial.print("Nilai ADC Ph: ");
  //  Serial.println(nilaiPengukuranPh);
  double TeganganPh = 5 / 1024.0 * nilaiPengukuranPh;
  //  Serial.print("TeganganPh: ");
  //  Serial.println(TeganganPh, 3);
  //Po = 7.00 + ((teganganPh7 - TeganganPh) / PhStep);
  Po = 7.00 + ((2.6 - TeganganPh) / 0.17);
  //  Serial.print("Nilai PH cairan: ");
  //  Serial.println(Po, 3);
  return Po;
}