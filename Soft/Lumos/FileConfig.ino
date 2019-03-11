// Загрузка данных сохраненных в файл  config.json
bool loadConfig() {
  Serial.print("Load config......");
  // Открываем файл для чтения
  File configFile = SPIFFS.open("/config.json", "r");
  if (!configFile) {
  // если файл не найден  
    Serial.println("Failed to open config file");
  //  Создаем файл запиав в него аные по умолчанию
    saveConfig();
    configFile.close();
    return false;
  }
  // Проверяем размер файла, будем использовать файл размером меньше 1024 байта
  size_t size = configFile.size();
  if (size > 1024) {
    Serial.println("Config file size is too large");
    configFile.close();
    return false;
  }

// загружаем файл конфигурации в глобальную переменную
  jsonConfig = configFile.readString();
  configFile.close();
  // Резервируем памяь для json обекта буфер может рости по мере необходимти предпочтительно для ESP8266 
    DynamicJsonBuffer jsonBuffer;
  //  вызовите парсер JSON через экземпляр jsonBuffer
  //  строку возьмем из глобальной переменной String jsonConfig
    JsonObject& root = jsonBuffer.parseObject(jsonConfig);
  // Теперь можно получить значения из root  
    _ssidAP = root["ssidAPName"].as<String>(); // Так получаем строку
    _passwordAP = root["ssidAPPassword"].as<String>();
    timezone = root["timezone"];               // Так получаем число
    SSDP_Name = root["SSDPName"].as<String>();
    _ssid = root["ssidName"].as<String>();
    _password = root["ssidPassword"].as<String>();
//    light = root["light"];
    String colors[4] = {"red", "green", "blue", "white"};
    for(int i=0; i<4; i++){outVal[i] = root[colors[i]];}

    sinSpd = root["speed"]; //sine speed
    md = root["mode"]; //mode
    dir = root["direction"]; //direction
    minVal = root["min"]; //minimum border
    
    
    Serial.println("Config loaded");
    return true;
}

// Запись данных в файл config.json
bool saveConfig() {
  Serial.print("Saving config.....");
  // Резервируем память для json обекта буфер может рости по мере необходимти предпочтительно для ESP8266 
  DynamicJsonBuffer jsonBuffer;
  //  вызовите парсер JSON через экземпляр jsonBuffer
  JsonObject& json = jsonBuffer.parseObject(jsonConfig);
  // Заполняем поля json 
  json["SSDPName"] = SSDP_Name;
  json["ssidAPName"] = _ssidAP;
  json["ssidAPPassword"] = _passwordAP;
  json["ssidName"] = _ssid;
  json["ssidPassword"] = _password;
  json["timezone"] = timezone;
  //  json["light"] = light;
  String colors[4] = {"red", "green", "blue", "white"};
  for(int i=0; i<4; i++){json[colors[i]] = outVal[i];}

  json["speed"] = sinSpd;
  json["mode"] = md;
  json["direction"] = dir;
  json["min"] = minVal;

  // Помещаем созданный json в глобальную переменную json.printTo(jsonConfig);
  json.printTo(jsonConfig);
  // Открываем файл для записи
  File configFile = SPIFFS.open("/config.json", "w");
  if (!configFile) {
    //Serial.println("Failed to open config file for writing");
    configFile.close();
    Serial.println("Failed to save");
    return false;
  }
  // Записываем строку json в файл 
  json.printTo(configFile);
  configFile.close();
  Serial.println("Config saved");
  return true;
}
