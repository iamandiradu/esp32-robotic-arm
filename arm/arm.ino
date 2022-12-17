/*
  Robotic Arm - ESP32 (GroundStudio Carbon V3)
  Controlled by an app made with App Invetor
  by Maurizio Miscio, Radu Iamandi
*/

#include <ESP32Servo.h>

#include "BluetoothSerial.h"

// Initializare modul Bluetooth
BluetoothSerial Bluetooth;

// Initializare motoare servo
Servo servo01;
Servo servo02;
Servo servo03;
Servo servo04;
Servo servo05;
Servo servo06;

int servo1Pos, servo2Pos, servo3Pos, servo4Pos, servo5Pos, servo6Pos; // Pozitia actuala
int servo1PPos, servo2PPos, servo3PPos, servo4PPos, servo5PPos, servo6PPos; // Pozitia precedenta
int servo01SP[50], servo02SP[50], servo03SP[50], servo04SP[50], servo05SP[50], servo06SP[50]; // to hold positions (??)

int velocity = 20; // Viteza
int indexArray = 0;
int dataInput;
int btCmd = 0; // Comanda Bluetooth

void setup() {
  servo01.attach(32, 500, 2400);
  servo02.attach(33, 500, 2400);
  servo03.attach(13, 500, 2400);
  servo04.attach(14, 500, 2400);
  servo05.attach(15, 500, 2400);
  servo06.attach(12, 500, 2400);

  Bluetooth.begin("ESP32_Arm_Control"); //Name of your Bluetooth Signal
  Serial.println("Bluetooth Device is Ready to Pair");
  delay(20);
  Serial.begin(115200);

  // Move all servos to 90 degrees
  servo1PPos = 90;
  servo01.write(servo1PPos);
  servo2PPos = 90;
  servo02.write(servo2PPos);
  servo3PPos = 90;
  servo03.write(servo3PPos);
  servo4PPos = 90;
  servo04.write(servo4PPos);
  servo5PPos = 90;
  servo05.write(servo5PPos);
  servo6PPos = 90;
  servo06.write(servo6PPos);
}

