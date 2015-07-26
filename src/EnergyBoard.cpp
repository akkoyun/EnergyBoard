/* *******************************************************************************
 *
 *  Copyright (C) 2014-2015 LVX Elektronik A.S. (info@lvx.com.tr)
 *  Can not be copied and/or distributed without the express permission of LVX
 *
 *	LVX Energy Board is a 3 phase energy monitoring development kit
 *
 *	The library is compatable with Arduino and the Open Hardware Community.
 *
 *	Library				: LVX Energy Board (MAX78630)
 *	Code Developer		: Mehmet Gunce Akkoyun (gunce.akkoyun@lvx.com.tr)
 *	Hardware Developer	: Alp Erkan Savli (alp.savli@lvx.com.tr)
 *	GitHub				: https://github.com/LVXElektronik/EnergyBoard
 *	Revision			: 2.1.2
 *	Relase				: July 2015
 *
 *********************************************************************************/

#include "Arduino.h"
#include "EnergyBoard.h"

#define EnergyBoard_Serial Serial2	 // Define EnergyBoard Serial

/*
	Library Usage
	-------------
	Voltage_RMS				-> Read RMS voltage of selected phase
	Voltage_RMS_Average		-> Read average RMS voltage
	Voltage_Instantaneous	-> Read instantaneous voltage of selected phase
	Voltage_Fundamental		-> Read fundamental voltage of selected phase
	Voltage_Harmonic		-> Read harmonic voltage of selected phase
	Voltage_RMS_Alarm_Min	-> Read minimum voltage alarm of IC
	Voltage_RMS_Alarm_Max	-> Read maximum voltage alarm of IC
 */

EnergyBoard::EnergyBoard(int Gain_) {
    
    EnergyBoard_Serial.begin(BoudRate);

    ClearBuffer();
    EnergyBoard_Serial.write(0xAA);	// Header (0xAA)
    EnergyBoard_Serial.write(0x04);	// Total Sended Byte (0x04)
    EnergyBoard_Serial.write(0xC4);	// Setting Command (0xC4)
    EnergyBoard_Serial.write(0x8E);	// CheckSum (0x8E)

	delay(10);
	ClearBuffer();
	EnergyBoard_Serial.write(0xAA);	// Header (0xAA)
	EnergyBoard_Serial.write(0x06);	// Total Sended Byte (0x04)
	EnergyBoard_Serial.write(0xCA);	// Setting Command (0xC4)
	EnergyBoard_Serial.write(0x65);	// Setting Command (0xC4)
	EnergyBoard_Serial.write(0xFF);	// Setting Command (0xC4)
	EnergyBoard_Serial.write(0x22);	// CheckSum (0x8E)

	EnergyBoard_Serial.end();
    
    if (Gain_ != 0) Gain = true;
}

