/******************************************************************************
 *
 *  Copyright (C) 2014-2015 X2bus (info@x2bus.com)
 *  Can not be copied and/or distributed without the express permission of X2bus
 *
 *  Project   : EnergyBoard Library
 *  Developer : Mehmet Gunce Akkoyun (gunce.akkoyun@x2bus.com)
 *  Hardware  : Alp Erkan Savli (alp.erkan@x2bus.com)
 *  Revision  : A0.1.0
 *
 ******************************************************************************/

#include "Arduino.h"
#include "MAX78630.h"

#define MAX_Serial Serial2

/*
	MAX78630 Serial Comminication Read Values Structure
	---------------------------------------------------
	Read Request : [1]-[2]-[3]-[4]-[5]-[6]-[7]
	---------------------------------------------------
	1. byte is the IC address byte
	2. byte is the total sended byte
	3. byte is the package payload type
	4. byte is the request (RMS) byte (2)
	5. byte is the request (RMS) byte (1)
	6. byte is the requested byte count
	7. byte is the CRC correction byte
 
	Library Usage
	-------------
	Voltage_R = MAX78630::Voltage('R');
	Voltage_S = MAX78630::Voltage('S');
	Voltage_T = MAX78630::Voltage('T');

	Current_R = MAX78630::Current('R');
	Current_S = MAX78630::Current('S');
	Current_T = MAX78630::Current('T');

	ActivePower_R = MAX78630::ActivePower('R');
	ActivePower_S = MAX78630::ActivePower('S');
	ActivePower_T = MAX78630::ActivePower('T');

	ReactivePower_R = MAX78630::ReactivePower('R');
	ReactivePower_S = MAX78630::ReactivePower('S');
	ReactivePower_T = MAX78630::ReactivePower('T');

	ApparentPower_R = MAX78630::ApparentPower('R');
	ApparentPower_S = MAX78630::ApparentPower('S');
	ApparentPower_T = MAX78630::ApparentPower('T');

	PowerFactor_R = MAX78630::PowerFactor('R');
	PowerFactor_S = MAX78630::PowerFactor('S');
	PowerFactor_T = MAX78630::PowerFactor('T');

	Frequency = MAX78630::Frequency();
*/

