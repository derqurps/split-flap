
// Initialize WiFi
void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
#ifdef serial
  Serial.print("Connecting to WiFi ..");
#endif
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  waitForSync(10);
#ifdef serial
  Serial.println(WiFi.localIP());
#endif
}
