/*
   Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleScan.cpp
   Ported to Arduino ESP32 by pcbreflux
   Modified to work with NimBLE
   BLE advertisement format from https://bthome.io/
*/

#include "NimBLEDevice.h"
#include "NimBLEBeacon.h"
#include "esp_sleep.h"

#define GPIO_DEEP_SLEEP_DURATION     1  // sleep x seconds and then wake up
RTC_DATA_ATTR static uint32_t bootcount; // remember number of boots in RTC Memory

#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/
BLEAdvertising *pAdvertising;

#define BEACON_UUID           "4dd96d12-2f2c-11ed-a261-0242ac120002" // UUID 1 128-Bit (may use linux tool uuidgen or random numbers via https://www.uuidgenerator.net/)

void setBeacon() {

  BLEBeacon oBeacon = BLEBeacon();
  oBeacon.setManufacturerId(0x4C00); // fake Apple 0x004C LSB (ENDIAN_CHANGE_U16!)
  oBeacon.setProximityUUID(BLEUUID(BEACON_UUID));
  BLEAdvertisementData oAdvertisementData = BLEAdvertisementData();
  BLEAdvertisementData oScanResponseData = BLEAdvertisementData();
  
  oAdvertisementData.setFlags(0x06); // this is 00000110. Bit 1 and bit 2 are 1, meaning: Bit 1: “LE General Discoverable Mode” Bit 2: “BR/EDR Not Supported”
  

  std::string strServiceData = "";
  std::string strServiceData2 = "";
  strServiceData += (char)11;     // Length
  strServiceData += (char)0x09;   // Type = Complete local name
  String device_name = "myBTHome_1"; //needs to only be 10 characters long
  char str_buf[10]; // Actually 11 in size as its 0 indexed
  device_name.toCharArray(str_buf,11); // Length is 11 here as we need to capture the null terminator
  strServiceData += str_buf;


  strServiceData2 += (char)11;     // Length
  strServiceData2 += 0x16;      // Type = Service Data - 16-bit UUID

  strServiceData2 += 0x1C;    // Use 0x1C18 for non-encrypted messages
  strServiceData2 += 0x18;
  
  //This is an example but you could change these packets to your needs. See https://bthome.io/
  // Temperature packet
  strServiceData2 += 0x23;  
  strServiceData2 += 0x02;

  // Temperature Value (little endian)
  strServiceData2 += 0xC4;  
  strServiceData2 += 0x09;

  // Count packet
  strServiceData2 += 0x03;  
  strServiceData2 += 0x09;

  // Count value
  strServiceData2 += (char)(bootcount & 0xFF);  // bootcount lower byte
  strServiceData2 += (char)((bootcount & 0xFF00)>>8); // bootcount upper byte
  

  oAdvertisementData.addData(strServiceData);
  oAdvertisementData.addData(strServiceData2);
  pAdvertising->setAdvertisementData(oAdvertisementData);
  pAdvertising->setScanResponseData(oScanResponseData);
  /**  pAdvertising->setAdvertisementType(ADV_TYPE_NONCONN_IND);
  *    Advertising mode. Can be one of following constants:
  *  - BLE_GAP_CONN_MODE_NON (non-connectable; 3.C.9.3.2).
  *  - BLE_GAP_CONN_MODE_DIR (directed-connectable; 3.C.9.3.3).
  *  - BLE_GAP_CONN_MODE_UND (undirected-connectable; 3.C.9.3.4).
  */
  pAdvertising->setAdvertisementType(BLE_GAP_CONN_MODE_NON);

}

void setup() {

    
  Serial.begin(115200);
  bootcount++;;
  Serial.printf("start ESP32 %d\n",bootcount);
  
  // Create the BLE Device
  BLEDevice::init("");

  pAdvertising = BLEDevice::getAdvertising();
  
  setBeacon();
   // Start advertising
  pAdvertising->start();
  Serial.println("Advertizing started...");
  delay(100);
  pAdvertising->stop();
  Serial.printf("enter deep sleep\n");
  esp_deep_sleep(1000000LL * GPIO_DEEP_SLEEP_DURATION);
  Serial.printf("in deep sleep\n");
}

void loop() {
}