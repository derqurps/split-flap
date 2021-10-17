void otaSetup() {
  ArduinoOTA.setHostname(mdns_hostname);
  
  ArduinoOTA.onStart([]() {
    Serial.println("Firmware Update Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nFirmware Update End");
  });
  ArduinoOTA.begin();
  #ifdef serial
    Serial.println("OTA setup finished");
  #endif
}
