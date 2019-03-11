void WIFI_init(){
  Serial.println("WIFI init");
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  delay(250);
  byte tries = 11;
  WiFi.begin(_ssid.c_str(), _password.c_str());
  Serial.print("Connecting to ");
  Serial.print(_ssid);
  while(--tries && WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }
  if(WiFi.status() != WL_CONNECTED){
    Serial.println("\nFailed to connect");
    wst = 0;
    Serial.println("WiFi up AP");
    StartAPMode();
  }
  else {
    wst = 1;
    Serial.println("\nWiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
}

bool StartAPMode(){
  IPAddress apIP(192, 168, 4, 1);
  WiFi.disconnect();
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(_ssidAP.c_str(), _passwordAP.c_str());
  Serial.println("AP created");
  Serial.print("AP name: ");
  Serial.println(_ssidAP);
  Serial.print("AP password: ");
  if(_passwordAP == "") Serial.println("<empty>");
  else Serial.println(_passwordAP);
  Serial.print("AP IP: ");
  Serial.println(WiFi.softAPIP());
  return true;
}

void OTA_init(){
  ArduinoOTA.setPort(8266);
  ArduinoOTA.setHostname(SSDP_Name.c_str());
  ArduinoOTA.begin();
  Serial.println("OTA ready");
}

void Reconnect(){
  Serial.println("Disconnected");
  WiFi.begin(_ssid.c_str(), _password.c_str());
  Serial.print("Reconnecting");
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nConnection restored/n");
}

void SSDP_init(void) {
  // SSDP дескриптор
  Serial.print("SSDP init....");
  HTTP.on("/description.xml", HTTP_GET, []() {
    SSDP.schema(HTTP.client());
  });
  //Если версия  2.0.0 закаментируйте следующую строчку
  SSDP.setDeviceType("upnp:rootdevice");
  SSDP.setSchemaURL("description.xml");
  SSDP.setHTTPPort(80);
  SSDP.setName(SSDP_Name);
  SSDP.setSerialNumber("001788102201");
  SSDP.setURL("/");
  SSDP.setModelName("Lumos");
  SSDP.setModelNumber("000000000001");
  SSDP.setModelURL("https://vk.com/citadel_bos");
  SSDP.setManufacturer("CITADEL Engineering");
  SSDP.setManufacturerURL("https://vk.com/citadel_bos");
  SSDP.begin();
  Serial.println("SSDP ready");
  Serial.print("SSDP name: ");
  Serial.println(SSDP_Name);
}
