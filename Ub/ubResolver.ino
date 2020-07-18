boolean fetchServerInfo()
{
  //jsonファイルをダウンロード
  WiFiClientSecure client;
  HTTPClient https;
  String payload;
  client.setInsecure();
  Serial.print("[HTTPS] begin...\n");
  if (https.begin(client, "https://gist.githubusercontent.com/634nakajima/52a9158314e0bc5e9f758f6f72cf9bb6/raw/link.json")) {

    // start connection and send HTTP header
    int httpCode = https.GET();

    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTPS] GET... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        payload = https.getString();
        Serial.println(payload);
      }
    } else {
      Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
      return false;
    }
    https.end();
  } else {
    Serial.printf("[HTTPS] Unable to connect\n");
    return false;
  }
  return parse(payload);
}

boolean parse(String p)
{
  JSONVar myObject = JSON.parse(p.c_str());

  if (JSON.typeof(myObject) == "undefined")
  {
    Serial.println("Parsing input failed!");
    return false;
  }

  if (myObject.hasOwnProperty("address") && myObject.hasOwnProperty("port"))
  {
    Serial.print("address: ");
    serverURL = myObject["address"];
    Serial.println(serverURL);

    Serial.print("port: ");
    serverPort = myObject["port"];
    Serial.println(serverPort);
    return true;
  }else {
    Serial.print("fetching failed");
    return false;
  }
}
