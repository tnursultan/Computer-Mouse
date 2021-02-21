#include <Wire.h>
#include <Mouse.h>
#include <MPU6050.h>
#include <I2Cdev.h>

// initialise gyroscope and mouse position parameters
MPU6050 mpu;
int16_t ax, ay, az, gx, gy, gz;
int vx, vy;

// map mouse buttons to pins on the microcontroller
const int left_button = 16;
const int right_button = 15;
const int scroll_button = 19;

void setup() {
  // initalise libraries
  Serial.begin(9600);
  Wire.begin();
  Mouse.begin();

  // gyroscope initialisation and check
  mpu.initialize();
  if (!mpu.testConnection()) { 
    while (1);
  }

  // initialise mouse buttons
  pinMode(left_button, INPUT);
  pinMode(right_button, INPUT);
  pinMode(scroll_button, INPUT);
}

void loop() {
  // read the current accelerometer and gyroscope parameters
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // divide by a coefficient to decrease cursor sensitivity
  // (bigger denominator -> slower cursor)
  // reverse mapped the x and y variables to use MPU in a horizontal orientation
  vx = gy/400;
  vy = gx/400;
  
  Mouse.move(vx, vy);

  // read if the buttons of the mouse have been clicked
  int left_clickState = digitalRead(left_button);
  int right_clickState = digitalRead(right_button);
  int scroll_clickState = digitalRead(scroll_button);

  // if the left button of the mouse is clicked:
  if (left_clickState == HIGH) {
    if (!Mouse.isPressed(MOUSE_LEFT))
      Mouse.press(MOUSE_LEFT);
  } else {
    if (Mouse.isPressed(MOUSE_LEFT))
      Mouse.release(MOUSE_LEFT);
  }

  // if the right button of the mouse is clicked:
  if (right_clickState == HIGH) {
    if (!Mouse.isPressed(MOUSE_RIGHT))
      Mouse.press(MOUSE_RIGHT);
  } else {
    if (Mouse.isPressed(MOUSE_RIGHT))
      Mouse.release(MOUSE_RIGHT);
  }

  // if the scroll (middle) button of the mouse is clicked:
  if (scroll_clickState == HIGH) {
    if (!Mouse.isPressed(MOUSE_MIDDLE))
      Mouse.press(MOUSE_MIDDLE);
  } else {
    if (Mouse.isPressed(MOUSE_MIDDLE))
      Mouse.release(MOUSE_MIDDLE);
  }

  delay(20);
}
