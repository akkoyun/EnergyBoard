#include "MAX78630.h"

MAX78630* Energy = 0;

void setup() {
    Serial.begin(9600);
    Energy = new MAX78630(1);
}

void loop() {
    
    Serial.print("Voltage : "); Serial.print(Energy->Voltage('R')); Serial.println(" V");
    delay(10);
    Serial.print("Current : "); Serial.print(Energy->Current('R')); Serial.println(" A");
    delay(10);
    Serial.print("Active Power : "); Serial.print(Energy->ActivePower('R')); Serial.println(" W");
    delay(10);
    Serial.print("ReActive Power : "); Serial.print(Energy->ReactivePower('R')); Serial.println(" VAR");
    delay(10);
    Serial.print("Apparent Power : "); Serial.print(Energy->ApparentPower('R')); Serial.println(" VA");
    delay(10);
    Serial.print("CosFi : "); Serial.println(Energy->PowerFactor('R'));
    delay(10);
    Serial.print("Frequency : "); Serial.print(Energy->Frequency()); Serial.println(" Hz");
    delay(10);
    Serial.print("IC Temperature : "); Serial.print(Energy->Temperature()); Serial.println(" C");
    delay(10);
    
    Serial.println("------------------------------------------");

    delay(5000);
}

