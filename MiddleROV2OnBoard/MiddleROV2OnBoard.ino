/*
 Name:		MiddleROV2Surface.ino
 Created:	06.12.2018 17:37:33
 Author:	Zinkov
*/

// the setup function runs once when you press reset or power the board
#include <Arduino.h>
#include "BrushlessMotor.h"
#include "RotaryCamera.h"

#define RS485_CONTROL_PIN 2

#define LEFT_MOTOR_PIN 4
#define RIGHT_MOTOR_PIN 6
#define VERTICAL_MOTOR_PIN 3

#define MANIPULATOR_PIN 7
#define CAMERA_PIN 8

BrushlessMotor leftMotor(LEFT_MOTOR_PIN);
BrushlessMotor rightMotor(RIGHT_MOTOR_PIN);
BrushlessMotor verticalMotor(VERTICAL_MOTOR_PIN);

RotaryCamera camera(CAMERA_PIN);
RotaryCamera manipulator(MANIPULATOR_PIN);

void setup() {
	Serial.begin(115200);
	Serial1.begin(115200);
	pinMode(RS485_CONTROL_PIN, OUTPUT);
	digitalWrite(RS485_CONTROL_PIN, HIGH);
	leftMotor.init();
	rightMotor.init();
	verticalMotor.init();
	camera.init();
	manipulator.init();
	//(2000);

	digitalWrite(RS485_CONTROL_PIN, LOW);
}

#define START_BYTE 0xFE
#define END_BYTE 0xEF

#define DEBUG

void loop() {
	//Serial.println(Serial1.available());
	if (Serial1.available() > 6) {
		uint8_t buffer[7] = {};
		Serial1.readBytes(buffer, 7);
		if (buffer[0] == START_BYTE && buffer[6] == END_BYTE) {
			leftMotor.set_power(buffer[1]);
			rightMotor.set_power(buffer[2]);
			verticalMotor.set_power(buffer[3]);
			camera.rotate(buffer[4]);
			manipulator.rotate(buffer[5]);
#ifdef DEBUG
			Serial.print((int8_t)buffer[1]); Serial.print(" ");
			Serial.print((int8_t)buffer[2]); Serial.print(" ");
			Serial.print((int8_t)buffer[3]); Serial.print(" ");
			Serial.print((int8_t)buffer[4]); Serial.print(" ");
			Serial.println((int8_t)buffer[5]);
#endif // DEBUG
		}
		else {
			Serial.println("ERROR");
			Serial.println(Serial1.readString());
		}
	}
}
