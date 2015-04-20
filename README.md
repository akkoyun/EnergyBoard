## Description
	The EnergyBoard is an energy measurement processor for polyphase power-monitoring 
	systems. It is designed for real-time monitoring for a variety of typical three-phase 
	configurations in industrial applications. The MAX78630 provides up to six analog 
	inputs for interfacing to voltage and current sensors. Scaled voltages from the 
	sensors are fed to the single converter front-end using a high-resolution delta-sigma 
	converter. Supported current sensors include current transformers (CTs), Rogowski 
	coils, and resistive shunts.  
	
	In this Arduino Library we can read all data of energy parameters.

![EB_Block](https://github.com/x2bus/EnergyBoard/blob/master/extras/EnergyBoard_Blok.jpg?raw=true)

## RMS Voltage

The EnergyBoard reports true RMS measurements for each input. An RMS value is 
obtained by performing the sum of the squares of instantaneous values over a time 
interval (accumulation interval) and then performing a square root of the result 
after dividing by the number of samples in the interval.

![RMS Voltage](http://upload.wikimedia.org/wikipedia/commons/thumb/8/89/Sine_wave_voltages.svg/530px-Sine_wave_voltages.svg.png)  
`Vrms : Root Mean Square of Voltage`  
`Vpk : Peak Voltage`  
`Vpp : Peak to Peak Voltage`  
	
	Syntax
	EnergyBoard.Voltage(char phase);
	
	Parameters
	phase : Phase of voltage (char) for example 'R', 'S', 'T'
	
	Returns
	Voltage value in float data type
	
	Example
	Float Voltage_R = EnergyBoard.Voltage('R');
	
## RMS Current

The EnergyBoard reports true RMS measurements for current inputs. The RMS current is 
obtained by performing the sum of the squares of the instantaneous voltage samples 
over the accumulation interval and then performing a square root of the result after 
dividing by the number of samples in the interval.

	Syntax
	EnergyBoard.Current(char phase);
	
	Parameters
	phase : Phase of current (char) for example 'R', 'S', 'T'
	
	Returns
	Current value in float data type
	
	Example
	Float Current_R = EnergyBoard.Current('R');

## Active Power

The instantaneous power results (PA, PB, PC) are obtained by multiplying aligned instantaneous 
voltage and current samples.

![Active Power](http://www.electronics-tutorials.ws/accircuits/acp29.gif?74587b)

	Syntax
	EnergyBoard.ActivePower(char phase);
	
	Parameters
	phase : Phase of power (char) for example 'R', 'S', 'T'
	
	Returns
	Active Power value in float data type
	
	Example
	Float ActivePower_R = EnergyBoard.ActivePower('R');

## ReActive Power

Instantaneous reactive power results are calculated by multiplying the instantaneous samples 
of current and the instantaneous quadrature voltage. 

![ReActive Power](http://www.blc.lsbu.ac.uk/eservices/Module1/Graphics/PF3.gif)

	Syntax
	EnergyBoard.ReactivePower(char phase);
	
	Parameters
	phase : Phase of power (char) for example 'R', 'S', 'T'
	
	Returns
	ReActive Power value in float data type
	
	Example
	Float ReactivePower_R = EnergyBoard.ReactivePower('R');

## Apparent Power

The apparent power, also referred as Volt-Amps, is the product of low rate RMS voltage and 
current results. Offsets applied to RMS current will affect apparent power results.

	Syntax
	EnergyBoard.ApparentPower(char phase);
	
	Parameters
	phase : Phase of power (char) for example 'R', 'S', 'T'
	
	Returns
	Apparent Power value in float data type
	
	Example
	Float ApparentPower_R = EnergyBoard.ApparentPower('R');
  
## PowerFactor (Cosfi)

The apparent power, also referred as Volt-Amps, is the product of low rate RMS voltage 
and current results. Offsets applied to RMS current will affect apparent power results.

![Cos Fi](http://www.practicalmachinist.com/vb/attachments/f11/35192d1307402684-calc-power-consumption-elect-meter-power-triangle.jpg)  

	Syntax
	EnergyBoard.PowerFactor(char phase);
	
	Parameters
	phase : Phase of power factor (char) for example 'R', 'S', 'T'
	
	Returns
	PowerFactor value in float data type
	
	Example
	Float PowerFactor_R = EnergyBoard.PowerFactor('R');

## Frequency

	Syntax
	EnergyBoard.Frequency(char phase);
	
	Parameters
	None
	
	Returns
	Frequency value in float data type
	
	Example
	Float Frequency = EnergyBoard.Frequency();

## Licences

All source code is licensed under the [MIT License](http://opensource.org/licenses/MIT)

	Copyright (c) 2015 X2bus <info@x2bus.com>
	 
	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is furnished
	to do so, subject to the following conditions:
	 
	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.
	 
	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
	THE SOFTWARE.

## Version History
**1.0.2**  
* IC Startup commands converted to HEX format.
* Voltage commands converted to HEX format.
* CommandSend function commands converted to HEX format.
* 'print' functions converted to 'write'
* Checksum equation converted to HEX