// Voltage Functions
float EnergyBoard::Voltage_RMS(char Phase) {
	
	EnergyBoard_Serial.begin(BoudRate);
	ClearBuffer();
	
	float Voltage_ = 0;
	
	// RMS Voltage Read
	// --------------------------------------------------------------------------------------------------
	if (Phase == 'R') SendCommand(0x90,0x00); // RMS Voltage Phase R = 0xAA,0x07,0xA3,0x90,0x00,0xE3,0x39
	if (Phase == 'S') SendCommand(0x93,0x00); // RMS Voltage Phase S = 0xAA,0x07,0xA3,0x93,0x00,0xE3,0x36
	if (Phase == 'T') SendCommand(0x96,0x00); // RMS Voltage Phase T = 0xAA,0x07,0xA3,0x96,0x00,0xE3,0x33
	// --------------------------------------------------------------------------------------------------
	
	if (EnergyBoard_Serial.available()) {
		
		byte Header					= EnergyBoard_Serial.read();
		byte Recieved_Byte_Count	= EnergyBoard_Serial.read();
		byte Data1					= EnergyBoard_Serial.read();
		byte Data2					= EnergyBoard_Serial.read();
		byte Data3					= EnergyBoard_Serial.read();
		byte CheckSum				= EnergyBoard_Serial.read();
		
		// Control Recieved Data
		if (Header == 0xAA) {
			Voltage_ = (Data3 * 65536 + Data2 * 256 + Data1);
		} // Acknowledge with data.
		else {
			Voltage_ = -1;
		} // Error
		
		if (Gain) {
			Voltage_ = Voltage_ * Voltage_Gain;
		} // Gain Correction
	}
	
	EnergyBoard_Serial.end();
	return Voltage_;
}
float EnergyBoard::Voltage_RMS_Average() {
	
	EnergyBoard_Serial.begin(BoudRate);
	ClearBuffer();
	
	float Voltage_ = 0;
	
	// RMS Voltage Avarage Read
	// --------------------------------------------------------------------------------
	SendCommand(0x99,0x00); // RMS Voltage Avarage = 0xAA,0x07,0xA3,0x99,0x00,0xE3,0x39
	// --------------------------------------------------------------------------------
	
	if (EnergyBoard_Serial.available()) {
		
		byte Header					= EnergyBoard_Serial.read();
		byte Recieved_Byte_Count	= EnergyBoard_Serial.read();
		byte Data1					= EnergyBoard_Serial.read();
		byte Data2					= EnergyBoard_Serial.read();
		byte Data3					= EnergyBoard_Serial.read();
		byte CheckSum				= EnergyBoard_Serial.read();
		
		// Control Recieved Data
		if (Header == 0xAA) {
			Voltage_ = (Data3 * 65536 + Data2 * 256 + Data1);
		} // Acknowledge with data.
		else {
			Voltage_ = -1;
		} // Error
		
		if (Gain) {
			Voltage_ = Voltage_ * Voltage_Gain;
		} // Gain Correction
	}
	
	EnergyBoard_Serial.end();
	return Voltage_;
}
float EnergyBoard::Voltage_Instantaneous(char Phase) {
	
	EnergyBoard_Serial.begin(BoudRate);
	ClearBuffer();
	
	float Voltage_ = 0;
	
	// Instantaneous Voltage Read
	// ------------------------------------------------------------------------------------------------------------
	if (Phase == 'R') SendCommand(0x87,0x00); // Instantaneous Voltage Phase R = 0xAA,0x07,0xA3,0x87,0x00,0xE3,0x39
	if (Phase == 'S') SendCommand(0x8A,0x00); // Instantaneous Voltage Phase S = 0xAA,0x07,0xA3,0x8A,0x00,0xE3,0x36
	if (Phase == 'T') SendCommand(0x8D,0x00); // Instantaneous Voltage Phase T = 0xAA,0x07,0xA3,0x8D,0x00,0xE3,0x33
	// ------------------------------------------------------------------------------------------------------------
	
	if (EnergyBoard_Serial.available()) {
		
		byte Header					= EnergyBoard_Serial.read();
		byte Recieved_Byte_Count	= EnergyBoard_Serial.read();
		byte Data1					= EnergyBoard_Serial.read();
		byte Data2					= EnergyBoard_Serial.read();
		byte Data3					= EnergyBoard_Serial.read();
		byte CheckSum				= EnergyBoard_Serial.read();
		
		// Control Recieved Data
		if (Header == 0xAA) {
			Voltage_ = (Data3 * 65536 + Data2 * 256 + Data1);
		} // Acknowledge with data.
		else {
			Voltage_ = -1;
		} // Error
		
		if (Gain) {
			Voltage_ = Voltage_ * Voltage_Gain;
		} // Gain Correction
	}
	
	EnergyBoard_Serial.end();
	return Voltage_;
}
float EnergyBoard::Voltage_Fundamental(char Phase) {
	
	EnergyBoard_Serial.begin(BoudRate);
	ClearBuffer();
	
	float Voltage_ = 0;
	
	// Fundamental Voltage Read
	// ----------------------------------------------------------------------------------------------------------
	if (Phase == 'R') SendCommand(0x9C,0x00); // Fundamental Voltage Phase R = 0xAA,0x07,0xA3,0x90,0x00,0xE3,0x39
	if (Phase == 'S') SendCommand(0x9F,0x00); // Fundamental Voltage Phase S = 0xAA,0x07,0xA3,0x93,0x00,0xE3,0x36
	if (Phase == 'T') SendCommand(0xA2,0x00); // Fundamental Voltage Phase T = 0xAA,0x07,0xA3,0x96,0x00,0xE3,0x33
	// ----------------------------------------------------------------------------------------------------------
	
	if (EnergyBoard_Serial.available()) {
		
		byte Header					= EnergyBoard_Serial.read();
		byte Recieved_Byte_Count	= EnergyBoard_Serial.read();
		byte Data1					= EnergyBoard_Serial.read();
		byte Data2					= EnergyBoard_Serial.read();
		byte Data3					= EnergyBoard_Serial.read();
		byte CheckSum				= EnergyBoard_Serial.read();
		
		// Control Recieved Data
		if (Header == 0xAA) {
			Voltage_ = (Data3 * 65536 + Data2 * 256 + Data1);
		} // Acknowledge with data.
		else {
			Voltage_ = -1;
		} // Error
		
		if (Gain) {
			Voltage_ = Voltage_ * Voltage_Gain;
		} // Gain Correction
	}
	
	EnergyBoard_Serial.end();
	return Voltage_;
}
float EnergyBoard::Voltage_Harmonic(char Phase) {

	EnergyBoard_Serial.begin(BoudRate);
	ClearBuffer();
	
	float Voltage_ = 0;
	
	// Harmonic Voltage Read
	// ----------------------------------------------------------------------------------------------------------
	if (Phase == 'R') SendCommand(0xA5,0x00); // Harmonic Voltage Phase R = 0xAA,0x07,0xA3,0xA5,0x00,0xE3,0x39
	if (Phase == 'S') SendCommand(0xA8,0x00); // Harmonic Voltage Phase S = 0xAA,0x07,0xA3,0xA8,0x00,0xE3,0x36
	if (Phase == 'T') SendCommand(0xAB,0x00); // Harmonic Voltage Phase T = 0xAA,0x07,0xA3,0xAB,0x00,0xE3,0x33
	// ----------------------------------------------------------------------------------------------------------
	
	if (EnergyBoard_Serial.available()) {
		
		byte Header					= EnergyBoard_Serial.read();
		byte Recieved_Byte_Count	= EnergyBoard_Serial.read();
		byte Data1					= EnergyBoard_Serial.read();
		byte Data2					= EnergyBoard_Serial.read();
		byte Data3					= EnergyBoard_Serial.read();
		byte CheckSum				= EnergyBoard_Serial.read();
		
		// Control Recieved Data
		if (Header == 0xAA) {
			Voltage_ = (Data3 * 65536 + Data2 * 256 + Data1);
		} // Acknowledge with data.
		else {
			Voltage_ = -1;
		} // Error
		
		if (Gain) {
			Voltage_ = Voltage_ * Voltage_Gain;
		} // Gain Correction
	}
	
	EnergyBoard_Serial.end();
	return Voltage_;
}
float EnergyBoard::Voltage_RMS_Alarm_Min() {
	
	EnergyBoard_Serial.begin(BoudRate);
	ClearBuffer();
	
	float Voltage_ = 0;
	
	// RMS Voltage MIN Limit Read
	// ----------------------------------------------------------------------------------
	SendCommand(0xB1,0x00); // RMS Voltage MIN Limit = 0xAA,0x07,0xA3,0xB1,0x00,0xE3,0x39
	// ----------------------------------------------------------------------------------
	
	if (EnergyBoard_Serial.available()) {
		
		byte Header					= EnergyBoard_Serial.read();
		byte Recieved_Byte_Count	= EnergyBoard_Serial.read();
		byte Data1					= EnergyBoard_Serial.read();
		byte Data2					= EnergyBoard_Serial.read();
		byte Data3					= EnergyBoard_Serial.read();
		byte CheckSum				= EnergyBoard_Serial.read();
		
		// Control Recieved Data
		if (Header == 0xAA) {
			Voltage_ = (Data3 * 65536 + Data2 * 256 + Data1);
		} // Acknowledge with data.
		else {
			Voltage_ = -1;
		} // Error
		
		if (Gain) {
			Voltage_ = Voltage_ * Voltage_Gain;
		} // Gain Correction
	}
	
	EnergyBoard_Serial.end();
	return Voltage_;
}
float EnergyBoard::Voltage_RMS_Alarm_Max() {
	
	EnergyBoard_Serial.begin(BoudRate);
	ClearBuffer();
	
	float Voltage_ = 0;
	
	// RMS Voltage MAX Limit Read
	// ----------------------------------------------------------------------------------
	SendCommand(0xB4,0x00); // RMS Voltage MAX Limit = 0xAA,0x07,0xA3,0xB4,0x00,0xE3,0x39
	// ----------------------------------------------------------------------------------
	
	if (EnergyBoard_Serial.available()) {
		
		byte Header					= EnergyBoard_Serial.read();
		byte Recieved_Byte_Count	= EnergyBoard_Serial.read();
		byte Data1					= EnergyBoard_Serial.read();
		byte Data2					= EnergyBoard_Serial.read();
		byte Data3					= EnergyBoard_Serial.read();
		byte CheckSum				= EnergyBoard_Serial.read();
		
		// Control Recieved Data
		if (Header == 0xAA) {
			Voltage_ = (Data3 * 65536 + Data2 * 256 + Data1);
		} // Acknowledge with data.
		else {
			Voltage_ = -1;
		} // Error
		
		if (Gain) {
			Voltage_ = Voltage_ * Voltage_Gain;
		} // Gain Correction
	}
	
	EnergyBoard_Serial.end();
	return abs(Voltage_);
}

