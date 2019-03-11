void HTTP_init(){
  HTTP.on("/set_color", handle_set_color);
  HTTP.on("/save_colors", handle_save_colors);
  HTTP.on("/light", handle_light);
  HTTP.on("/set_mode", handle_mode);
  HTTP.on("/set_direction", handle_direction);
  HTTP.on("/set_speed", handle_set_speed);
  HTTP.on("/set_border", handle_set_border);
  HTTP.on("/ssdp", handle_set_ssdp);
  HTTP.on("/ssid", handle_set_ssid);
  HTTP.on("/ssidap", handle_Set_Ssidap);
  HTTP.on("/restart", handleRestart);
  HTTP.on("/configs.json", handle_ConfigJSON);
  HTTP.on("/statusjson", handle_status);
  httpUpdater.setup(&HTTP);
  HTTP.begin();
}

void handle_set_color(){
  String colors[4] = {"red", "green", "blue", "white"};
  String message = "\t\tDriving colors\n\n";
  for(int c=0; c<4; c++){
    if(HTTP.argName(0) == colors[c]){
      outVal[c] = HTTP.arg(0).toInt();
      }
    message += "Set ";
    message += colors[c];
    message += " to ";
    message += outVal[c];
    message += "/255";
  }
  for(int i=0; i<4; i++){
    
  }
  HTTP.send(200, "text/plain", message);
}

void handle_save_colors(){
  saveConfig();
  HTTP.send(200, "text/plain", "Saving colors");
}

void handle_light(){
  String message;
  message += "Turn light to ";
  if(HTTP.arg("set") == "on") {light = 1; message += "ON";}
  if(HTTP.arg("set") == "off") {light = 0; message += "OFF";}
  HTTP.send(200, "text/plain", message);
}

void handle_mode(){
  String message;
  String mod[] = {"", "static", "sine"};
  md = HTTP.arg("mode").toInt();
  message += "Set mode to ";
  message += mod[md];
  HTTP.send(200, "text/plain", message);
}

void handle_direction(){
  String message;
  dir = HTTP.arg("dir").toInt();
  message += "Set direction to";
  message += dir;
  HTTP.send(200, "text/plain", message);
}

void handle_set_speed(){
  String message = "\t\tDriving speed\n\n";
  if(HTTP.argName(0) == "speed"){
    sinSpd = HTTP.arg(0).toInt();
    sinSpd = constrain(sinSpd, 1, 500);
  }
  message += "Set delay to ";
  message += sinSpd;
  message += " ms";
  HTTP.send(200, "text/plain", message);
}

void handle_set_border(){
  String message = "\t\tDriving speed\n\n";
  if(HTTP.argName(0) == "min"){
    minVal = HTTP.arg(0).toInt();
  }
  message += "Set minimum to ";
  message += minVal;
  HTTP.send(200, "text/plain", message);
}

void handle_set_ssdp(){
  SSDP_Name = HTTP.arg("ssdp");
  saveConfig();
  HTTP.send(200, "text/plain", "OK");
}

void handle_set_ssid(){
  _ssid = HTTP.arg("ssid");
  _password = HTTP.arg("password");
  saveConfig();
  HTTP.send(200, "text/plain", "OK");
}

void handle_set_ssidap(){
  _ssidAP = HTTP.arg("ssidAP");
  _passwordAP = HTTP.arg("passwordAP");
  saveConfig();
  HTTP.send(200, "text/plain", "OK");
}

void handle_Set_Ssidap() {              
  _ssidAP = HTTP.arg("ssidAP");         
  _passwordAP = HTTP.arg("passwordAP");
  saveConfig();                        
  HTTP.send(200, "text/plain", "OK");
}

void handleRestart(){
  String restart = HTTP.arg("device");
  if(restart == "ok"){
    HTTP.send(200, "text/plain", "Reset OK");
    ESP.restart();
    }
  else HTTP.send(200, "text/plain", "No reset");
}

void handle_ConfigJSON() {
  String root = "{}";  // Формировать строку для отправки в браузер json формат
  //{"SSDP":"SSDP-test","ssid":"home","password":"i12345678","ssidAP":"WiFi","passwordAP":"","ip":"192.168.0.101"}
  // Резервируем память для json обекта буфер может рости по мере необходимти, предпочтительно для ESP8266
  DynamicJsonBuffer jsonBuffer;
  //  вызовите парсер JSON через экземпляр jsonBuffer
  JsonObject& json = jsonBuffer.parseObject(root);
  // Заполняем поля json
  json["SSDP"] = SSDP_Name;
  json["ssidAP"] = _ssidAP;
  json["passwordAP"] = _passwordAP;
  json["ssid"] = _ssid;
  json["password"] = _password;
  json["timezone"] = timezone;
  json["ip"] = WiFi.localIP().toString();
//  json["light"] = light;
//  String colors[4] = {"red", "green", "blue", "white"};
//  for(int i=0; i<4; i++){json[colors[i]] = outVal[i];}
  // Помещаем созданный json в переменную root
  root = "";
  json.printTo(root);
  HTTP.send(200, "text/json", root);
}

void handle_status(){
  HTTP.send(200, "application/json", statusJSON());
}
