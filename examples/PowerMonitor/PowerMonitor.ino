#include "MAX78630.h"

MAX78630* Energy = 0;

void setup() {
    Serial.begin(9600);
    Energy = new MAX78630(1);
}

void loop() {
    
    Serial.print("Voltaj R : "); Serial.print(Energy->Voltage('R')); Serial.println(" V");
    Serial.print("Voltaj S : "); Serial.print(Energy->Voltage('S')); Serial.println(" V");
    Serial.print("Voltaj T : "); Serial.print(Energy->Voltage('T')); Serial.println(" V");
    Serial.print("Akim R : "); Serial.print(Energy->Current('R')); Serial.println(" A");
    Serial.print("Akim S : "); Serial.print(Energy->Current('S')); Serial.println(" A");
    Serial.print("Akim T : "); Serial.print(Energy->Current('T')); Serial.println(" A");
    Serial.print("Aktif Guc R : "); Serial.print(Energy->ActivePower('R')); Serial.println(" W");
    Serial.print("Aktif Guc S : "); Serial.print(Energy->ActivePower('S')); Serial.println(" W");
    Serial.print("Aktif Guc T : "); Serial.print(Energy->ActivePower('T')); Serial.println(" W");
    Serial.print("ReAktif Guc R : "); Serial.print(Energy->ReactivePower('R')); Serial.println(" VAR");
    Serial.print("ReAktif Guc S : "); Serial.print(Energy->ReactivePower('S')); Serial.println(" VAR");
    Serial.print("ReAktif Guc T : "); Serial.print(Energy->ReactivePower('T')); Serial.println(" VAR");
    Serial.print("Gorunur Guc R : "); Serial.print(Energy->ApparentPower('R')); Serial.println(" VA");
    Serial.print("Gorunur Guc S : "); Serial.print(Energy->ApparentPower('S')); Serial.println(" VA");
    Serial.print("Gorunur Guc T : "); Serial.print(Energy->ApparentPower('T')); Serial.println(" VA");
    Serial.print("CosFi R : "); Serial.println(Energy->PowerFactor('R'));
    Serial.print("CosFi S : "); Serial.println(Energy->PowerFactor('S'));
    Serial.print("CosFi T : "); Serial.println(Energy->PowerFactor('T'));
    Serial.print("Frekans : "); Serial.print(Energy->Frequency()); Serial.println(" Hz");
    Serial.print("MAX Sicaklik : "); Serial.print(Energy->Temperature()); Serial.println(" C");
    
    Serial.println("------------------------------------------");

    delay(1000);
}

