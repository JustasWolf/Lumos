String Uptime(){
  return Calc_time(0);
}

String Calc_time(unsigned long start_point){
  String tm = "";
  unsigned long t = millis() - start_point;
  t = t/1000;
  int _sec = t%60;
  int _min = (t/60)%60;
  int _hour = (t/3600)%60;
  tm += _hour;
  tm += ":";
  if(_min<10) {tm += "0";}
  tm += _min;
  tm += ":";
  if(_sec<10) {tm += "0";}
  tm += _sec;
  
  return tm;
}

void H(){
  HTTP.handleClient();
  ArduinoOTA.handle();
}
