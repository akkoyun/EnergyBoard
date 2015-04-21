/* *******************************************************************************
 *
 *  Copyright (C) 2014-2015 X2Bus (info@x2bus.com)
 *  Can not be copied and/or distributed without the express permission of X2Bus
 *
 *  The library is compatable with Arduino and the Open Hardware Community.
 *
 *  Library	: X2Bus Energy Board (MAX78630)
 *  Developer	: Mehmet Gunce Akkoyun (gunce.akkoyun@x2bus.com)
 *  GitHub	: https://github.com/x2bus/EnergyBoard
 *  Revision	: 1.1.0
 *  Relase	: April 2015
 *
 *********************************************************************************/

#include "Arduino.h"
#include "MAX78630.h"

#define MAX_Serial Serial2	 // Define EnergyBoard Serial

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
    
    MAX_Serial.begin(Serial_BoudRate);
    ClearBuffer();
    MAX_Serial.write(0xAA);	// Header (0xAA)
    MAX_Serial.write(0x04);		// Total Sended Byte (0x04)
    MAX_Serial.write(0xC4);	// Setting Command (0xC4)
    MAX_Serial.write(0x8E);	// CheckSum (0x8E)
    MAX_Serial.end();
    
    if (Gain != 0) MAX78630_Gain = true;
}

