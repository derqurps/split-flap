

static void handle_update_progress_cb(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
  uint32_t free_space = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
  if (!index){
    Serial.println("Update");
    Update.runAsync(true);
    if (!Update.begin(free_space)) {
      Update.printError(Serial);
    }
  }

  if (Update.write(data, len) != len) {
    Update.printError(Serial);
  }

  if (final) {
    if (!Update.end(true)){
      Update.printError(Serial);
    } else {
      restartNow = true;//Set flag so main loop can issue restart call
      Serial.println("Update complete");
    }
  }
}

void indexrequest(AsyncWebServerRequest *request){
  request->send_P(200, "text/html", upload_page);
}

void webSetup() {
  
  // Web Server Root URL
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(LittleFS, "/index.html", "text/html");
  });

  server.serveStatic("/", LittleFS, "/");

  server.on("/values", HTTP_GET, [](AsyncWebServerRequest * request) {
    String json = getCurrentInputValues();
    request->send(200, "application/json", json);
    json = String();
  });

AsyncCallbackJsonWebHandler* jsonHandler = new AsyncCallbackJsonWebHandler("/json", [](AsyncWebServerRequest *request, JsonVariant &json) {

    JsonObject object = json.as<JsonObject>();
    if (object.containsKey(PARAM_ALIGNMENT)) {
      alignment = object[PARAM_ALIGNMENT].as<String>();
      writeFile(LittleFS, alignmentPath, alignment.c_str());
    }
    if (object.containsKey(PARAM_SPEED)) {
      flapSpeed = object[PARAM_SPEED].as<String>();
      writeFile(LittleFS, flapspeedPath, flapSpeed.c_str());
    }
    if (object.containsKey(PARAM_DEVICEMODE)) {
      devicemode = object[PARAM_DEVICEMODE].as<String>();
      writeFile(LittleFS, devicemodePath, devicemode.c_str());
    }
    if (object.containsKey(PARAM_TEXT)) {
      flaptext = object[PARAM_TEXT].as<String>();
      writeFile(LittleFS, flaptextPath, flaptext.c_str());
    }
    
    String retjson = getCurrentInputValues();
    request->send(200, "application/json", retjson);
    retjson = String();
    
    /*AsyncResponseStream *response = request->beginResponseStream("application/json");
    DynamicJsonDocument doc2(1024);
    //JsonObject &root = jsonBuffer.createObject();
    doc2["heap"] = ESP.getFreeHeap();
    doc2["ssid"] = WiFi.SSID();
    serializeJson(object, *response);
    request->send(response);*/
  });

  server.addHandler(jsonHandler);

  server.on("/", HTTP_POST, [](AsyncWebServerRequest * request) {
    int params = request->params();
    for (int i = 0; i < params; i++) {
      AsyncWebParameter* p = request->getParam(i);
      if (p->isPost()) {

        // HTTP POST alignment value
        if (p->name() == PARAM_ALIGNMENT) {
          alignment = p->value().c_str();
          #ifdef serial
            Serial.print("Alignment set to: ");
            Serial.println(alignment);
          #endif
          writeFile(LittleFS, alignmentPath, alignment.c_str());
        }

        // HTTP POST speed slider value
        if (p->name() == PARAM_SPEED) {
          flapSpeed = p->value().c_str();
          #ifdef serial
            Serial.print("Speed set to: ");
            Serial.println(flapSpeed);
          #endif
          writeFile(LittleFS, flapspeedPath, flapSpeed.c_str());
        }

        // HTTP POST mode value
        if (p->name() == PARAM_DEVICEMODE) {
          devicemode = p->value().c_str();
          #ifdef serial
            Serial.print("Mode set to: ");
            Serial.println(devicemode);
          #endif
          writeFile(LittleFS, devicemodePath, devicemode.c_str());
        }

        // HTTP POST input1 value
        if (p->name() == PARAM_TEXT) {
          flaptext = p->value();
          #ifdef serial
            Serial.print("flaptext set to: ");
            Serial.println(flaptext);
          #endif
          writeFile(LittleFS, flaptextPath, flaptext.c_str());
        }
      }
    }
    request->send(200);
  });
  
  server.on("/update", HTTP_GET, indexrequest);
  
  server.on("/update", HTTP_POST, [](AsyncWebServerRequest *request) {
    request->send(200);
  },handle_update_progress_cb);
  
  server.begin();
  #ifdef serial
    Serial.println("webserver setup finished");
  #endif
}
