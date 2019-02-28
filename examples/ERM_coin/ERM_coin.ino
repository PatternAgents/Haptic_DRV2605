/*
 * ERM_Basic : Arduino IDE ERM_COIN example for TI DRV2605
 * 
 * Overview:
 * --------- 
 * This simple example for the ERM_COIN actuator types runs through 
 * all the effects in sequence.
 * 
 * Requirements:
 * -------------
 * PatternAgents DRV2605 library : https://github.com/PatternAgents/Haptic_DRV2605
 * 
 * Instructions:
 * -------------
 * 1) compile and Upload the sketch
 * 2) open the Serial Monitor
 *       
 *  Actuator Meta-Data :
 * 	Mfg        : Best Tong 
 * 	Model      : A0000051
 * 	Type       : ERM
 * 	Format     : Bar
 * 	Volatage   : 1.5V to 3.7V
 * 	RPM        : 7000 - 12000 ?(9000 RPM @  3.3 Volts)?
 * 	Dimensions : 0.45 x 3.15 x 3.85 inches (1.1 x 8.0 x 9.8 cm)
 * 
 * DRV2605 :  Immersion Libs Patented Waveform Libraries (under License with DRV2605)
 *
 * 	Library #   = Type / Nom Volts / Max Volts / Rise Time (mS) / Brake Time (mS)
 * 	Library   0 = Empty
 * 	Library A/1 = ERM  /  1.3V / 3.0V /  40 -  60 / 20 - 40
 * 	Library B/2 = ERM  /  3.0V / 3.0V /  40 -  60 / 5  - 15
 * 	Library C/3 = ERM  /  3.0V / 3.0V /  60 -  80 / 10 - 25
 * 	Library D/4 = ERM  /  3.0V / 3.0V / 100 - 140 / 15 - 25
 * 	Library E/5 = ERM  /  3.0V / 3.0V / 140 -     / 30 -
 * 	Library   6 = LRA  /       /      /           /
 * 	Library F/7 = ERM  /  4.5V / 5.0V /  35 -  45 / 10 - 20
 * 
 * ****************************************************************************** 
 */

#include "Haptic_DRV2605.h"
Haptic_DRV2605 haptic;             // Basic I2C instance - only SDA/SCL pins used
int           waveform = 0;        // current waveform
int           waveforms_max = 0;   // max waveforms available (we'll ask...)

void setup() {
  Serial.begin(115200);
   while (!Serial) {
    ; // wait for serial port to connect. Needed to see startup messages...
  }
  Serial.println("Haptic: ERM Coin Type Driver Basic Tests");
  Serial.println("Haptic: Connect I2C pins to WIRE Pins (SDA, SCL)");

  // initialize the Haptic controller
    if (haptic.begin() != HAPTIC_SUCCESS) {
      Serial.println("Haptic: Driver Error: DRV2605L Device not found - check your I2C connections?");
  } else {
      // okay correct chip, let's initialize it
      Serial.println("Haptic: DeviceID = DRV2605L @ Address 0x5A was found! ");
      // config the DRV2605 chip
      haptic.setActuatorType(ERM_COIN);              // pick an actuator type
      haptic.setMode(REGISTER_MODE);            // haptic effects triggered by I2C register write 
      waveforms_max = haptic.getWaveforms();    // how many waveforms available?
  }       
}

void loop() {
  Serial.print("Waveform #");                   // which waveform
  Serial.println(waveform);
  haptic.setWaveform(0, waveform);              // set the first sequence
  haptic.setWaveform(1, 0);                     // end the sequence
  haptic.goWait();				                      // play the waveform
  delay(100);					                          // wait for a while
  waveform++;					                          // next waveform
  if (waveform >= waveforms_max) waveform = 0;  // loop through all the waveforms
}
