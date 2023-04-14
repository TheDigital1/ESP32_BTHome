*** Thanks @Chreece New updated project that supports BTHome V2 here https://github.com/Chreece/BTHomeV2-ESP32-example ***

# ESP32_BTHome
This project aims to show how you can use a ESP32 and Arduino libraries to send BLE advertisement packets in the BTHome format.
This code should run on any ESP32. I have tested on ESP32-Wroom, and ESP32-C3

One main KPI for this code is to be as lightweight as possible allowing the ESP to boot quickly and send BLE advertisement packets and get back to sleep fast all to save power for battery operated projects.

This code depends on the NimBLE-Arduino library. 
https://github.com/h2zero/NimBLE-Arduino