// Current Function
float EnergyBoard::Current_RMS(char Phase) {
	
	EnergyBoard_Serial.begin(BoudRate);
	ClearBuffer();
	
	float Current_ = 0;
	
	while (Current_ <= 0) {
	
		// RMS Current Read
		// --------------------------------------------------------------------------------------------------
		if (Phase == 'R') SendCommand(0xD5,0x00); // RMS Current Phase R = 0xAA,0x07,0xA3,0xD5,0x00,0xE3,0xF4
		if (Phase == 'S') SendCommand(0xD8,0x00); // RMS Current Phase S = 0xAA,0x07,0xA3,0xD8,0x00,0xE3,0xF1
		if (Phase == 'T') SendCommand(0xDB,0x00); // RMS Current Phase T = 0xAA,0x07,0xA3,0xDB,0x00,0xE3,0xEE
		// --------------------------------------------------------------------------------------------------

		if (EnergyBoard_Serial.available()) {

			byte Header					= EnergyBoard_Serial.read();
			byte Recieved_Byte_Count	= EnergyBoard_Serial.read();
			byte Data1					= EnergyBoard_Serial.read();
			byte Data2					= EnergyBoard_Serial.read();
			byte Data3					= EnergyBoard_Serial.read();
			byte CheckSum				= EnergyBoard_Serial.read();

			// Control Recieved Data
			if (Header == 0xAA) {
                Current_ = (Data3 * 65536 + Data2 * 256 + Data1);
            } // Acknowledge with data.
			if (Header == 0xAD) {
				return -1001;
			} // Acknowledge without data. (Error: -1001)
			if (Header == 0xB0) {
				return -1002;
			} // Negative Acknowledge (NACK). (Error: -1002)
			if (Header == 0xBC) {
				return -1003;
			} // Command not implemented. (Error: -1003)
			if (Header == 0xBD) {
				return -1004;
			} // Checksum failed. (Error: -1004)

			if (Gain) {
                Current_ = Current_ * Current_Gain;
            } // Gain Correction
		}
	}
	
	EnergyBoard_Serial.end();
	return abs(Current_);
}
float EnergyBoard::Current_RMS_Average() {
	
	EnergyBoard_Serial.begin(BoudRate);
	ClearBuffer();
	
	float Current_ = 0;

	// RMS Current Avarage Read
	// --------------------------------------------------------------------------------
	SendCommand(0xDE,0x00); // RMS Current Avarage = 0xAA,0x07,0xA3,0xDE,0x00,0xE3,0xF4
	// --------------------------------------------------------------------------------
	
	if (EnergyBoard_Serial.available()) {
		
		byte Header					= EnergyBoard_Serial.read();
		byte Recieved_Byte_Count	= EnergyBoard_Serial.read();
		byte Data1					= EnergyBoard_Serial.read();
		byte Data2					= EnergyBoard_Serial.read();
		byte Data3					= EnergyBoard_Serial.read();
		byte CheckSum				= EnergyBoard_Serial.read();
		
		// Control Recieved Data
		if (Header == 0xAA) {
			Current_ = (Data3 * 65536 + Data2 * 256 + Data1);
		} // Acknowledge with data.
		if (Header == 0xAD) {
			return -1001;
		} // Acknowledge without data. (Error: -1001)
		if (Header == 0xB0) {
			return -1002;
		} // Negative Acknowledge (NACK). (Error: -1002)
		if (Header == 0xBC) {
			return -1003;
		} // Command not implemented. (Error: -1003)
		if (Header == 0xBD) {
			return -1004;
		} // Checksum failed. (Error: -1004)
		
		if (Gain) {
			Current_ = Current_ * Current_Gain;
		} // Gain Correction
	}
	
	EnergyBoard_Serial.end();
	return abs(Current_);
}
float EnergyBoard::Current_Peak(char Phase) {
	
	EnergyBoard_Serial.begin(BoudRate);
	ClearBuffer();
	
	float Current_ = 0;
	
	// Peak Current Read
	// ---------------------------------------------------------------------------------------------------
	if (Phase == 'R') SendCommand(0xCC,0x00); // Peak Current Phase R = 0xAA,0x07,0xA3,0xCC,0x00,0xE3,0xF4
	if (Phase == 'S') SendCommand(0xCF,0x00); // Peak Current Phase S = 0xAA,0x07,0xA3,0xCF,0x00,0xE3,0xF1
	if (Phase == 'T') SendCommand(0xD2,0x00); // Peak Current Phase T = 0xAA,0x07,0xA3,0xD2,0x00,0xE3,0xEE
	// ---------------------------------------------------------------------------------------------------
	
	if (EnergyBoard_Serial.available()) {
		
		byte Header					= EnergyBoard_Serial.read();
		byte Recieved_Byte_Count	= EnergyBoard_Serial.read();
		byte Data1					= EnergyBoard_Serial.read();
		byte Data2					= EnergyBoard_Serial.read();
		byte Data3					= EnergyBoard_Serial.read();
		byte CheckSum				= EnergyBoard_Serial.read();
		
		// Control Recieved Data
		if (Header == 0xAA) {
			Current_ = (Data3 * 65536 + Data2 * 256 + Data1);
		} // Acknowledge with data.
		if (Header == 0xAD) {
			return -1001;
		} // Acknowledge without data. (Error: -1001)
		if (Header == 0xB0) {
			return -1002;
		} // Negative Acknowledge (NACK). (Error: -1002)
		if (Header == 0xBC) {
			return -1003;
		} // Command not implemented. (Error: -1003)
		if (Header == 0xBD) {
			return -1004;
		} // Checksum failed. (Error: -1004)
		
		if (Gain) {
			Current_ = Current_ * Current_Gain;
		} // Gain Correction
	}
	
	EnergyBoard_Serial.end();
	return abs(Current_);
}
float EnergyBoard::Current_Instantaneous(char Phase) {
	
	EnergyBoard_Serial.begin(BoudRate);
	ClearBuffer();
	
	float Current_ = 0;
	
	// Instantaneous Current Read
	// ------------------------------------------------------------------------------------------------------------
	if (Phase == 'R') SendCommand(0xBA,0x00); // Instantaneous Current Phase R = 0xAA,0x07,0xA3,0xBA,0x00,0xE3,0xF4
	if (Phase == 'S') SendCommand(0xBD,0x00); // Instantaneous Current Phase S = 0xAA,0x07,0xA3,0xBD,0x00,0xE3,0xF1
	if (Phase == 'T') SendCommand(0xC0,0x00); // Instantaneous Current Phase T = 0xAA,0x07,0xA3,0xC0,0x00,0xE3,0xEE
	// ------------------------------------------------------------------------------------------------------------
	
	if (EnergyBoard_Serial.available()) {
		
		byte Header					= EnergyBoard_Serial.read();
		byte Recieved_Byte_Count	= EnergyBoard_Serial.read();
		byte Data1					= EnergyBoard_Serial.read();
		byte Data2					= EnergyBoard_Serial.read();
		byte Data3					= EnergyBoard_Serial.read();
		byte CheckSum				= EnergyBoard_Serial.read();
		
		// Control Recieved Data
		if (Header == 0xAA) {
			Current_ = (Data3 * 65536 + Data2 * 256 + Data1);
		} // Acknowledge with data.
		if (Header == 0xAD) {
			return -1001;
		} // Acknowledge without data. (Error: -1001)
		if (Header == 0xB0) {
			return -1002;
		} // Negative Acknowledge (NACK). (Error: -1002)
		if (Header == 0xBC) {
			return -1003;
		} // Command not implemented. (Error: -1003)
		if (Header == 0xBD) {
			return -1004;
		} // Checksum failed. (Error: -1004)
		
		if (Gain) {
			Current_ = Current_ * Current_Gain;
		} // Gain Correction
	}
	
	EnergyBoard_Serial.end();
	return abs(Current_);
}
float EnergyBoard::Current_Fundamental(char Phase) {
	
	EnergyBoard_Serial.begin(BoudRate);
	ClearBuffer();
	
	float Current_ = 0;
	
	// Fundamental Current Read
	// ----------------------------------------------------------------------------------------------------------
	if (Phase == 'R') SendCommand(0xE1,0x00); // Fundamental Current Phase R = 0xAA,0x07,0xA3,0xE1,0x00,0xE3,0xF4
	if (Phase == 'S') SendCommand(0xE4,0x00); // Fundamental Current Phase S = 0xAA,0x07,0xA3,0xE4,0x00,0xE3,0xF1
	if (Phase == 'T') SendCommand(0xE7,0x00); // Fundamental Current Phase T = 0xAA,0x07,0xA3,0xE7,0x00,0xE3,0xEE
	// ----------------------------------------------------------------------------------------------------------
	
	if (EnergyBoard_Serial.available()) {
		
		byte Header					= EnergyBoard_Serial.read();
		byte Recieved_Byte_Count	= EnergyBoard_Serial.read();
		byte Data1					= EnergyBoard_Serial.read();
		byte Data2					= EnergyBoard_Serial.read();
		byte Data3					= EnergyBoard_Serial.read();
		byte CheckSum				= EnergyBoard_Serial.read();
		
		// Control Recieved Data
		if (Header == 0xAA) {
			Current_ = (Data3 * 65536 + Data2 * 256 + Data1);
		} // Acknowledge with data.
		if (Header == 0xAD) {
			return -1001;
		} // Acknowledge without data. (Error: -1001)
		if (Header == 0xB0) {
			return -1002;
		} // Negative Acknowledge (NACK). (Error: -1002)
		if (Header == 0xBC) {
			return -1003;
		} // Command not implemented. (Error: -1003)
		if (Header == 0xBD) {
			return -1004;
		} // Checksum failed. (Error: -1004)
		
		if (Gain) {
			Current_ = Current_ * Current_Gain;
		} // Gain Correction
	}
	
	EnergyBoard_Serial.end();
	return abs(Current_);
}
float EnergyBoard::Current_Harmonic(char Phase) {
	
	EnergyBoard_Serial.begin(BoudRate);
	ClearBuffer();
	
	float Current_ = 0;
	
	// Harmonic Current Read
	// -------------------------------------------------------------------------------------------------------
	if (Phase == 'R') SendCommand(0xEA,0x00); // Harmonic Current Phase R = 0xAA,0x07,0xA3,0xEA,0x00,0xE3,0xF4
	if (Phase == 'S') SendCommand(0xED,0x00); // Harmonic Current Phase S = 0xAA,0x07,0xA3,0xED,0x00,0xE3,0xF1
	if (Phase == 'T') SendCommand(0xF0,0x00); // Harmonic Current Phase T = 0xAA,0x07,0xA3,0xF0,0x00,0xE3,0xEE
	// -------------------------------------------------------------------------------------------------------
	
	if (EnergyBoard_Serial.available()) {
		
		byte Header					= EnergyBoard_Serial.read();
		byte Recieved_Byte_Count	= EnergyBoard_Serial.read();
		byte Data1					= EnergyBoard_Serial.read();
		byte Data2					= EnergyBoard_Serial.read();
		byte Data3					= EnergyBoard_Serial.read();
		byte CheckSum				= EnergyBoard_Serial.read();
		
		// Control Recieved Data
		if (Header == 0xAA) {
			Current_ = (Data3 * 65536 + Data2 * 256 + Data1);
		} // Acknowledge with data.
		if (Header == 0xAD) {
			return -1001;
		} // Acknowledge without data. (Error: -1001)
		if (Header == 0xB0) {
			return -1002;
		} // Negative Acknowledge (NACK). (Error: -1002)
		if (Header == 0xBC) {
			return -1003;
		} // Command not implemented. (Error: -1003)
		if (Header == 0xBD) {
			return -1004;
		} // Checksum failed. (Error: -1004)
		
		if (Gain) {
			Current_ = Current_ * Current_Gain;
		} // Gain Correction
	}
	
	EnergyBoard_Serial.end();
	return abs(Current_);
}
float EnergyBoard::Current_RMS_Alarm_Max() {

	EnergyBoard_Serial.begin(BoudRate);
	ClearBuffer();
	
	float Current_ = 0;
	
	// RMS Current Max Limit Read
	// ----------------------------------------------------------------------------------
	SendCommand(0xF3,0x00); // RMS Current Max Limit = 0xAA,0x07,0xA3,0xF3,0x00,0xE3,0xF4
	// ----------------------------------------------------------------------------------
	
	if (EnergyBoard_Serial.available()) {
		
		byte Header					= EnergyBoard_Serial.read();
		byte Recieved_Byte_Count	= EnergyBoard_Serial.read();
		byte Data1					= EnergyBoard_Serial.read();
		byte Data2					= EnergyBoard_Serial.read();
		byte Data3					= EnergyBoard_Serial.read();
		byte CheckSum				= EnergyBoard_Serial.read();
		
		// Control Recieved Data
		if (Header == 0xAA) {
			Current_ = (Data3 * 65536 + Data2 * 256 + Data1);
		} // Acknowledge with data.
		if (Header == 0xAD) {
			return -1001;
		} // Acknowledge without data. (Error: -1001)
		if (Header == 0xB0) {
			return -1002;
		} // Negative Acknowledge (NACK). (Error: -1002)
		if (Header == 0xBC) {
			return -1003;
		} // Command not implemented. (Error: -1003)
		if (Header == 0xBD) {
			return -1004;
		} // Checksum failed. (Error: -1004)
		
		if (Gain) {
			Current_ = Current_ * Current_Gain;
		} // Gain Correction
	}
	
	EnergyBoard_Serial.end();
	return abs(Current_);
}

