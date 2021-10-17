void mqttSetup() {
 client.setServer(mqtt_server, mqtt_port);
 client.setCallback(callback);
 #ifdef serial
    Serial.println("mqtt setup finished");
  #endif
}
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    /*
     YOU MIGHT NEED TO CHANGE THIS LINE, IF YOU'RE HAVING PROBLEMS WITH MQTT MULTIPLE CONNECTIONS
     To change the ESP device ID, you will have to give a new name to the ESP8266.
     Here's how it looks:
       if (client.connect("ESP8266Client")) {
     You can do it like this:
       if (client.connect("ESP1_Office")) {
     Then, for the other ESP:
       if (client.connect("ESP2_Garage")) {
      That should solve your MQTT multiple connections problem
    */
    if (client.connect(mdns_hostname, MQTT_USER, MQTT_PASSWORD)) {
      Serial.println("connected");  
      // Subscribe or resubscribe to a topic
      // You can subscribe to more topics (to control more LEDs in this example)
      client.subscribe(mqtt_listen);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void callback(String topic, byte* message, unsigned int length) {
  char flap_msg[UNITSAMOUNT+1];
  int spaces = 0;
  
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  DynamicJsonDocument doc(1024);
  deserializeJson(doc, message);

  if(doc["devicemode"]) {
    String dm = doc["devicemode"];
    if( dm == "text" ||
      dm == "date" ||
      dm == "clock")
    {
      devicemode = dm;
    }
  }
  if(doc["alignment"]) {
    String align = doc["alignment"];
    if (align == "left" ||
      align == "center" ||
      align == "right") 
    {
      alignment = align;
    }
  }
  if(doc["flapspeed"] && doc["flapspeed"] >= MINSPEED && doc["flapspeed"] <= MAXSPEED) {
    int fs = doc["flapspeed"];
    flapSpeed = fs;
  }
  if(doc["flaptext"]) {
    // TODO
    String ft = doc["flaptext"];
    flaptext = ft;
    
  }
  updateFlapData();
}

void mqttLoopOps() {
  if (!client.connected()) {
    reconnect();
  }
  if (!client.loop()) {
    client.connect(mdns_hostname);
  }
}

void build_registration() {
  const size_t CAPACITY = JSON_OBJECT_SIZE(4);
  StaticJsonDocument<CAPACITY> doc;
  doc["name"] = mdns_hostname;
  doc["ip_address"] = ip_char;
  doc["state"] = "connected";
  doc["uptime"] = millis();
  serializeJson(doc, reg);
  #ifdef serial
    Serial.println("build_registration finished");
  #endif
}

void checkIn() {
  unsigned long uptime = millis();
  char registration[1024];
  DynamicJsonDocument doc(1024);
  deserializeJson(doc, reg);
  doc["uptime"] = uptime;
  doc["last_written"] = writtenLast;
  serializeJson(doc, registration);
  client.publish(mqtt_registration, registration);
  
}
