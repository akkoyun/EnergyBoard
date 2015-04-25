#include "MAX78630.h"

MAX78630* Energy = 0;

void setup() {
  Serial.begin(9600);
  Energy = new MAX78630(1);
}

void loop() {

  Serial.print("RMS Voltage : "); Serial.print(Energy->RMS_Voltage('R')); Serial.println(" V");
  Serial.print("RMS Voltage Avarage : "); Serial.print(Energy->RMS_Voltage_AVR()); Serial.println(" V");
  Serial.print("Instantaneous Voltage : "); Serial.print(Energy->INST_Voltage('R')); Serial.println(" V");
  Serial.print("Fundamental Voltage : "); Serial.print(Energy->FUN_Voltage('R')); Serial.println(" V");
  Serial.print("Harmonic Voltage : "); Serial.print(Energy->HARM_Voltage('R')); Serial.println(" V");
  Serial.print("RMS Voltage MIN Limit : "); Serial.print(Energy->RMS_Voltage_MIN()); Serial.println(" V");
  Serial.print("RMS Voltage MAX Limit : "); Serial.print(Energy->RMS_Voltage_MAX()); Serial.println(" V");
  Serial.print("RMS Current : "); Serial.print(Energy->RMS_Current('R')); Serial.println(" A");
  Serial.print("RMS Current Avarege: "); Serial.print(Energy->RMS_Current_AVR()); Serial.println(" A");
  Serial.print("PEAK Current : "); Serial.print(Energy->PEAK_Current('R')); Serial.println(" A");
  Serial.print("Instantaneous Current : "); Serial.print(Energy->INST_Current('R')); Serial.println(" A");
  Serial.print("Fundamental Current : "); Serial.print(Energy->FUN_Current('R')); Serial.println(" A");
  Serial.print("Harmonic Current : "); Serial.print(Energy->HARM_Current('R')); Serial.println(" A");
  Serial.print("RMS Current MAX Limit : "); Serial.print(Energy->RMS_Current_MAX()); Serial.println(" A");
  Serial.print("Active Power : "); Serial.print(Energy->ActivePower('R')); Serial.println(" W");
  Serial.print("Active Power Avarage : "); Serial.print(Energy->ActivePower_AVR()); Serial.println(" W");
  Serial.print("ReActive Power : "); Serial.print(Energy->ReActivePower('R')); Serial.println(" VAR");
  Serial.print("ReActive Power Avarage : "); Serial.print(Energy->ReActivePower_AVR()); Serial.println(" VAR");
  Serial.print("Apparent Power : "); Serial.print(Energy->ApparentPower('R')); Serial.println(" VA");
  Serial.print("Apparent Power Avarage : "); Serial.print(Energy->ApparentPower_AVR()); Serial.println(" VA");
  Serial.print("Fundamental Power : "); Serial.print(Energy->FundamentalPower('R')); Serial.println(" W");
  Serial.print("Harmonic Power : "); Serial.print(Energy->HarmonicPower('R')); Serial.println(" W");
  Serial.print("Fundamental Volt Amper : "); Serial.print(Energy->FundamentalVA('R')); Serial.println(" VA");
  Serial.print("Power Factor : "); Serial.print(Energy->PowerFactor('R')); Serial.println(" ");
  Serial.print("Power Factor Avarage : "); Serial.print(Energy->PowerFactor_AVR()); Serial.println(" ");
  Serial.print("Frequency : "); Serial.print(Energy->Frequency()); Serial.println(" Hz");
  Serial.print("IC Temperature : "); Serial.print(Energy->Temperature()); Serial.println(" C");
  Serial.println("------------------------------------------");

  delay(5000);
}