// Power Functions
float EnergyBoard::Power_Active(char Phase) {

	EnergyBoard_Serial.begin(BoudRate);
	ClearBuffer();

	float ActivePower_ = 0;

	// Active Power Read
	// -------------------------------------------------------------------------------------------------------
	if (Phase == 'R') SendCommand(0x1D,0x01); // Active Power Phase R = 0xAA,0x07,0xA3,0x1D,0x01,0xE3,0xF4
	if (Phase == 'S') SendCommand(0x20,0x01); // Active Power Phase S = 0xAA,0x07,0xA3,0x20,0x01,0xE3,0xF1
	if (Phase == 'T') SendCommand(0x23,0x01); // Active Power Phase T = 0xAA,0x07,0xA3,0x23,0x01,0xE3,0xEE
	// -------------------------------------------------------------------------------------------------------

	if (EnergyBoard_Serial.available()) {
		
		byte Header					= EnergyBoard_Serial.read();
		byte Recieved_Byte_Count	= EnergyBoard_Serial.read();
		byte Data1					= EnergyBoard_Serial.read();
		byte Data2					= EnergyBoard_Serial.read();
		byte Data3					= EnergyBoard_Serial.read();
		byte CheckSum				= EnergyBoard_Serial.read();
		
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
			return -1001;
		} // Acknowledge without data. (Error: -1001)
		if (Header == 0xB0) {
			return -1002;
		} // Negative Acknowledge (NACK). (Error: -1002)
		if (Header == 0xBC) {
			return -1003;
		} // Command not implemented. (Error: -1003)
		if (Header == 0xBD) {
			return -1004;
		} // Checksum failed. (Error: -1004)
		
		if (Gain) {
			ActivePower_ = ActivePower_ * Power_Gain;
		} // Gain Correction
	}

	EnergyBoard_Serial.end();
	return abs(ActivePower_);
}
float EnergyBoard::Power_Active_Average() {

	EnergyBoard_Serial.begin(BoudRate);
	ClearBuffer();
	
	float ActivePower_ = 0;
	
	// Active Power Avarage Read
	// ---------------------------------------------------------------------------------
	SendCommand(0x38,0x01); // Active Power Avarage = 0xAA,0x07,0xA3,0x38,0x01,0xE3,0xF4
	// ---------------------------------------------------------------------------------
	
	if (EnergyBoard_Serial.available()) {
		
		byte Header					= EnergyBoard_Serial.read();
		byte Recieved_Byte_Count	= EnergyBoard_Serial.read();
		byte Data1					= EnergyBoard_Serial.read();
		byte Data2					= EnergyBoard_Serial.read();
		byte Data3					= EnergyBoard_Serial.read();
		byte CheckSum				= EnergyBoard_Serial.read();
		
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
			return -1001;
		} // Acknowledge without data. (Error: -1001)
		if (Header == 0xB0) {
			return -1002;
		} // Negative Acknowledge (NACK). (Error: -1002)
		if (Header == 0xBC) {
			return -1003;
		} // Command not implemented. (Error: -1003)
		if (Header == 0xBD) {
			return -1004;
		} // Checksum failed. (Error: -1004)
		
		if (Gain) {
			ActivePower_ = ActivePower_ * Power_Gain;
		} // Gain Correction
	}
	
	EnergyBoard_Serial.end();
	return abs(ActivePower_);
}
float EnergyBoard::Power_Reactive(char Phase) {
	
	EnergyBoard_Serial.begin(BoudRate);
	ClearBuffer();
	
	float ReActivePower_ = 0;
	
	// ReActive Power Read
	// -----------------------------------------------------------------------------------------------------
	if (Phase == 'R') SendCommand(0x26,0x01); // ReActive Power Phase R = 0xAA,0x07,0xA3,0x26,0x01,0xE3,0xF4
	if (Phase == 'S') SendCommand(0x29,0x01); // ReActive Power Phase S = 0xAA,0x07,0xA3,0x29,0x01,0xE3,0xF1
	if (Phase == 'T') SendCommand(0x2C,0x01); // ReActive Power Phase T = 0xAA,0x07,0xA3,0x2C,0x01,0xE3,0xEE
	// -----------------------------------------------------------------------------------------------------
	
	if (EnergyBoard_Serial.available()) {
		
		byte Header					= EnergyBoard_Serial.read();
		byte Recieved_Byte_Count	= EnergyBoard_Serial.read();
		byte Data1					= EnergyBoard_Serial.read();
		byte Data2					= EnergyBoard_Serial.read();
		byte Data3					= EnergyBoard_Serial.read();
		byte CheckSum				= EnergyBoard_Serial.read();
		
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
			return -1001;
		} // Acknowledge without data. (Error: -1001)
		if (Header == 0xB0) {
			return -1002;
		} // Negative Acknowledge (NACK). (Error: -1002)
		if (Header == 0xBC) {
			return -1003;
		} // Command not implemented. (Error: -1003)
		if (Header == 0xBD) {
			return -1004;
		} // Checksum failed. (Error: -1004)
		
		if (Gain) {
			ReActivePower_ = ReActivePower_ * Power_Gain;
		} // Gain Correction
	}
	
	EnergyBoard_Serial.end();
	return abs(ReActivePower_);
}
float EnergyBoard::Power_Reactive_Average() {
	
	EnergyBoard_Serial.begin(BoudRate);
	ClearBuffer();
	
	float ReActivePower_ = 0;
	
	// ReActive Power Avarage Read
	// ---------------------------------------------------------------------------------
	SendCommand(0x3B,0x01); // ReActive Power Avarage = 0xAA,0x07,0xA3,0x38,0x01,0xE3,0xF4
	// ---------------------------------------------------------------------------------
	
	if (EnergyBoard_Serial.available()) {
		
		byte Header					= EnergyBoard_Serial.read();
		byte Recieved_Byte_Count	= EnergyBoard_Serial.read();
		byte Data1					= EnergyBoard_Serial.read();
		byte Data2					= EnergyBoard_Serial.read();
		byte Data3					= EnergyBoard_Serial.read();
		byte CheckSum				= EnergyBoard_Serial.read();
		
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
			return -1001;
		} // Acknowledge without data. (Error: -1001)
		if (Header == 0xB0) {
			return -1002;
		} // Negative Acknowledge (NACK). (Error: -1002)
		if (Header == 0xBC) {
			return -1003;
		} // Command not implemented. (Error: -1003)
		if (Header == 0xBD) {
			return -1004;
		} // Checksum failed. (Error: -1004)
		
		if (Gain) {
			ReActivePower_ = ReActivePower_ * Power_Gain;
		} // Gain Correction
	}
	
	EnergyBoard_Serial.end();
	return abs(ReActivePower_);
}
float EnergyBoard::Power_Apparent(char Phase) {

	EnergyBoard_Serial.begin(BoudRate);
	ClearBuffer();
	
	float ApparentPower_ = 0;
	
	// Apparent Power Read
	// -----------------------------------------------------------------------------------------------------
	if (Phase == 'R') SendCommand(0x2F,0x01); // Apparent Power Phase R = 0xAA,0x07,0xA3,0x2F,0x01,0xE3,0xF4
	if (Phase == 'S') SendCommand(0x32,0x01); // Apparent Power Phase S = 0xAA,0x07,0xA3,0x32,0x01,0xE3,0xF1
	if (Phase == 'T') SendCommand(0x35,0x01); // Apparent Power Phase T = 0xAA,0x07,0xA3,0x35,0x01,0xE3,0xEE
	// -----------------------------------------------------------------------------------------------------
	
	if (EnergyBoard_Serial.available()) {
		
		byte Header					= EnergyBoard_Serial.read();
		byte Recieved_Byte_Count	= EnergyBoard_Serial.read();
		byte Data1					= EnergyBoard_Serial.read();
		byte Data2					= EnergyBoard_Serial.read();
		byte Data3					= EnergyBoard_Serial.read();
		byte CheckSum				= EnergyBoard_Serial.read();
		
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
			return -1001;
		} // Acknowledge without data. (Error: -1001)
		if (Header == 0xB0) {
			return -1002;
		} // Negative Acknowledge (NACK). (Error: -1002)
		if (Header == 0xBC) {
			return -1003;
		} // Command not implemented. (Error: -1003)
		if (Header == 0xBD) {
			return -1004;
		} // Checksum failed. (Error: -1004)
		
		if (Gain) {
			ApparentPower_ = ApparentPower_ * Power_Gain;
		} // Gain Correction
	}
	
	EnergyBoard_Serial.end();
	return abs(ApparentPower_);
}
float EnergyBoard::Power_Apparent_Average() {
	
	EnergyBoard_Serial.begin(BoudRate);
	ClearBuffer();
	
	float ApparentPower_ = 0;
	
	// Apparent Power Avarage Read
	// ---------------------------------------------------------------------------------
	SendCommand(0x3E,0x01); // Apparent Power Avarage = 0xAA,0x07,0xA3,0x3E,0x01,0xE3,0xF4
	// ---------------------------------------------------------------------------------
	
	if (EnergyBoard_Serial.available()) {
		
		byte Header					= EnergyBoard_Serial.read();
		byte Recieved_Byte_Count	= EnergyBoard_Serial.read();
		byte Data1					= EnergyBoard_Serial.read();
		byte Data2					= EnergyBoard_Serial.read();
		byte Data3					= EnergyBoard_Serial.read();
		byte CheckSum				= EnergyBoard_Serial.read();
		
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
			return -1001;
		} // Acknowledge without data. (Error: -1001)
		if (Header == 0xB0) {
			return -1002;
		} // Negative Acknowledge (NACK). (Error: -1002)
		if (Header == 0xBC) {
			return -1003;
		} // Command not implemented. (Error: -1003)
		if (Header == 0xBD) {
			return -1004;
		} // Checksum failed. (Error: -1004)
		
		if (Gain) {
			ApparentPower_ = ApparentPower_ * Power_Gain;
		} // Gain Correction
	}
	
	EnergyBoard_Serial.end();
	return abs(ApparentPower_);
}
float EnergyBoard::Power_Fundamental(char Phase) {
	
	EnergyBoard_Serial.begin(BoudRate);
	ClearBuffer();
	
	float FundamentalPower_ = 0;
	
	// Fundamental Power Read
	// -----------------------------------------------------------------------------------------------------
	if (Phase == 'R') SendCommand(0x4A,0x01); // Fundamental Power Phase R = 0xAA,0x07,0xA3,0x2F,0x01,0xE3,0xF4
	if (Phase == 'S') SendCommand(0x4D,0x01); // Fundamental Power Phase S = 0xAA,0x07,0xA3,0x32,0x01,0xE3,0xF1
	if (Phase == 'T') SendCommand(0x50,0x01); // Fundamental Power Phase T = 0xAA,0x07,0xA3,0x35,0x01,0xE3,0xEE
	// -----------------------------------------------------------------------------------------------------
	
	if (EnergyBoard_Serial.available()) {
		
		byte Header					= EnergyBoard_Serial.read();
		byte Recieved_Byte_Count	= EnergyBoard_Serial.read();
		byte Data1					= EnergyBoard_Serial.read();
		byte Data2					= EnergyBoard_Serial.read();
		byte Data3					= EnergyBoard_Serial.read();
		byte CheckSum				= EnergyBoard_Serial.read();
		
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
			return -1001;
		} // Acknowledge without data. (Error: -1001)
		if (Header == 0xB0) {
			return -1002;
		} // Negative Acknowledge (NACK). (Error: -1002)
		if (Header == 0xBC) {
			return -1003;
		} // Command not implemented. (Error: -1003)
		if (Header == 0xBD) {
			return -1004;
		} // Checksum failed. (Error: -1004)
		
		if (Gain) {
			FundamentalPower_ = FundamentalPower_ * Power_Gain;
		} // Gain Correction
	}
	
	EnergyBoard_Serial.end();
	return abs(FundamentalPower_);
}
float EnergyBoard::Power_Harmonic(char Phase) {
	
	EnergyBoard_Serial.begin(BoudRate);
	ClearBuffer();
	
	float HarmonicPower_ = 0;
	
	// Harmonic Power Read
	// -----------------------------------------------------------------------------------------------------
	if (Phase == 'R') SendCommand(0x53,0x01); // Harmonic Power Phase R = 0xAA,0x07,0xA3,0x53,0x01,0xE3,0xF4
	if (Phase == 'S') SendCommand(0x56,0x01); // Harmonic Power Phase S = 0xAA,0x07,0xA3,0x56,0x01,0xE3,0xF1
	if (Phase == 'T') SendCommand(0x59,0x01); // Harmonic Power Phase T = 0xAA,0x07,0xA3,0x59,0x01,0xE3,0xEE
	// -----------------------------------------------------------------------------------------------------
	
	if (EnergyBoard_Serial.available()) {
		
		byte Header					= EnergyBoard_Serial.read();
		byte Recieved_Byte_Count	= EnergyBoard_Serial.read();
		byte Data1					= EnergyBoard_Serial.read();
		byte Data2					= EnergyBoard_Serial.read();
		byte Data3					= EnergyBoard_Serial.read();
		byte CheckSum				= EnergyBoard_Serial.read();
		
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
			return -1001;
		} // Acknowledge without data. (Error: -1001)
		if (Header == 0xB0) {
			return -1002;
		} // Negative Acknowledge (NACK). (Error: -1002)
		if (Header == 0xBC) {
			return -1003;
		} // Command not implemented. (Error: -1003)
		if (Header == 0xBD) {
			return -1004;
		} // Checksum failed. (Error: -1004)
		
		if (Gain) {
			HarmonicPower_ = HarmonicPower_ * Power_Gain;
		} // Gain Correction
	}
	
	EnergyBoard_Serial.end();
	return abs(HarmonicPower_);
}
float EnergyBoard::Power_Fundamental_VA(char Phase) {
	
	EnergyBoard_Serial.begin(BoudRate);
	ClearBuffer();
	
	float FundamentalVA_ = 0;
	
	// Fundamental Volt Amper Read
	// -----------------------------------------------------------------------------------------------------
	if (Phase == 'R') SendCommand(0x5C,0x01); // Fundamental VA Phase R = 0xAA,0x07,0xA3,0x53,0x01,0xE3,0xF4
	if (Phase == 'S') SendCommand(0x5F,0x01); // Fundamental VA Phase S = 0xAA,0x07,0xA3,0x56,0x01,0xE3,0xF1
	if (Phase == 'T') SendCommand(0x62,0x01); // Fundamental VA Phase T = 0xAA,0x07,0xA3,0x59,0x01,0xE3,0xEE
	// -----------------------------------------------------------------------------------------------------
	
	if (EnergyBoard_Serial.available()) {
		
		byte Header					= EnergyBoard_Serial.read();
		byte Recieved_Byte_Count	= EnergyBoard_Serial.read();
		byte Data1					= EnergyBoard_Serial.read();
		byte Data2					= EnergyBoard_Serial.read();
		byte Data3					= EnergyBoard_Serial.read();
		byte CheckSum				= EnergyBoard_Serial.read();
		
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
			return -1001;
		} // Acknowledge without data. (Error: -1001)
		if (Header == 0xB0) {
			return -1002;
		} // Negative Acknowledge (NACK). (Error: -1002)
		if (Header == 0xBC) {
			return -1003;
		} // Command not implemented. (Error: -1003)
		if (Header == 0xBD) {
			return -1004;
		} // Checksum failed. (Error: -1004)
		
		if (Gain) {
			FundamentalVA_ = FundamentalVA_ * Power_Gain;
		} // Gain Correction
	}
	
	EnergyBoard_Serial.end();
	return abs(FundamentalVA_);
}
float EnergyBoard::Power_Factor(char Phase) {

	EnergyBoard_Serial.begin(BoudRate);
	ClearBuffer();
	
	float PowerFactor_ = 0;
	
	// PowerFactor Read
	// --------------------------------------------------------------------------------------------------
	if (Phase == 'R') SendCommand(0x65,0x01); // PowerFactor Phase R = 0xAA,0x07,0xA3,0x65,0x01,0xE3,0x63
	if (Phase == 'S') SendCommand(0x68,0x01); // PowerFactor Phase S = 0xAA,0x07,0xA3,0x68,0x01,0xE3,0x60
	if (Phase == 'T') SendCommand(0x6B,0x01); // PowerFactor Phase T = 0xAA,0x07,0xA3,0x6B,0x01,0xE3,0x5D
	// --------------------------------------------------------------------------------------------------

	if (EnergyBoard_Serial.available()) {

		byte Header					= EnergyBoard_Serial.read();
		byte Recieved_Byte_Count	= EnergyBoard_Serial.read();
		byte Data1					= EnergyBoard_Serial.read();
		byte Data2					= EnergyBoard_Serial.read();
		byte Data3					= EnergyBoard_Serial.read();
		byte CheckSum				= EnergyBoard_Serial.read();

		// Control Recieved Data
		if (Header == 0xAA) {
			if (Data3 < 128) {
				PowerFactor_ = (Data3 * 65536 + Data2 * 256 + Data1);
			}
			else {
				PowerFactor_ = -((255 - Data3) * 65536 + (255 - Data2) * 256 + (255 - Data1));
			}
		} // Acknowledge with data.
		if (Header == 0xAD) {
			return -1001;
		} // Acknowledge without data. (Error: -1001)
		if (Header == 0xB0) {
			return -1002;
		} // Negative Acknowledge (NACK). (Error: -1002)
		if (Header == 0xBC) {
			return -1003;
		} // Command not implemented. (Error: -1003)
		if (Header == 0xBD) {
			return -1004;
		} // Checksum failed. (Error: -1004)

		if (Gain) {
			PowerFactor_ = PowerFactor_ * Power_Factor_Gain;
		} // Gain Correction
	}
	
	EnergyBoard_Serial.end();
	return abs(PowerFactor_);
}
float EnergyBoard::Power_Factor_Average() {

	EnergyBoard_Serial.begin(BoudRate);
	ClearBuffer();
	
	float PowerFactor_ = 0;
	
	// PowerFactor Avarage Read
	// --------------------------------------------------------------------------------
	SendCommand(0x6E,0x01); // PowerFactor Avarage = 0xAA,0x07,0xA3,0x65,0x01,0xE3,0x63
	// --------------------------------------------------------------------------------
	
	if (EnergyBoard_Serial.available()) {
		
		byte Header					= EnergyBoard_Serial.read();
		byte Recieved_Byte_Count	= EnergyBoard_Serial.read();
		byte Data1					= EnergyBoard_Serial.read();
		byte Data2					= EnergyBoard_Serial.read();
		byte Data3					= EnergyBoard_Serial.read();
		byte CheckSum				= EnergyBoard_Serial.read();
		
		// Control Recieved Data
		if (Header == 0xAA) {
			if (Data3 < 128) {
				PowerFactor_ = (Data3 * 65536 + Data2 * 256 + Data1);
			}
			else {
				PowerFactor_ = -((255 - Data3) * 65536 + (255 - Data2) * 256 + (255 - Data1));
			}
		} // Acknowledge with data.
		if (Header == 0xAD) {
			return -1001;
		} // Acknowledge without data. (Error: -1001)
		if (Header == 0xB0) {
			return -1002;
		} // Negative Acknowledge (NACK). (Error: -1002)
		if (Header == 0xBC) {
			return -1003;
		} // Command not implemented. (Error: -1003)
		if (Header == 0xBD) {
			return -1004;
		} // Checksum failed. (Error: -1004)
		
		if (Gain) {
			PowerFactor_ = PowerFactor_ * Power_Factor_Gain;
		} // Gain Correction
	}
	
	EnergyBoard_Serial.end();
	return abs(PowerFactor_);
}

