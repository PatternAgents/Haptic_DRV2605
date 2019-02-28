/*
 * complex : Arduino IDE Scripts ERM example for TI DRV2605L
 * 
 * Overview:
 * --------- 
 * This simple example for the ERM actuator types runs through 
 * several complex (scripted) sequences. The Haptic_DRV2605 library
 * employs a script player that allows for register writes and also
 * includes codes for goWait and Delay, to create complex vibration 
 * sequences with many different waveforms. This example shows how 
 * to add to add your own user scripts to use with the library.
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
 * 
 * DRV2605 :  Immersion Libs Patented Waveform Libraries (under License with DRV2605L chip)
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
 *  The Haptic_DRV2605 library uses scripts to reset and initialize the DRV2605L controller,
 *  as well as calibration, configuration and sequence playback. The Default Scripts are :
 *  ----------------------------
 *  0 DA7280 Reset/Init DRV2605L
 *  1 DA7280 LRA Motor Init
 *  2 DA7280 ERM Motor Init
 *  3 DA7280 ERM_COIN Motor Init
 *  4 DA7280 LRA Motor Calibrate
 *  5 Complex Sequence 1
 *  6 Complex Sequence 2
 *  7 Complex Sequence 3
 *  ----------------------------
 *
 * (Index 5) Complex Sequence #1 
 * Eight(8) Sequence Pattern, Waveforms 1-8
 *
 * (Index 6) Complex Sequence #2
 * Sixteen(16) Sequence Pattern, Waveforms 1-16
 *
 * (Index 7) Complex Sequence #3 
 * Twenty Four(24) Sequence Pattern, misc/random Waveforms
 *
 * NOTE : For Calibration to function properly, 
 *        the Actuator should be attached to a 
 *        50-150 milliGram weighted base plate (a sled, or slug).
 *
 ******************************************************************************* 
 */

#include "Haptic_DRV2605.h"
Haptic_DRV2605 haptic;           // Basic I2C instance - only SDA/SCL pins used
int           script;        // current script
int           scripts_max;   // max scripts available

// create your own script of register write commands
// play sixteen waveform sequences, eight at a time
const struct scr_type my_script[] = {
	{DRV2605_REG_WAVESEQ1,        44}, //! DRV2605 - 
	{DRV2605_REG_WAVESEQ2,        45}, //! DRV2605 - 
	{DRV2605_REG_WAVESEQ3,        46}, //! DRV2605 - 
	{DRV2605_REG_WAVESEQ4,        47}, //! DRV2605 - 
	{DRV2605_REG_WAVESEQ5,        48}, //! DRV2605 - 
	{DRV2605_REG_WAVESEQ6,        49}, //! DRV2605 - 
	{DRV2605_REG_WAVESEQ7,        50}, //! DRV2605 - 
	{DRV2605_REG_WAVESEQ8,        51}, //! DRV2605 - 
	{ACTUATOR_SCRIPT_GOWAIT,    0x00}, //! DRV2605 - go and wait
	{ACTUATOR_SCRIPT_DELAY,     0xAA}, //! DRV2605 - delay a while
	{DRV2605_REG_WAVESEQ1,       110}, //! DRV2605 - 
	{DRV2605_REG_WAVESEQ2,       111}, //! DRV2605 - 
	{DRV2605_REG_WAVESEQ3,       112}, //! DRV2605 - 
	{DRV2605_REG_WAVESEQ4,       113}, //! DRV2605 - 
	{DRV2605_REG_WAVESEQ5,       114}, //! DRV2605 - 
	{DRV2605_REG_WAVESEQ6,       115}, //! DRV2605 - 
	{DRV2605_REG_WAVESEQ7,       116}, //! DRV2605 - 
	{DRV2605_REG_WAVESEQ8,       117}, //! DRV2605 - 
	{ACTUATOR_SCRIPT_GOWAIT,    0x00}, //! DRV2605 - go and wait
	{ACTUATOR_SCRIPT_END,       0x00}  //! DRV2605 - end of script flag
};

void setup() {
  Serial.begin(115200);
   while (!Serial) {
    ; // wait for serial port to connect. Needed to see startup messages...
  }
  Serial.println("Haptic: Add Custom Scripts Test");
  Serial.println("Haptic: Connect I2C pins to WIRE Pins (SDA, SCL)");

  // initialize the Haptic controller
    if (haptic.begin() != HAPTIC_SUCCESS) {
      Serial.println("Haptic: Driver Error: DRV2605L Device not found - check your I2C connections?");
  } else {
      // okay correct chip, let's initialize it
      Serial.println("Haptic: DeviceID = DRV2605L @ Address 0x5A was found! ");
      // config the DRV2605 chip
      haptic.setActuatorType(ERM);              // pick an actuator type
      haptic.setMode(REGISTER_MODE);            // haptic effects triggered by I2C register write 
      scripts_max = haptic.getScripts();        // how many total scripts available?
      haptic.addScript(scripts_max, my_script); // we'll add our own script
      scripts_max = haptic.getScripts();        // now, how many total scripts available?
      script = ACTUATOR_SCRIPT_COMPLEX;         // start at the complex ones
  }       
}

void loop() {
  Serial.print("Script #");                 // which script
  Serial.println(script);
  haptic.playScript(script);				        // play the script
  delay(1000);					                    // wait for a while
  script++;					                        // next script
  if (script >= scripts_max) script = ACTUATOR_SCRIPT_COMPLEX;
}
 
