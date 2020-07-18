void event(const char *payload, size_t length)
{
  Serial.printf("got message: %s\n", payload);
}

void setupSocketIO()
{
  if (fetchServerInfo()) {
    webSocket.on("FINGERControl/SEND", event);
    webSocket.begin(serverURL.c_str(), serverPort);
  } else {
    Serial.println("SocketIO connection failed");
  }
}
