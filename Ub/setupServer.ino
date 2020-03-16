#ifndef APSSID
#define APSSID "ubiquitel"
#endif

/* Set these to your desired credentials. */
const char *ubssid = APSSID;

ESP8266WebServer server(80);

const String postForms = "<html>\
  <head>\
    <title>ESP8266 Web Server POST handling</title>\
    <style>\
      body { background-color: #ddd; font-family: Arial, Helvetica, Sans-Serif; Color: #000000; }\
    </style>\
  </head>\
  <body>\
    <form method=\"post\" enctype=\"application/x-www-form-urlencoded\" action=\"/postform/\">\
      <h1>SSID</h1>\
      <input type=\"text\" name=\"ssid\"><br>\
      <h1>Password</h1>\
      <input type=\"text\" name=\"password\"><br>\
      <input type=\"submit\" value=\"Submit\">\
    </form>\
  </body>\
</html>";

void handleRoot() {
  digitalWrite(LED, 1);
  server.send(200, "text/html", postForms);
  digitalWrite(LED, 0);
}

void handlePlain() {
  if (server.method() != HTTP_POST) {
    digitalWrite(LED, 1);
    server.send(405, "text/plain", "Method Not Allowed");
    digitalWrite(LED, 0);
  } else {
    digitalWrite(LED, 1);
    server.send(200, "text/plain", "POST body was:\n" + server.arg("plain"));
    Serial.println(server.arg("plain"));
    digitalWrite(LED, 0);
  }
}

void handleForm() {
  if (server.method() != HTTP_POST) {
    digitalWrite(LED, 1);
    server.send(405, "text/plain", "Method Not Allowed");
    digitalWrite(LED, 0);
  } else {
    digitalWrite(LED, 1);
    String message = "POST form was:\n";
    for (uint8_t i = 0; i < server.args(); i++) {
      message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
      Serial.println(server.arg(i));
    }
    writeAPInfo(server.arg(0), server.arg(1));
    server.send(200, "text/plain", message);
    digitalWrite(LED, 0);
  }
}

void handleNotFound() {
  digitalWrite(LED, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(LED, 0);
}

void setupServer() {
  WiFi.disconnect();
  WiFi.softAP(ubssid);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  server.on("/", handleRoot);
  server.on("/postplain/", handlePlain);
  server.on("/postform/", handleForm);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void loopServer() {
  server.handleClient();
}

void setupEEPROM() {
  Serial.begin(115200);
  EEPROM.begin(512);
  readAPSSID(ssid);
  readAPPassword(password);
}

void writeAPInfo(String writingSSID, String writingPassword) {
  for (int i = 0; i < 256; i++) {
    EEPROM.write(i, writingSSID.c_str()[i]);
    EEPROM.write(i + 256, writingPassword.c_str()[i]);
  }
  if (EEPROM.commit()) {
    Serial.print("rewrite SSID: ");
    Serial.println(writingSSID);
    Serial.print("rewrite password: ");
    Serial.println(writingPassword);
  } else {
    Serial.println("ERROR! EEPROM commit failed");
  }
}

char *readAPSSID(char *readingSSID) {
  for (int i = 0; i < 256; i++) {
    readingSSID[i] = EEPROM.read(i);
  }
  Serial.print("read SSID: ");
  Serial.println(readingSSID);
  return readingSSID;
}

char *readAPPassword(char *readingPassword) {
  for (int i = 0; i < 256; i++) {
    readingPassword[i] = EEPROM.read(i + 256);
  }
  Serial.print("read password: ");
  Serial.println(readingPassword);
  return readingPassword;
}