float MAX78630::RMS_Voltage(char Phase) {
	
	MAX_Serial.begin(Serial_BoudRate);
	ClearBuffer();
	
	float Voltage_ = 0;
	
	// RMS Voltage Read
	// --------------------------------------------------------------------------------------------------
	if (Phase == 'R') SendCommand(0x90,0x00); // RMS Voltage Phase R = 0xAA,0x07,0xA3,0x90,0x00,0xE3,0x39
	if (Phase == 'S') SendCommand(0x93,0x00); // RMS Voltage Phase S = 0xAA,0x07,0xA3,0x93,0x00,0xE3,0x36
	if (Phase == 'T') SendCommand(0x96,0x00); // RMS Voltage Phase T = 0xAA,0x07,0xA3,0x96,0x00,0xE3,0x33
	// --------------------------------------------------------------------------------------------------
	
	if (MAX_Serial.available()) {
		
		byte Header					= MAX_Serial.read();
		byte Recieved_Byte_Count	= MAX_Serial.read();
		byte Data1					= MAX_Serial.read();
		byte Data2					= MAX_Serial.read();
		byte Data3					= MAX_Serial.read();
		byte CheckSum				= MAX_Serial.read();
		
		// Control Recieved Data
		if (Header == 0xAA) {
			Voltage_ = (Data3 * 65536 + Data2 * 256 + Data1);
		} // Acknowledge with data.
		if (Header == 0xAD) {
			Voltage_ = -1001;
		} // Acknowledge without data. (Error: -1001)
		if (Header == 0xB0) {
			Voltage_ = -1002;
		} // Negative Acknowledge (NACK). (Error: -1002)
		if (Header == 0xBC) {
			Voltage_ = -1003;
		} // Command not implemented. (Error: -1003)
		if (Header == 0xBD) {
			Voltage_ = -1004;
		} // Checksum failed. (Error: -1004)
		
		if (MAX78630_Gain) {
			Voltage_ = Voltage_ * MAX78630_Voltage_Gain;
		} // Gain Correction
	}
	
	MAX_Serial.end();
	return Voltage_;
}
float MAX78630::RMS_Voltage_AVR() {
	
	MAX_Serial.begin(Serial_BoudRate);
	ClearBuffer();
	
	float Voltage_ = 0;
	
	// RMS Voltage Avarage Read
	// --------------------------------------------------------------------------------
	SendCommand(0x99,0x00); // RMS Voltage Avarage = 0xAA,0x07,0xA3,0x99,0x00,0xE3,0x39
	// --------------------------------------------------------------------------------
	
	if (MAX_Serial.available()) {
		
		byte Header					= MAX_Serial.read();
		byte Recieved_Byte_Count	= MAX_Serial.read();
		byte Data1					= MAX_Serial.read();
		byte Data2					= MAX_Serial.read();
		byte Data3					= MAX_Serial.read();
		byte CheckSum				= MAX_Serial.read();
		
		// Control Recieved Data
		if (Header == 0xAA) {
			Voltage_ = (Data3 * 65536 + Data2 * 256 + Data1);
		} // Acknowledge with data.
		if (Header == 0xAD) {
			Voltage_ = -1001;
		} // Acknowledge without data. (Error: -1001)
		if (Header == 0xB0) {
			Voltage_ = -1002;
		} // Negative Acknowledge (NACK). (Error: -1002)
		if (Header == 0xBC) {
			Voltage_ = -1003;
		} // Command not implemented. (Error: -1003)
		if (Header == 0xBD) {
			Voltage_ = -1004;
		} // Checksum failed. (Error: -1004)
		
		if (MAX78630_Gain) {
			Voltage_ = Voltage_ * MAX78630_Voltage_Gain;
		} // Gain Correction
	}
	
	MAX_Serial.end();
	return Voltage_;
}
float MAX78630::INST_Voltage(char Phase) {
	
	MAX_Serial.begin(Serial_BoudRate);
	ClearBuffer();
	
	float Voltage_ = 0;
	
	// Instantaneous Voltage Read
	// ------------------------------------------------------------------------------------------------------------
	if (Phase == 'R') SendCommand(0x87,0x00); // Instantaneous Voltage Phase R = 0xAA,0x07,0xA3,0x87,0x00,0xE3,0x39
	if (Phase == 'S') SendCommand(0x8A,0x00); // Instantaneous Voltage Phase S = 0xAA,0x07,0xA3,0x8A,0x00,0xE3,0x36
	if (Phase == 'T') SendCommand(0x8D,0x00); // Instantaneous Voltage Phase T = 0xAA,0x07,0xA3,0x8D,0x00,0xE3,0x33
	// ------------------------------------------------------------------------------------------------------------
	
	if (MAX_Serial.available()) {
		
		byte Header					= MAX_Serial.read();
		byte Recieved_Byte_Count	= MAX_Serial.read();
		byte Data1					= MAX_Serial.read();
		byte Data2					= MAX_Serial.read();
		byte Data3					= MAX_Serial.read();
		byte CheckSum				= MAX_Serial.read();
		
		// Control Recieved Data
		if (Header == 0xAA) {
			Voltage_ = (Data3 * 65536 + Data2 * 256 + Data1);
		} // Acknowledge with data.
		if (Header == 0xAD) {
			Voltage_ = -1001;
		} // Acknowledge without data. (Error: -1001)
		if (Header == 0xB0) {
			Voltage_ = -1002;
		} // Negative Acknowledge (NACK). (Error: -1002)
		if (Header == 0xBC) {
			Voltage_ = -1003;
		} // Command not implemented. (Error: -1003)
		if (Header == 0xBD) {
			Voltage_ = -1004;
		} // Checksum failed. (Error: -1004)
		
		if (MAX78630_Gain) {
			Voltage_ = Voltage_ * MAX78630_Voltage_Gain;
		} // Gain Correction
	}
	
	MAX_Serial.end();
	return Voltage_;
}
float MAX78630::FUN_Voltage(char Phase) {
	
	MAX_Serial.begin(Serial_BoudRate);
	ClearBuffer();
	
	float Voltage_ = 0;
	
	// Fundamental Voltage Read
	// ----------------------------------------------------------------------------------------------------------
	if (Phase == 'R') SendCommand(0x9C,0x00); // Fundamental Voltage Phase R = 0xAA,0x07,0xA3,0x90,0x00,0xE3,0x39
	if (Phase == 'S') SendCommand(0x9F,0x00); // Fundamental Voltage Phase S = 0xAA,0x07,0xA3,0x93,0x00,0xE3,0x36
	if (Phase == 'T') SendCommand(0xA2,0x00); // Fundamental Voltage Phase T = 0xAA,0x07,0xA3,0x96,0x00,0xE3,0x33
	// ----------------------------------------------------------------------------------------------------------
	
	if (MAX_Serial.available()) {
		
		byte Header					= MAX_Serial.read();
		byte Recieved_Byte_Count	= MAX_Serial.read();
		byte Data1					= MAX_Serial.read();
		byte Data2					= MAX_Serial.read();
		byte Data3					= MAX_Serial.read();
		byte CheckSum				= MAX_Serial.read();
		
		// Control Recieved Data
		if (Header == 0xAA) {
			Voltage_ = (Data3 * 65536 + Data2 * 256 + Data1);
		} // Acknowledge with data.
		if (Header == 0xAD) {
			Voltage_ = -1001;
		} // Acknowledge without data. (Error: -1001)
		if (Header == 0xB0) {
			Voltage_ = -1002;
		} // Negative Acknowledge (NACK). (Error: -1002)
		if (Header == 0xBC) {
			Voltage_ = -1003;
		} // Command not implemented. (Error: -1003)
		if (Header == 0xBD) {
			Voltage_ = -1004;
		} // Checksum failed. (Error: -1004)
		
		if (MAX78630_Gain) {
			Voltage_ = Voltage_ * MAX78630_Voltage_Gain;
		} // Gain Correction
	}
	
	MAX_Serial.end();
	return Voltage_;
}
float MAX78630::HARM_Voltage(char Phase) {

	MAX_Serial.begin(Serial_BoudRate);
	ClearBuffer();
	
	float Voltage_ = 0;
	
	// Harmonic Voltage Read
	// ----------------------------------------------------------------------------------------------------------
	if (Phase == 'R') SendCommand(0xA5,0x00); // Harmonic Voltage Phase R = 0xAA,0x07,0xA3,0xA5,0x00,0xE3,0x39
	if (Phase == 'S') SendCommand(0xA8,0x00); // Harmonic Voltage Phase S = 0xAA,0x07,0xA3,0xA8,0x00,0xE3,0x36
	if (Phase == 'T') SendCommand(0xAB,0x00); // Harmonic Voltage Phase T = 0xAA,0x07,0xA3,0xAB,0x00,0xE3,0x33
	// ----------------------------------------------------------------------------------------------------------
	
	if (MAX_Serial.available()) {
		
		byte Header					= MAX_Serial.read();
		byte Recieved_Byte_Count	= MAX_Serial.read();
		byte Data1					= MAX_Serial.read();
		byte Data2					= MAX_Serial.read();
		byte Data3					= MAX_Serial.read();
		byte CheckSum				= MAX_Serial.read();
		
		// Control Recieved Data
		if (Header == 0xAA) {
			Voltage_ = (Data3 * 65536 + Data2 * 256 + Data1);
		} // Acknowledge with data.
		if (Header == 0xAD) {
			Voltage_ = -1001;
		} // Acknowledge without data. (Error: -1001)
		if (Header == 0xB0) {
			Voltage_ = -1002;
		} // Negative Acknowledge (NACK). (Error: -1002)
		if (Header == 0xBC) {
			Voltage_ = -1003;
		} // Command not implemented. (Error: -1003)
		if (Header == 0xBD) {
			Voltage_ = -1004;
		} // Checksum failed. (Error: -1004)
		
		if (MAX78630_Gain) {
			Voltage_ = Voltage_ * MAX78630_Voltage_Gain;
		} // Gain Correction
	}
	
	MAX_Serial.end();
	return Voltage_;
}
float MAX78630::RMS_Voltage_MIN() {
	
	MAX_Serial.begin(Serial_BoudRate);
	ClearBuffer();
	
	float Voltage_ = 0;
	
	// RMS Voltage MIN Limit Read
	// ----------------------------------------------------------------------------------
	SendCommand(0xB1,0x00); // RMS Voltage MIN Limit = 0xAA,0x07,0xA3,0xB1,0x00,0xE3,0x39
	// ----------------------------------------------------------------------------------
	
	if (MAX_Serial.available()) {
		
		byte Header					= MAX_Serial.read();
		byte Recieved_Byte_Count	= MAX_Serial.read();
		byte Data1					= MAX_Serial.read();
		byte Data2					= MAX_Serial.read();
		byte Data3					= MAX_Serial.read();
		byte CheckSum				= MAX_Serial.read();
		
		// Control Recieved Data
		if (Header == 0xAA) {
			Voltage_ = (Data3 * 65536 + Data2 * 256 + Data1);
		} // Acknowledge with data.
		if (Header == 0xAD) {
			Voltage_ = -1001;
		} // Acknowledge without data. (Error: -1001)
		if (Header == 0xB0) {
			Voltage_ = -1002;
		} // Negative Acknowledge (NACK). (Error: -1002)
		if (Header == 0xBC) {
			Voltage_ = -1003;
		} // Command not implemented. (Error: -1003)
		if (Header == 0xBD) {
			Voltage_ = -1004;
		} // Checksum failed. (Error: -1004)
		
		if (MAX78630_Gain) {
			Voltage_ = Voltage_ * MAX78630_Voltage_Gain;
		} // Gain Correction
	}
	
	MAX_Serial.end();
	return Voltage_;
}
float MAX78630::RMS_Voltage_MAX() {
	
	MAX_Serial.begin(Serial_BoudRate);
	ClearBuffer();
	
	float Voltage_ = 0;
	
	// RMS Voltage MAX Limit Read
	// ----------------------------------------------------------------------------------
	SendCommand(0xB4,0x00); // RMS Voltage MAX Limit = 0xAA,0x07,0xA3,0xB4,0x00,0xE3,0x39
	// ----------------------------------------------------------------------------------
	
	if (MAX_Serial.available()) {
		
		byte Header					= MAX_Serial.read();
		byte Recieved_Byte_Count	= MAX_Serial.read();
		byte Data1					= MAX_Serial.read();
		byte Data2					= MAX_Serial.read();
		byte Data3					= MAX_Serial.read();
		byte CheckSum				= MAX_Serial.read();
		
		// Control Recieved Data
		if (Header == 0xAA) {
			Voltage_ = (Data3 * 65536 + Data2 * 256 + Data1);
		} // Acknowledge with data.
		if (Header == 0xAD) {
			Voltage_ = -1001;
		} // Acknowledge without data. (Error: -1001)
		if (Header == 0xB0) {
			Voltage_ = -1002;
		} // Negative Acknowledge (NACK). (Error: -1002)
		if (Header == 0xBC) {
			Voltage_ = -1003;
		} // Command not implemented. (Error: -1003)
		if (Header == 0xBD) {
			Voltage_ = -1004;
		} // Checksum failed. (Error: -1004)
		
		if (MAX78630_Gain) {
			Voltage_ = Voltage_ * MAX78630_Voltage_Gain;
		} // Gain Correction
	}
	
	MAX_Serial.end();
	return Voltage_;
}
float MAX78630::RMS_Current(char Phase) {
	
	MAX_Serial.begin(Serial_BoudRate);
	ClearBuffer();
	
	float Current_ = 0;
	
	while (Current_ <= 0) {
	
		// RMS Current Read
		// --------------------------------------------------------------------------------------------------
		if (Phase == 'R') SendCommand(0xD5,0x00); // RMS Current Phase R = 0xAA,0x07,0xA3,0xD5,0x00,0xE3,0xF4
		if (Phase == 'S') SendCommand(0xD8,0x00); // RMS Current Phase S = 0xAA,0x07,0xA3,0xD8,0x00,0xE3,0xF1
		if (Phase == 'T') SendCommand(0xDB,0x00); // RMS Current Phase T = 0xAA,0x07,0xA3,0xDB,0x00,0xE3,0xEE
		// --------------------------------------------------------------------------------------------------

		if (MAX_Serial.available()) {

			byte Header					= MAX_Serial.read();
			byte Recieved_Byte_Count	= MAX_Serial.read();
			byte Data1					= MAX_Serial.read();
			byte Data2					= MAX_Serial.read();
			byte Data3					= MAX_Serial.read();
			byte CheckSum				= MAX_Serial.read();

			// Control Recieved Data
			if (Header == 0xAA) {
			Current_ = (Data3 * 65536 + Data2 * 256 + Data1);
		} // Acknowledge with data.
			if (Header == 0xAD) {
			Current_ = -1001;
		} // Acknowledge without data. (Error: -1001)
			if (Header == 0xB0) {
			Current_ = -1002;
		} // Negative Acknowledge (NACK). (Error: -1002)
			if (Header == 0xBC) {
			Current_ = -1003;
		} // Command not implemented. (Error: -1003)
			if (Header == 0xBD) {
			Current_ = -1004;
		} // Checksum failed. (Error: -1004)

			if (MAX78630_Gain == 1) {
			Current_ = Current_ * MAX78630_Current_Gain;
		} // Gain Correction
		}
	}
	
	MAX_Serial.end();
	return Current_;
}
float MAX78630::RMS_Current_AVR() {
	
	MAX_Serial.begin(Serial_BoudRate);
	ClearBuffer();
	
	float Current_ = 0;

	// RMS Current Avarage Read
	// --------------------------------------------------------------------------------
	SendCommand(0xDE,0x00); // RMS Current Avarage = 0xAA,0x07,0xA3,0xDE,0x00,0xE3,0xF4
	// --------------------------------------------------------------------------------
	
	if (MAX_Serial.available()) {
		
		byte Header					= MAX_Serial.read();
		byte Recieved_Byte_Count	= MAX_Serial.read();
		byte Data1					= MAX_Serial.read();
		byte Data2					= MAX_Serial.read();
		byte Data3					= MAX_Serial.read();
		byte CheckSum				= MAX_Serial.read();
		
		// Control Recieved Data
		if (Header == 0xAA) {
			Current_ = (Data3 * 65536 + Data2 * 256 + Data1);
		} // Acknowledge with data.
		if (Header == 0xAD) {
			Current_ = -1001;
		} // Acknowledge without data. (Error: -1001)
		if (Header == 0xB0) {
			Current_ = -1002;
		} // Negative Acknowledge (NACK). (Error: -1002)
		if (Header == 0xBC) {
			Current_ = -1003;
		} // Command not implemented. (Error: -1003)
		if (Header == 0xBD) {
			Current_ = -1004;
		} // Checksum failed. (Error: -1004)
		
		if (MAX78630_Gain == 1) {
			Current_ = Current_ * MAX78630_Current_Gain;
		} // Gain Correction
	}
	
	MAX_Serial.end();
	return Current_;
}
float MAX78630::PEAK_Current(char Phase) {
	
	MAX_Serial.begin(Serial_BoudRate);
	ClearBuffer();
	
	float Current_ = 0;
	
	// Peak Current Read
	// ---------------------------------------------------------------------------------------------------
	if (Phase == 'R') SendCommand(0xCC,0x00); // Peak Current Phase R = 0xAA,0x07,0xA3,0xCC,0x00,0xE3,0xF4
	if (Phase == 'S') SendCommand(0xCF,0x00); // Peak Current Phase S = 0xAA,0x07,0xA3,0xCF,0x00,0xE3,0xF1
	if (Phase == 'T') SendCommand(0xD2,0x00); // Peak Current Phase T = 0xAA,0x07,0xA3,0xD2,0x00,0xE3,0xEE
	// ---------------------------------------------------------------------------------------------------
	
	if (MAX_Serial.available()) {
		
		byte Header					= MAX_Serial.read();
		byte Recieved_Byte_Count	= MAX_Serial.read();
		byte Data1					= MAX_Serial.read();
		byte Data2					= MAX_Serial.read();
		byte Data3					= MAX_Serial.read();
		byte CheckSum				= MAX_Serial.read();
		
		// Control Recieved Data
		if (Header == 0xAA) {
			Current_ = (Data3 * 65536 + Data2 * 256 + Data1);
		} // Acknowledge with data.
		if (Header == 0xAD) {
			Current_ = -1001;
		} // Acknowledge without data. (Error: -1001)
		if (Header == 0xB0) {
			Current_ = -1002;
		} // Negative Acknowledge (NACK). (Error: -1002)
		if (Header == 0xBC) {
			Current_ = -1003;
		} // Command not implemented. (Error: -1003)
		if (Header == 0xBD) {
			Current_ = -1004;
		} // Checksum failed. (Error: -1004)
		
		if (MAX78630_Gain == 1) {
			Current_ = Current_ * MAX78630_Current_Gain;
		} // Gain Correction
	}
	
	MAX_Serial.end();
	return Current_;
}
float MAX78630::INST_Current(char Phase) {
	
	MAX_Serial.begin(Serial_BoudRate);
	ClearBuffer();
	
	float Current_ = 0;
	
	// Instantaneous Current Read
	// ------------------------------------------------------------------------------------------------------------
	if (Phase == 'R') SendCommand(0xBA,0x00); // Instantaneous Current Phase R = 0xAA,0x07,0xA3,0xBA,0x00,0xE3,0xF4
	if (Phase == 'S') SendCommand(0xBD,0x00); // Instantaneous Current Phase S = 0xAA,0x07,0xA3,0xBD,0x00,0xE3,0xF1
	if (Phase == 'T') SendCommand(0xC0,0x00); // Instantaneous Current Phase T = 0xAA,0x07,0xA3,0xC0,0x00,0xE3,0xEE
	// ------------------------------------------------------------------------------------------------------------
	
	if (MAX_Serial.available()) {
		
		byte Header					= MAX_Serial.read();
		byte Recieved_Byte_Count	= MAX_Serial.read();
		byte Data1					= MAX_Serial.read();
		byte Data2					= MAX_Serial.read();
		byte Data3					= MAX_Serial.read();
		byte CheckSum				= MAX_Serial.read();
		
		// Control Recieved Data
		if (Header == 0xAA) {
			Current_ = (Data3 * 65536 + Data2 * 256 + Data1);
		} // Acknowledge with data.
		if (Header == 0xAD) {
			Current_ = -1001;
		} // Acknowledge without data. (Error: -1001)
		if (Header == 0xB0) {
			Current_ = -1002;
		} // Negative Acknowledge (NACK). (Error: -1002)
		if (Header == 0xBC) {
			Current_ = -1003;
		} // Command not implemented. (Error: -1003)
		if (Header == 0xBD) {
			Current_ = -1004;
		} // Checksum failed. (Error: -1004)
		
		if (MAX78630_Gain == 1) {
			Current_ = Current_ * MAX78630_Current_Gain;
		} // Gain Correction
	}
	
	MAX_Serial.end();
	return Current_;
}
float MAX78630::FUN_Current(char Phase) {
	
	MAX_Serial.begin(Serial_BoudRate);
	ClearBuffer();
	
	float Current_ = 0;
	
	// Fundamental Current Read
	// ----------------------------------------------------------------------------------------------------------
	if (Phase == 'R') SendCommand(0xE1,0x00); // Fundamental Current Phase R = 0xAA,0x07,0xA3,0xE1,0x00,0xE3,0xF4
	if (Phase == 'S') SendCommand(0xE4,0x00); // Fundamental Current Phase S = 0xAA,0x07,0xA3,0xE4,0x00,0xE3,0xF1
	if (Phase == 'T') SendCommand(0xE7,0x00); // Fundamental Current Phase T = 0xAA,0x07,0xA3,0xE7,0x00,0xE3,0xEE
	// ----------------------------------------------------------------------------------------------------------
	
	if (MAX_Serial.available()) {
		
		byte Header					= MAX_Serial.read();
		byte Recieved_Byte_Count	= MAX_Serial.read();
		byte Data1					= MAX_Serial.read();
		byte Data2					= MAX_Serial.read();
		byte Data3					= MAX_Serial.read();
		byte CheckSum				= MAX_Serial.read();
		
		// Control Recieved Data
		if (Header == 0xAA) {
			Current_ = (Data3 * 65536 + Data2 * 256 + Data1);
		} // Acknowledge with data.
		if (Header == 0xAD) {
			Current_ = -1001;
		} // Acknowledge without data. (Error: -1001)
		if (Header == 0xB0) {
			Current_ = -1002;
		} // Negative Acknowledge (NACK). (Error: -1002)
		if (Header == 0xBC) {
			Current_ = -1003;
		} // Command not implemented. (Error: -1003)
		if (Header == 0xBD) {
			Current_ = -1004;
		} // Checksum failed. (Error: -1004)
		
		if (MAX78630_Gain == 1) {
			Current_ = Current_ * MAX78630_Current_Gain;
		} // Gain Correction
	}
	
	MAX_Serial.end();
	return Current_;
}
float MAX78630::HARM_Current(char Phase) {
	
	MAX_Serial.begin(Serial_BoudRate);
	ClearBuffer();
	
	float Current_ = 0;
	
	// Harmonic Current Read
	// -------------------------------------------------------------------------------------------------------
	if (Phase == 'R') SendCommand(0xEA,0x00); // Harmonic Current Phase R = 0xAA,0x07,0xA3,0xEA,0x00,0xE3,0xF4
	if (Phase == 'S') SendCommand(0xED,0x00); // Harmonic Current Phase S = 0xAA,0x07,0xA3,0xED,0x00,0xE3,0xF1
	if (Phase == 'T') SendCommand(0xF0,0x00); // Harmonic Current Phase T = 0xAA,0x07,0xA3,0xF0,0x00,0xE3,0xEE
	// -------------------------------------------------------------------------------------------------------
	
	if (MAX_Serial.available()) {
		
		byte Header					= MAX_Serial.read();
		byte Recieved_Byte_Count	= MAX_Serial.read();
		byte Data1					= MAX_Serial.read();
		byte Data2					= MAX_Serial.read();
		byte Data3					= MAX_Serial.read();
		byte CheckSum				= MAX_Serial.read();
		
		// Control Recieved Data
		if (Header == 0xAA) {
			Current_ = (Data3 * 65536 + Data2 * 256 + Data1);
		} // Acknowledge with data.
		if (Header == 0xAD) {
			Current_ = -1001;
		} // Acknowledge without data. (Error: -1001)
		if (Header == 0xB0) {
			Current_ = -1002;
		} // Negative Acknowledge (NACK). (Error: -1002)
		if (Header == 0xBC) {
			Current_ = -1003;
		} // Command not implemented. (Error: -1003)
		if (Header == 0xBD) {
			Current_ = -1004;
		} // Checksum failed. (Error: -1004)
		
		if (MAX78630_Gain == 1) {
			Current_ = Current_ * MAX78630_Current_Gain;
		} // Gain Correction
	}
	
	MAX_Serial.end();
	return Current_;
}
float MAX78630::RMS_Current_MAX() {

	MAX_Serial.begin(Serial_BoudRate);
	ClearBuffer();
	
	float Current_ = 0;
	
	// RMS Current Max Limit Read
	// ----------------------------------------------------------------------------------
	SendCommand(0xF3,0x00); // RMS Current Max Limit = 0xAA,0x07,0xA3,0xF3,0x00,0xE3,0xF4
	// ----------------------------------------------------------------------------------
	
	if (MAX_Serial.available()) {
		
		byte Header					= MAX_Serial.read();
		byte Recieved_Byte_Count	= MAX_Serial.read();
		byte Data1					= MAX_Serial.read();
		byte Data2					= MAX_Serial.read();
		byte Data3					= MAX_Serial.read();
		byte CheckSum				= MAX_Serial.read();
		
		// Control Recieved Data
		if (Header == 0xAA) {
			Current_ = (Data3 * 65536 + Data2 * 256 + Data1);
		} // Acknowledge with data.
		if (Header == 0xAD) {
			Current_ = -1001;
		} // Acknowledge without data. (Error: -1001)
		if (Header == 0xB0) {
			Current_ = -1002;
		} // Negative Acknowledge (NACK). (Error: -1002)
		if (Header == 0xBC) {
			Current_ = -1003;
		} // Command not implemented. (Error: -1003)
		if (Header == 0xBD) {
			Current_ = -1004;
		} // Checksum failed. (Error: -1004)
		
		if (MAX78630_Gain == 1) {
			Current_ = Current_ * MAX78630_Current_Gain;
		} // Gain Correction
	}
	
	MAX_Serial.end();
	return Current_;
}
float MAX78630::ActivePower(char Phase) {

	MAX_Serial.begin(Serial_BoudRate);
	ClearBuffer();

	float ActivePower_ = 0;

	// Active Power Read
	// -------------------------------------------------------------------------------------------------------
	if (Phase == 'R') SendCommand(0x1D,0x01); // Active Power Phase R = 0xAA,0x07,0xA3,0x1D,0x01,0xE3,0xF4
	if (Phase == 'S') SendCommand(0x20,0x01); // Active Power Phase S = 0xAA,0x07,0xA3,0x20,0x01,0xE3,0xF1
	if (Phase == 'T') SendCommand(0x23,0x01); // Active Power Phase T = 0xAA,0x07,0xA3,0x23,0x01,0xE3,0xEE
	// -------------------------------------------------------------------------------------------------------

	if (MAX_Serial.available()) {
		
		byte Header					= MAX_Serial.read();
		byte Recieved_Byte_Count	= MAX_Serial.read();
		byte Data1					= MAX_Serial.read();
		byte Data2					= MAX_Serial.read();
		byte Data3					= MAX_Serial.read();
		byte CheckSum				= MAX_Serial.read();
		
		// Control Recieved Data
		if (Header == 0xAA) {
			if (Data3 < 128) {
				ActivePower_ = (Data3 * 65536 + Data2 * 256 + Data1);
			}
			else {
				ActivePower_ = -((255 - Data3) * 65536 + (255 - Data2) * 256 + (255 - Data1));
			}
		} // Acknowledge with data.
		if (Header == 0xAD) {
			ActivePower_ = -1001;
		} // Acknowledge without data. (Error: -1001)
		if (Header == 0xB0) {
			ActivePower_ = -1002;
		} // Negative Acknowledge (NACK). (Error: -1002)
		if (Header == 0xBC) {
			ActivePower_ = -1003;
		} // Command not implemented. (Error: -1003)
		if (Header == 0xBD) {
			ActivePower_ = -1004;
		} // Checksum failed. (Error: -1004)
		
		if (MAX78630_Gain == 1) {
			ActivePower_ = ActivePower_ * MAX78630_Power_Gain;
		} // Gain Correction
	}

	MAX_Serial.end();
	return ActivePower_;
}
float MAX78630::ActivePower_AVR() {

	MAX_Serial.begin(Serial_BoudRate);
	ClearBuffer();
	
	float ActivePower_ = 0;
	
	// Active Power Avarage Read
	// ---------------------------------------------------------------------------------
	SendCommand(0x38,0x01); // Active Power Avarage = 0xAA,0x07,0xA3,0x38,0x01,0xE3,0xF4
	// ---------------------------------------------------------------------------------
	
	if (MAX_Serial.available()) {
		
		byte Header					= MAX_Serial.read();
		byte Recieved_Byte_Count	= MAX_Serial.read();
		byte Data1					= MAX_Serial.read();
		byte Data2					= MAX_Serial.read();
		byte Data3					= MAX_Serial.read();
		byte CheckSum				= MAX_Serial.read();
		
		// Control Recieved Data
		if (Header == 0xAA) {
			if (Data3 < 128) {
				ActivePower_ = (Data3 * 65536 + Data2 * 256 + Data1);
			}
			else {
				ActivePower_ = -((255 - Data3) * 65536 + (255 - Data2) * 256 + (255 - Data1));
			}
		} // Acknowledge with data.
		if (Header == 0xAD) {
			ActivePower_ = -1001;
		} // Acknowledge without data. (Error: -1001)
		if (Header == 0xB0) {
			ActivePower_ = -1002;
		} // Negative Acknowledge (NACK). (Error: -1002)
		if (Header == 0xBC) {
			ActivePower_ = -1003;
		} // Command not implemented. (Error: -1003)
		if (Header == 0xBD) {
			ActivePower_ = -1004;
		} // Checksum failed. (Error: -1004)
		
		if (MAX78630_Gain == 1) {
			ActivePower_ = ActivePower_ * MAX78630_Power_Gain;
		} // Gain Correction
	}
	
	MAX_Serial.end();
	return ActivePower_;
}
float MAX78630::ReActivePower(char Phase) {
	
	MAX_Serial.begin(Serial_BoudRate);
	ClearBuffer();
	
	float ReActivePower_ = 0;
	
	// ReActive Power Read
	// -----------------------------------------------------------------------------------------------------
	if (Phase == 'R') SendCommand(0x26,0x01); // ReActive Power Phase R = 0xAA,0x07,0xA3,0x26,0x01,0xE3,0xF4
	if (Phase == 'S') SendCommand(0x29,0x01); // ReActive Power Phase S = 0xAA,0x07,0xA3,0x29,0x01,0xE3,0xF1
	if (Phase == 'T') SendCommand(0x2C,0x01); // ReActive Power Phase T = 0xAA,0x07,0xA3,0x2C,0x01,0xE3,0xEE
	// -----------------------------------------------------------------------------------------------------
	
	if (MAX_Serial.available()) {
		
		byte Header					= MAX_Serial.read();
		byte Recieved_Byte_Count	= MAX_Serial.read();
		byte Data1					= MAX_Serial.read();
		byte Data2					= MAX_Serial.read();
		byte Data3					= MAX_Serial.read();
		byte CheckSum				= MAX_Serial.read();
		
		// Control Recieved Data
		if (Header == 0xAA) {
			if (Data3 < 128) {
				ReActivePower_ = (Data3 * 65536 + Data2 * 256 + Data1);
			}
			else {
				ReActivePower_ = -((255 - Data3) * 65536 + (255 - Data2) * 256 + (255 - Data1));
			}
		} // Acknowledge with data.
		if (Header == 0xAD) {
			ReActivePower_ = -1001;
		} // Acknowledge without data. (Error: -1001)
		if (Header == 0xB0) {
			ReActivePower_ = -1002;
		} // Negative Acknowledge (NACK). (Error: -1002)
		if (Header == 0xBC) {
			ReActivePower_ = -1003;
		} // Command not implemented. (Error: -1003)
		if (Header == 0xBD) {
			ReActivePower_ = -1004;
		} // Checksum failed. (Error: -1004)
		
		if (MAX78630_Gain == 1) {
			ReActivePower_ = ReActivePower_ * MAX78630_Power_Gain;
		} // Gain Correction
	}
	
	MAX_Serial.end();
	if (ReActivePower_ < 0) ReActivePower_ = ReActivePower_ * -1;
	return ReActivePower_;
}
float MAX78630::ReActivePower_AVR() {
	
	MAX_Serial.begin(Serial_BoudRate);
	ClearBuffer();
	
	float ReActivePower_ = 0;
	
	// ReActive Power Avarage Read
	// ---------------------------------------------------------------------------------
	SendCommand(0x3B,0x01); // ReActive Power Avarage = 0xAA,0x07,0xA3,0x38,0x01,0xE3,0xF4
	// ---------------------------------------------------------------------------------
	
	if (MAX_Serial.available()) {
		
		byte Header					= MAX_Serial.read();
		byte Recieved_Byte_Count	= MAX_Serial.read();
		byte Data1					= MAX_Serial.read();
		byte Data2					= MAX_Serial.read();
		byte Data3					= MAX_Serial.read();
		byte CheckSum				= MAX_Serial.read();
		
		// Control Recieved Data
		if (Header == 0xAA) {
			if (Data3 < 128) {
				ReActivePower_ = (Data3 * 65536 + Data2 * 256 + Data1);
			}
			else {
				ReActivePower_ = -((255 - Data3) * 65536 + (255 - Data2) * 256 + (255 - Data1));
			}
		} // Acknowledge with data.
		if (Header == 0xAD) {
			ReActivePower_ = -1001;
		} // Acknowledge without data. (Error: -1001)
		if (Header == 0xB0) {
			ReActivePower_ = -1002;
		} // Negative Acknowledge (NACK). (Error: -1002)
		if (Header == 0xBC) {
			ReActivePower_ = -1003;
		} // Command not implemented. (Error: -1003)
		if (Header == 0xBD) {
			ReActivePower_ = -1004;
		} // Checksum failed. (Error: -1004)
		
		if (MAX78630_Gain == 1) {
			ReActivePower_ = ReActivePower_ * MAX78630_Power_Gain;
		} // Gain Correction
	}
	
	MAX_Serial.end();
	if (ReActivePower_ < 0) ReActivePower_ = ReActivePower_ * -1;
	return ReActivePower_;
}
float MAX78630::ApparentPower(char Phase) {
	
	MAX_Serial.begin(Serial_BoudRate);
	ClearBuffer();
	
	float ApparentPower_ = 0;
	
	// Apparent Power Read
	// -----------------------------------------------------------------------------------------------------
	if (Phase == 'R') SendCommand(0x2F,0x01); // Apparent Power Phase R = 0xAA,0x07,0xA3,0x2F,0x01,0xE3,0xF4
	if (Phase == 'S') SendCommand(0x32,0x01); // Apparent Power Phase S = 0xAA,0x07,0xA3,0x32,0x01,0xE3,0xF1
	if (Phase == 'T') SendCommand(0x35,0x01); // Apparent Power Phase T = 0xAA,0x07,0xA3,0x35,0x01,0xE3,0xEE
	// -----------------------------------------------------------------------------------------------------
	
	if (MAX_Serial.available()) {
		
		byte Header					= MAX_Serial.read();
		byte Recieved_Byte_Count	= MAX_Serial.read();
		byte Data1					= MAX_Serial.read();
		byte Data2					= MAX_Serial.read();
		byte Data3					= MAX_Serial.read();
		byte CheckSum				= MAX_Serial.read();
		
		// Control Recieved Data
		if (Header == 0xAA) {
			if (Data3 < 128) {
				ApparentPower_ = (Data3 * 65536 + Data2 * 256 + Data1);
			}
			else {
				ApparentPower_ = -((255 - Data3) * 65536 + (255 - Data2) * 256 + (255 - Data1));
			}
		} // Acknowledge with data.
		if (Header == 0xAD) {
			ApparentPower_ = -1001;
		} // Acknowledge without data. (Error: -1001)
		if (Header == 0xB0) {
			ApparentPower_ = -1002;
		} // Negative Acknowledge (NACK). (Error: -1002)
		if (Header == 0xBC) {
			ApparentPower_ = -1003;
		} // Command not implemented. (Error: -1003)
		if (Header == 0xBD) {
			ApparentPower_ = -1004;
		} // Checksum failed. (Error: -1004)
		
		if (MAX78630_Gain == 1) {
			ApparentPower_ = ApparentPower_ * MAX78630_Power_Gain;
		} // Gain Correction
	}
	
	MAX_Serial.end();
	if (ApparentPower_ < 0) ApparentPower_ = ApparentPower_ * -1;
	return ApparentPower_;
}
float MAX78630::ApparentPower_AVR() {
	
	MAX_Serial.begin(Serial_BoudRate);
	ClearBuffer();
	
	float ApparentPower_ = 0;
	
	// Apparent Power Avarage Read
	// ---------------------------------------------------------------------------------
	SendCommand(0x3E,0x01); // Apparent Power Avarage = 0xAA,0x07,0xA3,0x3E,0x01,0xE3,0xF4
	// ---------------------------------------------------------------------------------
	
	if (MAX_Serial.available()) {
		
		byte Header					= MAX_Serial.read();
		byte Recieved_Byte_Count	= MAX_Serial.read();
		byte Data1					= MAX_Serial.read();
		byte Data2					= MAX_Serial.read();
		byte Data3					= MAX_Serial.read();
		byte CheckSum				= MAX_Serial.read();
		
		// Control Recieved Data
		if (Header == 0xAA) {
			if (Data3 < 128) {
				ApparentPower_ = (Data3 * 65536 + Data2 * 256 + Data1);
			}
			else {
				ApparentPower_ = -((255 - Data3) * 65536 + (255 - Data2) * 256 + (255 - Data1));
			}
		} // Acknowledge with data.
		if (Header == 0xAD) {
			ApparentPower_ = -1001;
		} // Acknowledge without data. (Error: -1001)
		if (Header == 0xB0) {
			ApparentPower_ = -1002;
		} // Negative Acknowledge (NACK). (Error: -1002)
		if (Header == 0xBC) {
			ApparentPower_ = -1003;
		} // Command not implemented. (Error: -1003)
		if (Header == 0xBD) {
			ApparentPower_ = -1004;
		} // Checksum failed. (Error: -1004)
		
		if (MAX78630_Gain == 1) {
			ApparentPower_ = ApparentPower_ * MAX78630_Power_Gain;
		} // Gain Correction
	}
	
	MAX_Serial.end();
	if (ApparentPower_ < 0) ApparentPower_ = ApparentPower_ * -1;
	return ApparentPower_;
}
float MAX78630::FundamentalPower(char Phase) {
	
	MAX_Serial.begin(Serial_BoudRate);
	ClearBuffer();
	
	float FundamentalPower_ = 0;
	
	// Fundamental Power Read
	// -----------------------------------------------------------------------------------------------------
	if (Phase == 'R') SendCommand(0x4A,0x01); // Fundamental Power Phase R = 0xAA,0x07,0xA3,0x2F,0x01,0xE3,0xF4
	if (Phase == 'S') SendCommand(0x4D,0x01); // Fundamental Power Phase S = 0xAA,0x07,0xA3,0x32,0x01,0xE3,0xF1
	if (Phase == 'T') SendCommand(0x50,0x01); // Fundamental Power Phase T = 0xAA,0x07,0xA3,0x35,0x01,0xE3,0xEE
	// -----------------------------------------------------------------------------------------------------
	
	if (MAX_Serial.available()) {
		
		byte Header					= MAX_Serial.read();
		byte Recieved_Byte_Count	= MAX_Serial.read();
		byte Data1					= MAX_Serial.read();
		byte Data2					= MAX_Serial.read();
		byte Data3					= MAX_Serial.read();
		byte CheckSum				= MAX_Serial.read();
		
		// Control Recieved Data
		if (Header == 0xAA) {
			if (Data3 < 128) {
				FundamentalPower_ = (Data3 * 65536 + Data2 * 256 + Data1);
			}
			else {
				FundamentalPower_ = -((255 - Data3) * 65536 + (255 - Data2) * 256 + (255 - Data1));
			}
		} // Acknowledge with data.
		if (Header == 0xAD) {
			FundamentalPower_ = -1001;
		} // Acknowledge without data. (Error: -1001)
		if (Header == 0xB0) {
			FundamentalPower_ = -1002;
		} // Negative Acknowledge (NACK). (Error: -1002)
		if (Header == 0xBC) {
			FundamentalPower_ = -1003;
		} // Command not implemented. (Error: -1003)
		if (Header == 0xBD) {
			FundamentalPower_ = -1004;
		} // Checksum failed. (Error: -1004)
		
		if (MAX78630_Gain == 1) {
			FundamentalPower_ = FundamentalPower_ * MAX78630_Power_Gain;
		} // Gain Correction
	}
	
	MAX_Serial.end();
	if (FundamentalPower_ < 0) FundamentalPower_ = FundamentalPower_ * -1;
	return FundamentalPower_;
}
float MAX78630::HarmonicPower(char Phase) {
	
	MAX_Serial.begin(Serial_BoudRate);
	ClearBuffer();
	
	float HarmonicPower_ = 0;
	
	// Harmonic Power Read
	// -----------------------------------------------------------------------------------------------------
	if (Phase == 'R') SendCommand(0x53,0x01); // Harmonic Power Phase R = 0xAA,0x07,0xA3,0x53,0x01,0xE3,0xF4
	if (Phase == 'S') SendCommand(0x56,0x01); // Harmonic Power Phase S = 0xAA,0x07,0xA3,0x56,0x01,0xE3,0xF1
	if (Phase == 'T') SendCommand(0x59,0x01); // Harmonic Power Phase T = 0xAA,0x07,0xA3,0x59,0x01,0xE3,0xEE
	// -----------------------------------------------------------------------------------------------------
	
	if (MAX_Serial.available()) {
		
		byte Header					= MAX_Serial.read();
		byte Recieved_Byte_Count	= MAX_Serial.read();
		byte Data1					= MAX_Serial.read();
		byte Data2					= MAX_Serial.read();
		byte Data3					= MAX_Serial.read();
		byte CheckSum				= MAX_Serial.read();
		
		// Control Recieved Data
		if (Header == 0xAA) {
			if (Data3 < 128) {
				HarmonicPower_ = (Data3 * 65536 + Data2 * 256 + Data1);
			}
			else {
				HarmonicPower_ = -((255 - Data3) * 65536 + (255 - Data2) * 256 + (255 - Data1));
			}
		} // Acknowledge with data.
		if (Header == 0xAD) {
			HarmonicPower_ = -1001;
		} // Acknowledge without data. (Error: -1001)
		if (Header == 0xB0) {
			HarmonicPower_ = -1002;
		} // Negative Acknowledge (NACK). (Error: -1002)
		if (Header == 0xBC) {
			HarmonicPower_ = -1003;
		} // Command not implemented. (Error: -1003)
		if (Header == 0xBD) {
			HarmonicPower_ = -1004;
		} // Checksum failed. (Error: -1004)
		
		if (MAX78630_Gain == 1) {
			HarmonicPower_ = HarmonicPower_ * MAX78630_Power_Gain;
		} // Gain Correction
	}
	
	MAX_Serial.end();
	if (HarmonicPower_ < 0) HarmonicPower_ = HarmonicPower_ * -1;
	return HarmonicPower_;
}
float MAX78630::FundamentalVA(char Phase) {
	
	MAX_Serial.begin(Serial_BoudRate);
	ClearBuffer();
	
	float FundamentalVA_ = 0;
	
	// Fundamental Volt Amper Read
	// -----------------------------------------------------------------------------------------------------
	if (Phase == 'R') SendCommand(0x5C,0x01); // Fundamental VA Phase R = 0xAA,0x07,0xA3,0x53,0x01,0xE3,0xF4
	if (Phase == 'S') SendCommand(0x5F,0x01); // Fundamental VA Phase S = 0xAA,0x07,0xA3,0x56,0x01,0xE3,0xF1
	if (Phase == 'T') SendCommand(0x62,0x01); // Fundamental VA Phase T = 0xAA,0x07,0xA3,0x59,0x01,0xE3,0xEE
	// -----------------------------------------------------------------------------------------------------
	
	if (MAX_Serial.available()) {
		
		byte Header					= MAX_Serial.read();
		byte Recieved_Byte_Count	= MAX_Serial.read();
		byte Data1					= MAX_Serial.read();
		byte Data2					= MAX_Serial.read();
		byte Data3					= MAX_Serial.read();
		byte CheckSum				= MAX_Serial.read();
		
		// Control Recieved Data
		if (Header == 0xAA) {
			if (Data3 < 128) {
				FundamentalVA_ = (Data3 * 65536 + Data2 * 256 + Data1);
			}
			else {
				FundamentalVA_ = -((255 - Data3) * 65536 + (255 - Data2) * 256 + (255 - Data1));
			}
		} // Acknowledge with data.
		if (Header == 0xAD) {
			FundamentalVA_ = -1001;
		} // Acknowledge without data. (Error: -1001)
		if (Header == 0xB0) {
			FundamentalVA_ = -1002;
		} // Negative Acknowledge (NACK). (Error: -1002)
		if (Header == 0xBC) {
			FundamentalVA_ = -1003;
		} // Command not implemented. (Error: -1003)
		if (Header == 0xBD) {
			FundamentalVA_ = -1004;
		} // Checksum failed. (Error: -1004)
		
		if (MAX78630_Gain == 1) {
			FundamentalVA_ = FundamentalVA_ * MAX78630_Power_Gain;
		} // Gain Correction
	}
	
	MAX_Serial.end();
	if (FundamentalVA_ < 0) FundamentalVA_ = FundamentalVA_ * -1;
	return FundamentalVA_;
}
float MAX78630::PowerFactor(char Phase) {
	
	MAX_Serial.begin(Serial_BoudRate);
	ClearBuffer();
	
	float PowerFactor_ = 0;
	
	// PowerFactor Read
	// --------------------------------------------------------------------------------------------------
	if (Phase == 'R') SendCommand(0x65,0x01); // PowerFactor Phase R = 0xAA,0x07,0xA3,0x65,0x01,0xE3,0x63
	if (Phase == 'S') SendCommand(0x68,0x01); // PowerFactor Phase S = 0xAA,0x07,0xA3,0x68,0x01,0xE3,0x60
	if (Phase == 'T') SendCommand(0x6B,0x01); // PowerFactor Phase T = 0xAA,0x07,0xA3,0x6B,0x01,0xE3,0x5D
	// --------------------------------------------------------------------------------------------------

	if (MAX_Serial.available()) {

		byte Header					= MAX_Serial.read();
		byte Recieved_Byte_Count	= MAX_Serial.read();
		byte Data1					= MAX_Serial.read();
		byte Data2					= MAX_Serial.read();
		byte Data3					= MAX_Serial.read();
		byte CheckSum				= MAX_Serial.read();

		// Control Recieved Data
		if (Header == 170) {
			if (Data3 < 128) {
				PowerFactor_ = (Data3 * 65536 + Data2 * 256 + Data1);
			}
			else {
				PowerFactor_ = -((255 - Data3) * 65536 + (255 - Data2) * 256 + (255 - Data1));
			}
		} // Acknowledge with data.
		if (Header == 173) {
			PowerFactor_ = -1001;
		} // Acknowledge without data.
		if (Header == 176) {
			PowerFactor_ = -1002;
		} // Negative Acknowledge (NACK).
		if (Header == 188) {
			PowerFactor_ = -1003;
		} // Command not implemented.
		if (Header == 189) {
			PowerFactor_ = -1004;
		} // Checksum failed.

		if (MAX78630_Gain == 1) {
			PowerFactor_ = PowerFactor_ * MAX78630_Power_Factor_Gain;
		} // Gain Correction
	}
	
	MAX_Serial.end();
	return PowerFactor_;
}
float MAX78630::PowerFactor_AVR() {
	
	MAX_Serial.begin(Serial_BoudRate);
	ClearBuffer();
	
	float PowerFactor_ = 0;
	
	// PowerFactor Avarage Read
	// --------------------------------------------------------------------------------
	SendCommand(0x6E,0x01); // PowerFactor Avarage = 0xAA,0x07,0xA3,0x65,0x01,0xE3,0x63
	// --------------------------------------------------------------------------------
	
	if (MAX_Serial.available()) {
		
		byte Header					= MAX_Serial.read();
		byte Recieved_Byte_Count	= MAX_Serial.read();
		byte Data1					= MAX_Serial.read();
		byte Data2					= MAX_Serial.read();
		byte Data3					= MAX_Serial.read();
		byte CheckSum				= MAX_Serial.read();
		
		// Control Recieved Data
		if (Header == 170) {
			if (Data3 < 128) {
				PowerFactor_ = (Data3 * 65536 + Data2 * 256 + Data1);
			}
			else {
				PowerFactor_ = -((255 - Data3) * 65536 + (255 - Data2) * 256 + (255 - Data1));
			}
		} // Acknowledge with data.
		if (Header == 173) {
			PowerFactor_ = -1001;
		} // Acknowledge without data.
		if (Header == 176) {
			PowerFactor_ = -1002;
		} // Negative Acknowledge (NACK).
		if (Header == 188) {
			PowerFactor_ = -1003;
		} // Command not implemented.
		if (Header == 189) {
			PowerFactor_ = -1004;
		} // Checksum failed.
		
		if (MAX78630_Gain == 1) {
			PowerFactor_ = PowerFactor_ * MAX78630_Power_Factor_Gain;
		} // Gain Correction
	}
	
	MAX_Serial.end();
	return PowerFactor_;
}
float MAX78630::Frequency(void) {
	
    MAX_Serial.begin(Serial_BoudRate);
	ClearBuffer();
	
    float Frequency_ = 0;
	
	// Frequency Read
	// ----------------------------------------------------------------------
	SendCommand(0x80,0x01); // Frequency = 0xAA,0x07,0xA3,0x80,0x01,0xE3,0x48
	// ----------------------------------------------------------------------

	if (MAX_Serial.available()) {

		byte Header					= MAX_Serial.read();
		byte Recieved_Byte_Count	= MAX_Serial.read();
		byte Data1					= MAX_Serial.read();
		byte Data2					= MAX_Serial.read();
		byte Data3					= MAX_Serial.read();
		byte CheckSum				= MAX_Serial.read();

		// Control Recieved Data
		if (Header == 170) {
                Frequency_ = (Data3 * 65536 + Data2 * 256 + Data1);
            } // Acknowledge with data.
		if (Header == 173) {
                delay(10);
            } // Acknowledge without data.
		if (Header == 176) {
                delay(10);
            } // Negative Acknowledge (NACK).
		if (Header == 188) {
                delay(10);
            } // Command not implemented.
		if (Header == 189) {
                delay(10);
            } // Checksum failed.
            
		if (MAX78630_Gain == 1) {
                Frequency_ = Frequency_ * MAX78630_Frequency_Gain;
            } // Gain Correction
	}
	
    MAX_Serial.end();
    return Frequency_;
}
float MAX78630::Temperature(void) {
    
    MAX_Serial.begin(Serial_BoudRate);
    delay(10);
    
    float Temperature_ = 0;
    
    if (ClearBuffer() == true) {
        SendCommand(116,1);
        
        if (MAX_Serial.available()) {
            
            byte Header					= MAX_Serial.read();
            byte Recieved_Byte_Count	= MAX_Serial.read();
            byte Data1					= MAX_Serial.read();
            byte Data2					= MAX_Serial.read();
            byte Data3					= MAX_Serial.read();
            byte CheckSum				= MAX_Serial.read();
            
            // Control Recieved Data
            if (Header == 170) {
                Temperature_ = (Data3 * 65536 + Data2 * 256 + Data1);
            } // Acknowledge with data.
            if (Header == 173) {
                delay(10);
            } // Acknowledge without data.
            if (Header == 176) {
                delay(10);
            } // Negative Acknowledge (NACK).
            if (Header == 188) {
                delay(10);
            } // Command not implemented.
            if (Header == 189) {
                delay(10);
            } // Checksum failed.
            
            if (MAX78630_Gain == 1) {
                Temperature_ = Temperature_ / 1000;
            } // Gain Correction
        }
    }
    
    MAX_Serial.end();
    return Temperature_;
}

bool MAX78630::ClearBuffer(void) {
    
    MAX_Serial.flush();
    while(MAX_Serial.available() > 0) char _t = MAX_Serial.read();
    delay(20);
    
    return true;
}
bool MAX78630::SendCommand(int CHR1, int CHR2) {
    
    int CheckSum = 0x100 - ((0xAA + 0x07 + 0xA3 + CHR1 + CHR2 + 0xE3) % 256);
    
    MAX_Serial.write(0xAA);			// (0xAA)
    MAX_Serial.write(0x07);			// (0x07)
    MAX_Serial.write(0xA3);			// (0xA3)
    MAX_Serial.write(CHR1);			// (CHR2)
    MAX_Serial.write(CHR2);			// (CHR1)
    MAX_Serial.write(0xE3);			// (0xE3)
    MAX_Serial.write(CheckSum);		// (CHK)
    
    delay(10);
    
    return true;
}