// Other Functions
float EnergyBoard::Frequency(void) {
	
    EnergyBoard_Serial.begin(BoudRate);
	ClearBuffer();
	
    float Frequency_ = 0;
	
	// Frequency Read
	// ----------------------------------------------------------------------
	SendCommand(0x80,0x01); // Frequency = 0xAA,0x07,0xA3,0x80,0x01,0xE3,0x48
	// ----------------------------------------------------------------------

	if (EnergyBoard_Serial.available()) {

		byte Header					= EnergyBoard_Serial.read();
		byte Recieved_Byte_Count	= EnergyBoard_Serial.read();
		byte Data1					= EnergyBoard_Serial.read();
		byte Data2					= EnergyBoard_Serial.read();
		byte Data3					= EnergyBoard_Serial.read();
		byte CheckSum				= EnergyBoard_Serial.read();

		// Control Recieved Data
		if (Header == 0xAA) {
                Frequency_ = (Data3 * 65536 + Data2 * 256 + Data1);
            } // Acknowledge with data.
		if (Header == 0xAD) {
			return -1001;
		} // Acknowledge without data. (Error: -1001)
		if (Header == 0xB0) {
			return -1002;
		} // Negative Acknowledge (NACK). (Error: -1002)
		if (Header == 0xBC) {
			return -1003;
		} // Command not implemented. (Error: -1003)
		if (Header == 0xBD) {
			return -1004;
		} // Checksum failed. (Error: -1004)
		
		if (Gain) {
                Frequency_ = Frequency_ * Frequency_Gain;
            } // Gain Correction
	}
	
    EnergyBoard_Serial.end();
    return abs(Frequency_);
}
float EnergyBoard::IC_Temperature(void) {
    
    EnergyBoard_Serial.begin(BoudRate);
    delay(10);
    
    float Temperature_ = 0;
    
    if (ClearBuffer() == true) {
        SendCommand(0x74,0x01);
        
        if (EnergyBoard_Serial.available()) {
            
            byte Header					= EnergyBoard_Serial.read();
            byte Recieved_Byte_Count	= EnergyBoard_Serial.read();
            byte Data1					= EnergyBoard_Serial.read();
            byte Data2					= EnergyBoard_Serial.read();
            byte Data3					= EnergyBoard_Serial.read();
            byte CheckSum				= EnergyBoard_Serial.read();
            
            // Control Recieved Data
            if (Header == 0xAA) {
                Temperature_ = (Data3 * 65536 + Data2 * 256 + Data1);
            } // Acknowledge with data.
			if (Header == 0xAD) {
				return -1001;
			} // Acknowledge without data. (Error: -1001)
			if (Header == 0xB0) {
				return -1002;
			} // Negative Acknowledge (NACK). (Error: -1002)
			if (Header == 0xBC) {
				return -1003;
			} // Command not implemented. (Error: -1003)
			if (Header == 0xBD) {
				return -1004;
			} // Checksum failed. (Error: -1004)
			
            if (Gain) {
                Temperature_ = Temperature_ / 1000;
            } // Gain Correction
        }
    }
    
    EnergyBoard_Serial.end();
    return Temperature_;
}