void loop() {
  
  // Check incoming data
  if (Bluetooth.available()) {
    dataInput = Bluetooth.read(); // Read incoming data
    Serial.println(dataInput);
    if (dataInput >= 0 && dataInput <= 27) { // Commands ID
      btCmd = dataInput;
    }

    // Live Robotic Arm Movement:

    // Move Servo 1 clockwise
    while (btCmd == 16) {
      if (Bluetooth.available()) {
        btCmd = Bluetooth.read();
      }
      servo01.write(servo1PPos);
      servo1PPos++;
      delay(velocity);
    }
    // Move Servo 1 counterclockwise
    while (btCmd == 17) {
      if (Bluetooth.available()) {
        btCmd = Bluetooth.read();
      }
      servo01.write(servo1PPos);
      servo1PPos--;
      delay(velocity);
    }

    // Servo2 movement
    while (btCmd == 19) {
      if (Bluetooth.available()) {
        btCmd = Bluetooth.read();
      }
      servo02.write(servo2PPos);
      servo2PPos++;
      delay(velocity);
    }
    while (btCmd == 18) {
      if (Bluetooth.available()) {
        btCmd = Bluetooth.read();
      }
      servo02.write(servo2PPos);
      servo2PPos--;
      delay(velocity);
    }

    // Servo3 movement
    while (btCmd == 20) {
      if (Bluetooth.available()) {
        btCmd = Bluetooth.read();
      }
      servo03.write(servo3PPos);
      servo3PPos++;
      delay(velocity);
    }
    while (btCmd == 21) {
      if (Bluetooth.available()) {
        btCmd = Bluetooth.read();
      }
      servo03.write(servo3PPos);
      servo3PPos--;
      delay(velocity);
    }

    // Servo4 movement
    while (btCmd == 23) {
      if (Bluetooth.available()) {
        btCmd = Bluetooth.read();
      }
      servo04.write(servo4PPos);
      servo4PPos++;
      delay(velocity);
    }
    while (btCmd == 22) {
      if (Bluetooth.available()) {
        btCmd = Bluetooth.read();
      }
      servo04.write(servo4PPos);
      servo4PPos--;
      delay(velocity);
    }

    // Servo5 movement
    while (btCmd == 25) {
      if (Bluetooth.available()) {
        btCmd = Bluetooth.read();
      }
      servo05.write(servo5PPos);
      servo5PPos++;
      delay(velocity);
    }
    while (btCmd == 24) {
      if (Bluetooth.available()) {
        btCmd = Bluetooth.read();
      }
      servo05.write(servo5PPos);
      servo5PPos--;
      delay(velocity);
    }

    // Servo6 movement
    while (btCmd == 26) {
      if (Bluetooth.available()) {
        btCmd = Bluetooth.read();
      }
      servo06.write(servo6PPos);
      servo6PPos++;
      delay(velocity);
    }
    while (btCmd == 27) {
      if (Bluetooth.available()) {
        btCmd = Bluetooth.read();
      }
      servo06.write(servo6PPos);
      servo6PPos--;
      delay(velocity);
    }

    // If the speed slider is switched...
    if (dataInput > 101 && dataInput < 250) {
      velocity = dataInput / 10; // Change servo speed (delay between steps)
    }

    // If the SAVE key is pressed from the APP...
    if (btCmd == 12) {
      servo01SP[indexArray] = servo1PPos;  // Save the position of the servos in the Arrays
      servo02SP[indexArray] = servo2PPos;
      servo03SP[indexArray] = servo3PPos;
      servo04SP[indexArray] = servo4PPos;
      servo05SP[indexArray] = servo5PPos;
      servo06SP[indexArray] = servo6PPos;
      indexArray++;  // Increase the index of the Array
      btCmd= 0;
    }

    // If the START button is pressed
    if (btCmd == 14) {
      while (dataInput != 13) { // Carry out the steps one after the other until the "RESET" key is pressed
        for (int i = 0; i <= indexArray - 2; i++) { // for loop to repeat as many times as the saved positions
          if (Bluetooth.available()) { // Check incoming data
            dataInput = Bluetooth.read();
            if (dataInput == 15) { // If the "PAUSE" button is pressed...
              while (dataInput != 14) { // Wait until the "START" button is pressed again
                if (Bluetooth.available()) {
                  dataInput = Bluetooth.read();
                  if (dataInput == 13) {
                    Serial.println("RESET");
                    break;
                  }
                }
              }
            }
            // If the position of the "speed" cursor has changed...
            if (dataInput > 100 && dataInput < 150) {
              velocity = dataInput / 10; // Change the speed of the servos (delay between one step and another)
            }
          }

          // Servo1 movement
          if (servo01SP[i] == servo01SP[i + 1]) { }
          if (servo01SP[i] > servo01SP[i + 1]) {
            for (int j = servo01SP[i]; j >= servo01SP[i + 1]; j--) {
              servo01.write(j);
              delay(velocity);
            }
          }
          if (servo01SP[i] < servo01SP[i + 1]) {
            for (int j = servo01SP[i]; j <= servo01SP[i + 1]; j++) {
              servo01.write(j);
              delay(velocity);
            }
          }

          // Servo2 movement
          if (servo02SP[i] == servo02SP[i + 1]) {}
          if (servo02SP[i] > servo02SP[i + 1]) {
            for (int j = servo02SP[i]; j >= servo02SP[i + 1]; j--) {
              servo02.write(j);
              delay(velocity);
            }
          }
          if (servo02SP[i] < servo02SP[i + 1]) {
            for (int j = servo02SP[i]; j <= servo02SP[i + 1]; j++) {
              servo02.write(j);
              delay(velocity);
            }
          }

          // Servo3 movement
          if (servo03SP[i] == servo03SP[i + 1]) {}
          if (servo03SP[i] > servo03SP[i + 1]) {
            for (int j = servo03SP[i]; j >= servo03SP[i + 1]; j--) {
              servo03.write(j);
              delay(velocity);
            }
          }
          if (servo03SP[i] < servo03SP[i + 1]) {
            for (int j = servo03SP[i]; j <= servo03SP[i + 1]; j++) {
              servo03.write(j);
              delay(velocity);
            }
          }

          // Servo4 movement
          if (servo04SP[i] == servo04SP[i + 1]) {}
          if (servo04SP[i] > servo04SP[i + 1]) {
            for (int j = servo04SP[i]; j >= servo04SP[i + 1]; j--) {
              servo04.write(j);
              delay(velocity);
            }
          }
          if (servo04SP[i] < servo04SP[i + 1]) {
            for (int j = servo04SP[i]; j <= servo04SP[i + 1]; j++) {
              servo04.write(j);
              delay(velocity);
            }
          }

          // Servo5 movement
          if (servo05SP[i] == servo05SP[i + 1]) {}
          if (servo05SP[i] > servo05SP[i + 1]) {
            for (int j = servo05SP[i]; j >= servo05SP[i + 1]; j--) {
              servo05.write(j);
              delay(velocity);
            }
          }
          if (servo05SP[i] < servo05SP[i + 1]) {
            for (int j = servo05SP[i]; j <= servo05SP[i + 1]; j++) {
              servo05.write(j);
              delay(velocity);
            }
          }

          // Servo6 movement
          if (servo06SP[i] == servo06SP[i + 1]) {}
          if (servo06SP[i] > servo06SP[i + 1]) {
            for (int j = servo06SP[i]; j >= servo06SP[i + 1]; j--) {
              servo06.write(j);
              delay(velocity);
            }
          }
          if (servo06SP[i] < servo06SP[i + 1]) {
            for (int j = servo06SP[i]; j <= servo06SP[i + 1]; j++) {
              servo06.write(j);
              delay(velocity);
            }
          }
        }
      }
     // If the "RESET" button was pressed...
      if (dataInput != 14) {
        memset(servo01SP, 0, sizeof(servo01SP));  // Empty the data from the Array
        memset(servo02SP, 0, sizeof(servo02SP));
        memset(servo03SP, 0, sizeof(servo03SP));
        memset(servo04SP, 0, sizeof(servo04SP));
        memset(servo05SP, 0, sizeof(servo05SP));
        memset(servo06SP, 0, sizeof(servo06SP));
        indexArray = 0;  // Index to 0
      }
    }
  }
}