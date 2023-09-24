
/*
   Lego Inventor Python

  import struct
  import hub
  p=hub.port.A
  p.device.get()

  p.device.mode(0,struct.pack('2h',1,2)

*/

/* pybricks


*/


#include "LPF2_local.h"

#define RXD2 18
#define TXD2 19
EV3UARTEmulation sensor(RXD2, TXD2, 62, 115200);

void demo_callback(byte buf[], byte s) {
  byte nr_short = int(s / 2);
  short vals[nr_short];
  Serial.printf("size %d, nr short %d\n", s, nr_short);
  for (int i = 0; i < nr_short; i++) {
    vals[i] = buf[i * 2] + buf[i * 2 + 1] * 256;
    Serial.printf("vals[%d]=%d\n", i, vals[i]);
  }
}

void demo_callback1(byte buf[], byte s) {
  Serial.printf("Mode 1\r\n");
  Serial.printf("size %d \n", s);
  for (int i = 0; i < s; i++) {
    Serial.printf("buf[%d]=%d\n", i, buf[i]);
  }
}

void demo_callback2(byte buf[], byte s) {
  Serial.printf("Mode 2\r\n");
  Serial.printf("size %d \n", s);
  for (int i = 0; i < s; i++) {
    Serial.printf("buf[%d]=%d\n", i, buf[i]);
  }
}


void setup() {
  Serial.begin(115200);
  sensor.create_mode("GP32", true, DATA8, 32, 5, 0, 0.0f, 512.0f, 0.0f, 1024.0f, 0.0f, 100.0f, "RAW", ABSOLUTE, ABSOLUTE);
  sensor.create_mode("rgb", true, DATA8, 16, 5, 0, 0.0f, 100.0f, 0.0f, 100.0f, 0.0f, 100.0f, "RAW", ABSOLUTE, ABSOLUTE); //map in and map out unit = "XYBD" = x, y, buttons, d-pad
  sensor.create_mode("aaa", true, DATA8, 8, 3, 0, 0.0f, 100.0f, 0.0f, 100.0f, 0.0f, 100.0f, "XYBD", ABSOLUTE, ABSOLUTE); //map in and map out unit = "XYBD" = x, y, buttons, d-pad
  sensor.get_mode(0)->setCallback(demo_callback);  // attach call back function to mode 0
  sensor.get_mode(1)->setCallback(demo_callback1);  // attach call back function to mode 1
  sensor.get_mode(2)->setCallback(demo_callback2);  // attach call back function to mode 2
  sensor.reset();
  delay(200);

}

short counter = 0;
unsigned long last_reading = 0;

void loop() {
  // This call fetches all the gamepad info from the NINA (ESP32) module.
  // Just call this function in your main loop.
  // The gamepads pointer (the ones received in the callbacks) gets updated
  // automatically.
  sensor.heart_beat();
  if (millis() - last_reading > 20) {
    int mode = sensor.get_current_mode();
    if (mode == 0) {
      byte bb[32];
      memset(bb, 0, 32);
      bb[0] = counter;
      bb[1] = counter * 2;
      counter += 1;
      sensor.send_data8(bb, 32);
    }

    if (mode == 1) {
      byte bb[16];
      memset(bb, 0, 16);
      bb[0] = counter;
      bb[1] = counter * 3;
      counter += 1;
      sensor.send_data8(bb, 16);
    }
    if (mode == 2) {
      byte bb[8];
      memset(bb, 0, 8);
      bb[0] = counter;
      bb[1] = counter + 1;
      counter += 1;
      sensor.send_data8(bb, 8);
    }
    last_reading = millis();
  }
  vTaskDelay(1);
  //delay(150);
}
