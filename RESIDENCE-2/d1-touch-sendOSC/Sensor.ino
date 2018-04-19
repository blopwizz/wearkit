void readSensor() {
  vSensor = analogRead(A0);
  vSum = vSum - vvSensor[iReading];
  vvSensor[iReading] = vSensor;
  vSum = vSum + vvSensor[iReading];
  iReading += 1;
  if (iReading >= N_SMOOTHING) {
    iReading = 0;
  }
  vResistivity = vSum / N_SMOOTHING;
}

void printSensor() {
//  Serial.print("0 ");
//  Serial.print(vSensor);
//  Serial.print(" ");
//  Serial.print(vResistivity);
//  Serial.println(" 1024");
}
