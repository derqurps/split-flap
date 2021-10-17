
void setupTime() {
  //Needed for time functions
  LocalTimezone.setPosix(localTimezoneString);
  setInterval(60);
  #ifdef SECRET_NTP_SERVER
  setServer(SECRET_NTP_SERVER);
  #endif
  #ifdef serial
    Serial.println("timezone initialized");
  #endif
}

void showDate() {
  showNewData(LocalTimezone.dateTime(dateFormat));
}

void showClock() {
  showNewData(LocalTimezone.dateTime(clockFormat));
}