// Private Functions
bool EnergyBoard::ClearBuffer(void) {
    
    EnergyBoard_Serial.flush();
    while(EnergyBoard_Serial.available() > 0) char _t = EnergyBoard_Serial.read();
    delay(20);
    return true;
}
bool EnergyBoard::SendCommand(int CHR1, int CHR2) {
	
/*
	MAX78630 Serial Comminication Read Values Structure
	---------------------------------------------------
	Read Request : [1]-[2]-[3]-[4]-[5]-[6]-[7]
	---------------------------------------------------
	1. byte is the IC address byte (0xAA)
	2. byte is the total sended byte (0x07)
	3. byte is the package payload type (0xA3)
	4. byte is the request (RMS) byte (2)
	5. byte is the request (RMS) byte (1)
	6. byte is the requested byte count (0xE3)
	7. byte is the CRC correction byte (CHK)
*/

    int CheckSum = 0x100 - ((0xAA + 0x07 + 0xA3 + CHR1 + CHR2 + 0xE3) % 256); // Calculate checksum
    
    EnergyBoard_Serial.write(0xAA);
    EnergyBoard_Serial.write(0x07);
    EnergyBoard_Serial.write(0xA3);
    EnergyBoard_Serial.write(CHR1);
    EnergyBoard_Serial.write(CHR2);
    EnergyBoard_Serial.write(0xE3);
    EnergyBoard_Serial.write(CheckSum);
    delay(10);
    
    return true;
}