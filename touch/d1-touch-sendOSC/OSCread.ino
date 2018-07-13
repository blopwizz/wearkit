OSCErrorCode error;
char str[256];

void readOSC() {
  OSCMessage receivedMessage;
  int size = Udp.parsePacket();

  if (size > 0) {
    while (size--) {
      receivedMessage.fill(Udp.read());
    }
    if (!receivedMessage.hasError()) {
    } else {
      error = receivedMessage.getError();
      Serial.print("error: ");
      Serial.println(error);
      Serial.print("type: ");
      Serial.println(receivedMessage.getType(0));
    }
  }

}
