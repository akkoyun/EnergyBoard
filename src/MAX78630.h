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

#ifndef __MAX78630__
#define __MAX78630__

#include <Arduino.h>

// MAX78630 Setting
#define Serial_BoudRate					38400						// Serial Boud Rate (Default: 38400)

// Gain Multipliers
#define MAX78630_Voltage_Gain				0.0000793945940258412		// Voltage Gain Default
#define MAX78630_Current_Gain				0.00000596046186144157		// Current Gain Default
#define MAX78630_Power_Gain				0.00396962860118015			// Power Gain Default
#define MAX78630_Power_Factor_Gain			0.00000023841946872415		// Power Factor Gain Default
#define MAX78630_Frequency_Gain			0.0000152587672179127		// Frequency Gain Default

class MAX78630
{
public:
	MAX78630(int Gain);												// Start MAX78630 Startup Parameters
	float RMS_Voltage(char phase);									// Read RMS Voltage at selected phase
	float RMS_Voltage_AVR();										// Read RMS Voltage avarage
	float INST_Voltage(char phase);									// Read Instantaneous Voltage at selected phase
	float FUN_Voltage(char phase);									// Read Fundamental Voltage at selected phase
	float HARM_Voltage(char phase);									// Read Harmonic Voltage at selected phase
	float RMS_Voltage_MIN();										// Read RMS Voltage Mın Limit
	float RMS_Voltage_MAX();										// Read RMS Voltage Max Limit
	float RMS_Current(char phase);									// Read RMS Current at selected phase
	float RMS_Current_AVR();										// Read RMS Current Avarage
	float PEAK_Current(char phase);									// Read Peak Current at selected phase
	float INST_Current(char phase);									// Read Instantaneous Current at selected phase
	float FUN_Current(char phase);									// Read Fundamental Current at selected phase
	float HARM_Current(char phase);									// Read Harmonic Current at selected phase
	float RMS_Current_MAX();										// Read RMS Current Max Limit
	float ActivePower(char phase);									// Read Active Power at selected phase
	float ActivePower_AVR();										// Read Active Power Avarage
	float ReActivePower(char phase);								// Read ReActive Power at selected phase
	float ReActivePower_AVR();										// Read ReActive Power Avarage
	float ApparentPower(char phase);								// Read Apparent Power at selected phase
	float ApparentPower_AVR();										// Read ApparentPower Power Avarage
	float FundamentalPower(char phase);								// Read Fundamental Power at selected phase
	float HarmonicPower(char phase);								// Read Harmonic Power at selected phase
	float FundamentalVA(char phase);								// Read Fundamental Volt Amper at selected phase
	float PowerFactor(char phase);									// Read Power Factor at selected phase
	float PowerFactor_AVR();										// Read Power Factor Avarage
	float Frequency(void);											// Read Frequency of system
    float Temperature(void);                                        // Chip Temperature
private:
	bool ClearBuffer(void);											// Clear Serial Buffer Function (internal)
    bool SendCommand(int CHR1, int CHR2);                           // Send Command

    bool MAX78630_Gain;												// Gain Variable
};

#endif /* defined(__MAX78630__) */