MAX78630::MAX78630(int Gain) {
	MAX_Serial.begin(38400);

	MAX_Serial.flush(); while(MAX_Serial.available() > 0) char _t = MAX_Serial.read();

	MAX_Serial.print(char(170));	// 0xAA
	MAX_Serial.print(char(04));		// 0x04
	MAX_Serial.print(char(196));	// 0xC4
	MAX_Serial.print(char(142));	// 0x8E

	if (Gain != 0) Gain = 1;

	MAX78630_Gain = Gain;										// Make Global Variable
}
float MAX78630::Voltage(char Phase) {

	MAX_Serial.flush(); while(MAX_Serial.available() > 0) char _t = MAX_Serial.read();

	float Voltage_ = 0;
	byte inBuffer[6];
	int bufIndx = 0;

	// RMS Voltage Read
	// ----------------
	// RMS Voltage Phase R = 0xAA,0x07,0xA3,0xAA,0x00,0xE3,0x39
	// RMS Voltage Phase S = 0xAA,0x07,0xA3,0x93,0x00,0xE3,0x36
	// RMS Voltage Phase T = 0xAA,0x07,0xA3,0x96,0x00,0xE3,0x33

	MAX_Serial.print(char(170));	// (0xAA)
	MAX_Serial.print(char(07));		// (0x07)
	MAX_Serial.print(char(163));	// (0xA3)

	switch (Phase) {
        case 'R':
            MAX_Serial.print(char(144));    // (0xAA)
            MAX_Serial.print(char(0));      // (0x00)
            MAX_Serial.print(char(227));    // (0xE3)
            MAX_Serial.print(char(57));     // (0x39)
            break;
        case 'S':
            MAX_Serial.print(char(147));    // (0x93)
            MAX_Serial.print(char(0));      // (0x00)
            MAX_Serial.print(char(227));    // (0xE3)
            MAX_Serial.print(char(54));     // (0x36)
            break;
        case 'T':
            MAX_Serial.print(char(150));    // (0x96)
            MAX_Serial.print(char(0));      // (0x00)
            MAX_Serial.print(char(227));    // (0xE3)
            MAX_Serial.print(char(51));     // (0x33)
            break;
        default:
            return 0;
    }
	delay(5);

	if (MAX_Serial.available()) {
		while (MAX_Serial.available()) {
			inBuffer[bufIndx] = MAX_Serial.read();
			bufIndx ++;
		}
		
		byte r = inBuffer[4];
		byte t = inBuffer[3];
		byte y = inBuffer[2];

		if (MAX78630_Gain == 1) {
			Voltage_ = (r * 65536 + t * 256 + y) * MAX78630_Voltage_Gain;
		}
		else {
			Voltage_ = (r * 65536 + t * 256 + y);
		} // Gain Correction Selection


		inBuffer[bufIndx] = 0;
		bufIndx = 0;
	}
    else {
		Voltage_ = 0;
	}

	return abs(Voltage_);
}
float MAX78630::Current(char Phase) {

	MAX_Serial.flush(); while(MAX_Serial.available() > 0) char _t = MAX_Serial.read();

	float Current_ = 0;
	byte inBuffer[6];
	int bufIndx = 0;

	// RMS Current Read
	// ----------------
	// RMS Current Phase R = 0xAA,0x07,0xA3,0xD5,0x00,0xE3,0xF4
	// RMS Current Phase S = 0xAA,0x07,0xA3,0xD8,0x00,0xE3,0xF1
	// RMS Current Phase T = 0xAA,0x07,0xA3,0xDB,0x00,0xE3,0xEE

	MAX_Serial.print(char(170));	// (0xAA)
	MAX_Serial.print(char(07));		// (0x07)
	MAX_Serial.print(char(163));	// (0xA3)

	switch (Phase) {
		case 'R':
			MAX_Serial.print(char(213));    // (0xD5)
			MAX_Serial.print(char(0));      // (0x00)
			MAX_Serial.print(char(227));    // (0xE3)
			MAX_Serial.print(char(244));    // (0xF4)
			break;
		case 'S':
			MAX_Serial.print(char(216));    // (0xD8)
			MAX_Serial.print(char(0));      // (0x00)
			MAX_Serial.print(char(227));    // (0xE3)
			MAX_Serial.print(char(241));    // (0xF1)
			break;
		case 'T':
			MAX_Serial.print(char(219));    // (0xDB)
			MAX_Serial.print(char(0));      // (0x00)
			MAX_Serial.print(char(227));    // (0xE3)
			MAX_Serial.print(char(238));    // (0xEE)
			break;
		default:
			return 0;
	}

	delay(5);

	if (MAX_Serial.available()) {
		while (MAX_Serial.available()) {
			inBuffer[bufIndx] = MAX_Serial.read();
			bufIndx ++;
		}

		byte r = inBuffer[4];
		byte t = inBuffer[3];
		byte y = inBuffer[2];

		if (MAX78630_Gain == 1) {
			Current_ = (r * 65536 + t * 256 + y) * MAX78630_Current_Gain;
		}
		else {
			Current_ = (r * 65536 + t * 256 + y);
		} // Gain Correction Selection

		inBuffer[bufIndx] = 0;
		bufIndx = 0;
	}
    else
	{
		Current_ = 0;
	}

	return abs(Current_);
}
float MAX78630::ActivePower(char Phase) {

	MAX_Serial.flush(); while(MAX_Serial.available() > 0) char _t = MAX_Serial.read();

	float ActivePower_ = 0;
	byte inBuffer[6];
	int bufIndx = 0;

	// Active Power Read
	// -----------------
	// Active Power Phase R = 0xAA,0x07,0xA3,0x1D,0x01,0xE3,0xAB
	// Active Power Phase S = 0xAA,0x07,0xA3,0x20,0x01,0xE3,0xA8
	// Active Power Phase T = 0xAA,0x07,0xA3,0x23,0x01,0xE3,0xEE

	MAX_Serial.print(char(170));	// (0xAA)
	MAX_Serial.print(char(07));		// (0x07)
	MAX_Serial.print(char(163));	// (0xA3)

	switch (Phase) {
		case 'R':
			MAX_Serial.print(char(29));     // (0x1D)
			MAX_Serial.print(char(1));      // (0x01)
			MAX_Serial.print(char(227));    // (0xE3)
			MAX_Serial.print(char(171));    // (0xAB)
			break;
		case 'S':
			MAX_Serial.print(char(32));     // (0x20)
			MAX_Serial.print(char(1));      // (0x01)
			MAX_Serial.print(char(227));    // (0xE3)
			MAX_Serial.print(char(168));    // (0xA8)
			break;
		case 'T':
			MAX_Serial.print(char(35));     // (0x23)
			MAX_Serial.print(char(1));      // (0x01)
			MAX_Serial.print(char(227));    // (0xE3)
			MAX_Serial.print(char(165));    // (0xEE)
			break;
		default:
			return 0;
	}

	delay(5);

	if (MAX_Serial.available()) {
		while (MAX_Serial.available()) {
			inBuffer[bufIndx] = MAX_Serial.read();
			bufIndx ++;
		}
		
		byte r = inBuffer[4];
		byte t = inBuffer[3];
		byte y = inBuffer[2];

		if (MAX78630_Gain == 1) {
			if (r < 128) {
				ActivePower_ = (r * 65536 + t * 256 + y) * MAX78630_Power_Gain;
			}
			else {
				ActivePower_ = -((255 - r) * 65536 + (255 - t) * 256 + (255 - y)) * MAX78630_Power_Gain;
			}
		}
		else {
			if (r < 128) {
				ActivePower_ = (r * 65536 + t * 256 + y);
			}
			else {
				ActivePower_ = -((255 - r) * 65536 + (255 - t) * 256 + (255 - y));
			}
		} // Gain Correction Selection

		inBuffer[bufIndx] = 0;
		bufIndx = 0;
	}
	else
	{
		ActivePower_ = 0;
	}

	return abs(ActivePower_);
}
float MAX78630::ReactivePower(char Phase) {

	MAX_Serial.flush(); while(MAX_Serial.available() > 0) char _t = MAX_Serial.read();

	float ReactivePower_ = 0;
	byte inBuffer[6];
	int bufIndx = 0;

	// ReActive Power Read
	// -------------------
	// ReActive Power Phase R = 0xAA,0x07,0xA3,0x26,0x01,0xE3,0xA2
	// ReActive Power Phase S = 0xAA,0x07,0xA3,0x29,0x01,0xE3,0x9F
	// ReActive Power Phase T = 0xAA,0x07,0xA3,0x2C,0x01,0xE3,0x9C

	MAX_Serial.print(char(170));	// (0xAA)
	MAX_Serial.print(char(7));		// (0x07)
	MAX_Serial.print(char(163));	// (0xA3)

	switch (Phase) {
		case 'R':
			MAX_Serial.print(char(38));     // (0x26)
			MAX_Serial.print(char(1));      // (0x01)
			MAX_Serial.print(char(227));    // (0xE3)
			MAX_Serial.print(char(162));    // (0xA2)
			break;
		case 'S':
			MAX_Serial.print(char(41));     // (0x29)
			MAX_Serial.print(char(1));      // (0x01)
			MAX_Serial.print(char(227));    // (0xE3)
			MAX_Serial.print(char(159));    // (0x9F)
			break;
		case 'T':
			MAX_Serial.print(char(44));     // (0x2C)
			MAX_Serial.print(char(1));      // (0x01)
			MAX_Serial.print(char(227));    // (0xE3)
			MAX_Serial.print(char(156));    // (0x9C)
			break;
		default:
			return 0;
	}

	delay(5);

	if (MAX_Serial.available()) {
		while (MAX_Serial.available()) {
			inBuffer[bufIndx] = MAX_Serial.read();
			bufIndx ++;
		}

		byte r = inBuffer[4];
		byte t = inBuffer[3];
		byte y = inBuffer[2];

		if (MAX78630_Gain == 1) {
			if (r < 128) {
				ReactivePower_ = (r * 65536 + t * 256 + y) * MAX78630_Power_Gain;
			}
			else {
				ReactivePower_ = -((255 - r) * 65536 + (255 - t) * 256 + (255 - y)) * MAX78630_Power_Gain;
			}
		}
		else {
			if (r < 128) {
				ReactivePower_ = (r * 65536 + t * 256 + y);
			}
			else {
				ReactivePower_ = -((255 - r) * 65536 + (255 - t) * 256 + (255 - y));
			}
		} // Gain Correction Selection

		inBuffer[bufIndx] = 0;
		bufIndx = 0;
	}
	else
	{
		ReactivePower_ = 0;
	}

	return abs(ReactivePower_);
}
float MAX78630::ApparentPower(char Phase) {

	MAX_Serial.flush(); while(MAX_Serial.available() > 0) char _t = MAX_Serial.read();

	float ApparentPower_ = 0;
	byte inBuffer[6];
	int bufIndx = 0;

	// Apparent Power Read
	// -------------------
	// Apparent Power Phase R = 0xAA,0x07,0xA3,0x2F,0x01,0xE3,0x99
	// Apparent Power Phase S = 0xAA,0x07,0xA3,0x32,0x01,0xE3,0x96
	// Apparent Power Phase T = 0xAA,0x07,0xA3,0x35,0x01,0xE3,0x93

	MAX_Serial.print(char(170));	// (0xAA)
	MAX_Serial.print(char(7));		// (0x07)
	MAX_Serial.print(char(163));	// (0xA3)

	switch (Phase) {
		case 'R':
			MAX_Serial.print(char(47));     // (0x2F)
			MAX_Serial.print(char(1));      // (0x01)
			MAX_Serial.print(char(227));    // (0xE3)
			MAX_Serial.print(char(153));    // (0x99)
			break;
		case 'S':
			MAX_Serial.print(char(50));     // (0x32)
			MAX_Serial.print(char(1));      // (0x01)
			MAX_Serial.print(char(227));    // (0xE3)
			MAX_Serial.print(char(150));    // (0x96)
			break;
		case 'T':
			MAX_Serial.print(char(53));     // (0x35)
			MAX_Serial.print(char(1));      // (0x01)
			MAX_Serial.print(char(227));    // (0xE3)
			MAX_Serial.print(char(147));    // (0x93)
			break;
		default:
			return 0;
	}

	delay(5);

	if (MAX_Serial.available()) {
		while (MAX_Serial.available()) {
			inBuffer[bufIndx] = MAX_Serial.read();
			bufIndx ++;
		}

		byte r = inBuffer[4];
		byte t = inBuffer[3];
		byte y = inBuffer[2];

		if (MAX78630_Gain == 1) {
			if (r < 128) {
				ApparentPower_ = (r * 65536 + t * 256 + y) * MAX78630_Power_Gain;
			}
			else {
				ApparentPower_ = -((255 - r) * 65536 + (255 - t) * 256 + (255 - y)) * MAX78630_Power_Gain;
			}
		}
		else {
			if (r < 128) {
				ApparentPower_ = (r * 65536 + t * 256 + y);
			}
			else {
				ApparentPower_ = -((255 - r) * 65536 + (255 - t) * 256 + (255 - y));
			}
		} // Gain Correction Selection

		inBuffer[bufIndx] = 0;
		bufIndx = 0;
	}
	else
	{
		ApparentPower_ = 0;
	}

	return abs(ApparentPower_);
}
float MAX78630::PowerFactor(char Phase) {

	MAX_Serial.flush(); while(MAX_Serial.available() > 0) char _t = MAX_Serial.read();

	float PowerFactor_ = 0;
	byte inBuffer[6];
	int bufIndx = 0;

	// PowerFactor Read
	// -------------------
	// PowerFactor Phase R = 0xAA,0x07,0xA3,0x65,0x01,0xE3,0x63
	// PowerFactor Phase S = 0xAA,0x07,0xA3,0x68,0x01,0xE3,0x60
	// PowerFactor Phase T = 0xAA,0x07,0xA3,0x6B,0x01,0xE3,0x5D

	MAX_Serial.print(char(170));	// (0xAA)
	MAX_Serial.print(char(7));		// (0x07)
	MAX_Serial.print(char(163));	// (0xA3)

	switch (Phase) {
		case 'R':
			MAX_Serial.print(char(101));    // (0x65)
			MAX_Serial.print(char(1));      // (0x01)
			MAX_Serial.print(char(227));    // (0xE3)
			MAX_Serial.print(char(99));     // (0x63)
			break;
		case 'S':
			MAX_Serial.print(char(104));    // (0x68)
			MAX_Serial.print(char(1));      // (0x01)
			MAX_Serial.print(char(227));    // (0xE3)
			MAX_Serial.print(char(96));     // (0x60)
			break;
		case 'T':
			MAX_Serial.print(char(107));    // (0x6B)
			MAX_Serial.print(char(1));      // (0x01)
			MAX_Serial.print(char(227));    // (0xE3)
			MAX_Serial.print(char(93));     // (0x5D)
			break;
		default:
			return 0;
	}

	delay(5);

	if (MAX_Serial.available()) {
		while (MAX_Serial.available()) {
			inBuffer[bufIndx] = MAX_Serial.read();
			bufIndx ++;
		}

		byte r = inBuffer[4];
		byte t = inBuffer[3];
		byte y = inBuffer[2];

		if (MAX78630_Gain == 1) {
			if (r < 128) {
				PowerFactor_ = (r * 65536 + t * 256 + y) * MAX78630_Power_Factor_Gain;
			}
			else {
				PowerFactor_ = -((255 - r) * 65536 + (255 - t) * 256 + (255 - y)) * MAX78630_Power_Factor_Gain;
			}
		}
		else {
			if (r < 128) {
				PowerFactor_ = (r * 65536 + t * 256 + y);
			}
			else {
				PowerFactor_ = -((255 - r) * 65536 + (255 - t) * 256 + (255 - y));
			}
		} // Gain Correction Selection

		inBuffer[bufIndx] = 0;
		bufIndx = 0;
	}
	else
	{
		PowerFactor_ = 0;
	}

	return abs(PowerFactor_);
}
float MAX78630::Frequency(void) {

	MAX_Serial.flush(); while(MAX_Serial.available() > 0) char _t = MAX_Serial.read();

	float Frequency_ = 0;
	byte inBuffer[6];
	int bufIndx = 0;

	// Frequency Read
	// --------------
	// Frequency = 0xAA,0x07,0xA3,0x80,0x01,0xE3,0x48

	MAX_Serial.print(char(170));	// (0xAA)
	MAX_Serial.print(char(7));		// (0x07)
	MAX_Serial.print(char(163));	// (0xA3)

	MAX_Serial.print(char(128));	// (0x80)
	MAX_Serial.print(char(1));		// (0x01)
	MAX_Serial.print(char(227));	// (0xE3)
	MAX_Serial.print(char(72));		// (0x48)

	delay(5);

	if (MAX_Serial.available()) {
		while (MAX_Serial.available()) {
			inBuffer[bufIndx] = MAX_Serial.read();
			bufIndx ++;
		}

		byte r = inBuffer[4];
		byte t = inBuffer[3];
		byte y = inBuffer[2];

		if (MAX78630_Gain == 1) {
			Frequency_ = (r * 65536 + t * 256 + y) * MAX78630_Frequency_Gain;
		}
		else {
			Frequency_ = (r * 65536 + t * 256 + y);
		} // Gain Correction Selection

		inBuffer[bufIndx] = 0;
		bufIndx = 0;
	}
	else
	{
		Frequency_ = 0;
	}

	return abs(Frequency_);
}