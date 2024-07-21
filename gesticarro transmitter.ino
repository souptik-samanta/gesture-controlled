#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>

int ADXL345 = 0x53;
int X_out, Y_out, Z_out;

RF24 radio(8, 10);
const byte address[6] = "00001";

struct data {
  float xAxis;
  float yAxis;
};
data send_data;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  Wire.beginTransmission(ADXL345);
  Wire.write(0x2D);
  Wire.write(8);
  Wire.endTransmission();
  delay(10);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_250KBPS);
  radio.stopListening();
}

void loop() {
  Wire.beginTransmission(ADXL345);
  Wire.write(0x32);
  Wire.endTransmission(false);
  Wire.requestFrom(ADXL345, 6, true);
  X_out = (Wire.read() | Wire.read() << 8);
  Y_out = (Wire.read() | Wire.read() << 8);
  Z_out = (Wire.read() | Wire.read() << 8);
  send_data.xAxis = (int)X_out;
  send_data.yAxis = Y_out;

  radio.write(&send_data, sizeof(data));
  Serial.print("Sent X: ");
  Serial.print(send_data.xAxis);
  Serial.print(" Y: ");
  Serial.println(send_data.yAxis);
  delay(10); // Adjust the delay as needed
}
