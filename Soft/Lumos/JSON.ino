String statusJSON(){
  String statsjson = "";
  statsjson += "{";

  //выключатель
  statsjson += "\"light\":{\"type\":\"bool\",\"value\":\"";
  if(light == 1) {statsjson += "true";}
  else statsjson += "false";
  statsjson += "\"},";

  //mode
  statsjson += "\"mode\":{\"type\":\"int\",\"value\":\"";
  statsjson += md;
  statsjson += "\"},";

  //direction
  statsjson += "\"direction\":{\"type\":\"int\",\"value\":\"";
  statsjson += dir;
  statsjson += "\"},";

  //speed
  statsjson += "\"speed\":{\"type\":\"int\",\"value\":\"";
  statsjson += sinSpd;
  statsjson += "\"},";

  //speed
  statsjson += "\"min\":{\"type\":\"int\",\"value\":\"";
  statsjson += minVal;
  statsjson += "\"},";
  
  //цвета
  String colors[4] = {"red", "green", "blue", "white"};
  for(int i=0; i<4; i++){
    statsjson += "\"";
    statsjson += colors[i];
    statsjson += "\":{\"type\":\"int\",\"value\":\"";
    statsjson += outVal[i];
    statsjson += "\"},";
  }

  //аптайм
  statsjson += "\"uptime\":{\"type\":\"string\",\"value\":\"";
  statsjson += Uptime();
  statsjson += "\"}";

  statsjson += "}";
  return statsjson;
}
