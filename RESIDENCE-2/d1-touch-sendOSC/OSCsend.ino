void sendOSC() {
  OSCMessage msg1("/vResistivity");
  msg1.add(vResistivity);
  Udp.beginPacket(outIp, outPort);
  msg1.send(Udp);
  Udp.endPacket();
  msg1.empty();
}
