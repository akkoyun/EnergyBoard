/* *******************************************************************************
 *
 *  Copyright (C) 2014-2015 X2Bus (info@x2bus.com)
 *  Can not be copied and/or distributed without the express permission of X2Bus
 *
 *	The library is compatable with Arduino and the Open Hardware Community.
 *
 *  Library		: X2Bus Energy Board (MAX78630)
 *  Developer	: Mehmet Gunce Akkoyun (gunce.akkoyun@x2bus.com)
 *  GitHub		: https://github.com/x2bus/EnergyBoard
 *  Revision	: 1.0.1
 *	Relase		: April 2015
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
	MAX_Serial.print(char(170));	// Header (0xAA)
	MAX_Serial.print(char(04));		// Total Sended Byte (0x04)
	MAX_Serial.print(char(196));	// Setting Command (0xC4)
	MAX_Serial.print(char(142));	// CheckSum (0x8E)
    MAX_Serial.end();

	if (Gain != 0) MAX78630_Gain = 1;
}

float MAX78630::Voltage(char Phase) {

    MAX_Serial.begin(Serial_BoudRate);
	delay(10);

    float Voltage_ = 0;

    if (ClearBuffer() == true) {

		// RMS Voltage Read
		// ----------------------------------------------------------------------------------------------
		if (Phase == 'R') SendCommand(144,0); // RMS Voltage Phase R = 0xAA,0x07,0xA3,0x90,0x00,0xE3,0x39
		if (Phase == 'S') SendCommand(147,0); // RMS Voltage Phase S = 0xAA,0x07,0xA3,0x93,0x00,0xE3,0x36
		if (Phase == 'T') SendCommand(150,0); // RMS Voltage Phase T = 0xAA,0x07,0xA3,0x96,0x00,0xE3,0x33
		// ----------------------------------------------------------------------------------------------


		if (MAX_Serial.available()) {

			byte Header					= MAX_Serial.read();
			byte Recieved_Byte_Count	= MAX_Serial.read();
			byte Data1					= MAX_Serial.read();
			byte Data2					= MAX_Serial.read();
			byte Data3					= MAX_Serial.read();
			byte CheckSum				= MAX_Serial.read();

			// Control Recieved Data
			if (Header == 170) {
                Voltage_ = (Data3 * 65536 + Data2 * 256 + Data1);
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
                Voltage_ = Voltage_ * MAX78630_Voltage_Gain;
            } // Gain Correction
		}
	}
    
    MAX_Serial.end();
    return Voltage_;
}
float MAX78630::Current(char Phase) {

    MAX_Serial.begin(Serial_BoudRate);
	delay(10);

	float Current_ = 0;

    if (ClearBuffer() == true) {

		// RMS Current Read
		// ----------------------------------------------------------------------------------------------
		if (Phase == 'R') SendCommand(213,0); // RMS Current Phase R = 0xAA,0x07,0xA3,0xD5,0x00,0xE3,0xF4
		if (Phase == 'S') SendCommand(216,0); // RMS Current Phase S = 0xAA,0x07,0xA3,0xD8,0x00,0xE3,0xF1
		if (Phase == 'T') SendCommand(219,0); // RMS Current Phase T = 0xAA,0x07,0xA3,0xDB,0x00,0xE3,0xEE
		// ----------------------------------------------------------------------------------------------

		if (MAX_Serial.available()) {
			
			byte Header					= MAX_Serial.read();
			byte Recieved_Byte_Count	= MAX_Serial.read();
			byte Data1					= MAX_Serial.read();
			byte Data2					= MAX_Serial.read();
			byte Data3					= MAX_Serial.read();
			byte CheckSum				= MAX_Serial.read();
			
			// Control Recieved Data
			if (Header == 170) {
				Current_ = (Data3 * 65536 + Data2 * 256 + Data1);
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
				Current_ = Current_ * MAX78630_Current_Gain;
			} // Gain Correction
		}
	}
	
	MAX_Serial.end();
	return Current_;
}
float MAX78630::ActivePower(char Phase) {

    MAX_Serial.begin(Serial_BoudRate);

	float ActivePower_ = 0;
	byte inBuffer[6];
	int bufIndx = 0;

	if (ClearBuffer() == true) {
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

	}
	else
	{
		ActivePower_ = 0;
	}

    MAX_Serial.end();
	return ActivePower_;
}
float MAX78630::ReactivePower(char Phase) {

    MAX_Serial.begin(Serial_BoudRate);

	float ReactivePower_ = 0;
	byte inBuffer[6];
	int bufIndx = 0;

	if (ClearBuffer() == true) {
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

	}
	else
	{
		ReactivePower_ = 0;
	}
	
    MAX_Serial.end();
	return ReactivePower_;
}
float MAX78630::ApparentPower(char Phase) {

    MAX_Serial.begin(Serial_BoudRate);

	float ApparentPower_ = 0;
	byte inBuffer[6];
	int bufIndx = 0;

	if (ClearBuffer() == true) {
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

	}
	else
	{
		ApparentPower_ = 0;
	}

	MAX_Serial.end();
	return ApparentPower_;
}
float MAX78630::PowerFactor(char Phase) {

    MAX_Serial.begin(Serial_BoudRate);
    delay(10);

	float PowerFactor_ = 0;

	if (ClearBuffer() == true) {
        
        // PowerFactor Read
        // ----------------------------------------------------------------------------------------------
        if (Phase == 'R') SendCommand(101,1); // PowerFactor Phase R = 0xAA,0x07,0xA3,0x65,0x01,0xE3,0x63
        if (Phase == 'S') SendCommand(104,1); // PowerFactor Phase S = 0xAA,0x07,0xA3,0x68,0x01,0xE3,0x60
        if (Phase == 'T') SendCommand(107,1); // PowerFactor Phase T = 0xAA,0x07,0xA3,0x6B,0x01,0xE3,0x5D
        // ----------------------------------------------------------------------------------------------

        
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
                PowerFactor_ = PowerFactor_ * MAX78630_Power_Factor_Gain;
            } // Gain Correction
        }
    }

	MAX_Serial.end();
	return PowerFactor_;
}
float MAX78630::Frequency(void) {

    MAX_Serial.begin(Serial_BoudRate);
	delay(10);
	
	float Frequency_ = 0;

    if (ClearBuffer() == true) {
        
        // Frequency Read
		// ----------------------------------------------------------------------------------------------
		SendCommand(128,1); // Frequency = 0xAA,0x07,0xA3,0x80,0x01,0xE3,0x48
		// ----------------------------------------------------------------------------------------------

		
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
	
	int CheckSum = 256 - ((170 + 7 + 163 + CHR1 + CHR2 + 227) % 256);
	
	MAX_Serial.print(char(170));			// (0xAA)
	MAX_Serial.print(char(7));				// (0x07)
	MAX_Serial.print(char(163));			// (0xA3)
	MAX_Serial.print(char(CHR1));			// (CHR2)
	MAX_Serial.print(char(CHR2));			// (CHR1)
	MAX_Serial.print(char(227));			// (0xE3)
	MAX_Serial.print(char(CheckSum));		// (CHK)
	
	delay(10);
	
	return true;
}