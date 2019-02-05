/*  Haptic DRV2605 - Haptic Driver for Dialog DRV2605
    Copyright (C) 2018,2019 PatternAgents, LLC

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <Wire.h>
#include "Haptic_DRV2605.h"

//! haptic_reset[] - DRV2605 reset & basic setup :
//! (Actuator independent setup commands)
const struct scr_type haptic_reset[] = {
	{DRV2605_REG_MODE,          0x80}, //! DRV2605 - reset 
	{ACTUATOR_SCRIPT_DELAY,     0x50}, //! DRV2605 - delay 50mSec for Reset? no spec? 
	{ACTUATOR_SCRIPT_END,       0x00}  //! DRV2605 - end of script flag
};
#define SCRIPT_RESET 0

//! haptic_LRA_basic[] - DRV2605 setup for LRA coin types :
//! register load table for basic device/actuator initialization 
//! (Actuator dependent setup commands with auto-calibration)
const struct scr_type haptic_LRA_basic[] = {
	{DRV2605_REG_MODE,          0x07}, //! DRV2605 - Calibrate Mode! 
	{DRV2605_REG_FEEDBACK,      0xAA}, //! DRV2605 - LRA mode
	{DRV2605_REG_CONTROL3,      0x20}, //! DRV2605 - LRA closed loop
	{DRV2605_REG_RATEDV,        0x82}, //! DRV2605 - rated voltage 3V
	{DRV2605_REG_CLAMPV,        0x96}, //! DRV2605 - clamp voltage = 3.3V
	{DRV2605_REG_CONTROL4,      0x30}, //! DRV2605 - Autocal time = 3 (1.2 seconds!)
	{DRV2605_REG_CONTROL1,      0x83}, //! DRV2605 - drive_time set for 128Hz = 3.9mSec
	{DRV2605_REG_CONTROL2,      0xF5}, //! DRV2605 - sample_time = 3, balnk =1, idiss = 1
	{DRV2605_REG_LIBRARY,       0x06}, //! DRV2605 - Library 6 is LRA
	{DRV2605_REG_WAVESEQ1,         1}, //! DRV2605 - strong click
	{DRV2605_REG_WAVESEQ2,         0}, //! DRV2605 - strong click
	{DRV2605_REG_OVERDRIVE,        0}, //! DRV2605 - no overdrive
	{DRV2605_REG_SUSTAINPOS,       0}, //! DRV2605 - Sustain Time Offset 
	{DRV2605_REG_SUSTAINNEG,       0}, //! DRV2605 - Sustain Time Offset
	{DRV2605_REG_BRAKE,            0}, //! DRV2605 - Braking Time Offset
	{DRV2605_REG_RTPIN,         0x00}, //! DRV2605 - real-time-playback zero
	{DRV2605_REG_AUDIOVIBECTRL, 0x00}, //! DRV2605 - audio to vibe control   (0x00) 10ms, 100hz
	{DRV2605_REG_AUDIOMINLVL,   0x00}, //! DRV2605 - audio-to-vibe min level 
	{DRV2605_REG_AUDIOMAXLVL,   0x40}, //! DRV2605 -  audio-to-vibe max level
	{DRV2605_REG_AUDIOMINDRV,   0x20}, //! DRV2605 - audio-to-vibe min drive
	{DRV2605_REG_AUDIOMAXDRV,   0x64}, //! DRV2605 - audio-to-vibe max drive
	{DRV2605_REG_GO,            0x01}, //! DRV2605 - trigger a calibrate cycle
	{ACTUATOR_SCRIPT_DELAY,     0xFF}, //! DRV2605 - delay 0.25 sec 
	{ACTUATOR_SCRIPT_DELAY,     0xFF}, //! DRV2605 - delay 0.25 sec 
	{ACTUATOR_SCRIPT_DELAY,     0xFF}, //! DRV2605 - delay 0.25 sec 
	{ACTUATOR_SCRIPT_DELAY,     0xFF}, //! DRV2605 - delay 0.25 sec 
	{ACTUATOR_SCRIPT_DELAY,     0xFF}, //! DRV2605 - delay 0.25 sec (1.25 sec max)
	{DRV2605_REG_MODE,          0x00}, //! DRV2605 - calibrate 
	{ACTUATOR_SCRIPT_END,       0x00}  //! DRV2605 - end of script flag
};
#define SCRIPT_LRA_BASIC 1

//! haptic_ERM_basic[] - DRV2605 setup for ERM types :
//! register load table for basic device/actuator initialization 
//! (Actuator dependent setup commands)
const struct scr_type haptic_ERM_basic[] = {
	{DRV2605_REG_MODE,          0x00}, //! DRV2605 - out of standby 
	{DRV2605_REG_LIBRARY,       0x01}, //! DRV2605 - Library A
	{DRV2605_REG_RTPIN,         0x00}, //! DRV2605 - no real-time-playback
	{DRV2605_REG_WAVESEQ1,         1}, //! DRV2605 - strong click
	{DRV2605_REG_WAVESEQ2,         0}, //! DRV2605 - strong click
	{DRV2605_REG_OVERDRIVE,        0}, //! DRV2605 - no overdrive
	{DRV2605_REG_SUSTAINPOS,       0}, //! DRV2605 - Sustain Time Offset 
	{DRV2605_REG_SUSTAINNEG,       0}, //! DRV2605 - Sustain Time Offset
	{DRV2605_REG_BRAKE,            0}, //! DRV2605 - Braking Time Offset
	{DRV2605_REG_AUDIOVIBECTRL, 0x00}, //! DRV2605 - audio to vibe control   (0x00) 10ms, 100hz
	{DRV2605_REG_AUDIOMINLVL,   0x00}, //! DRV2605 - audio-to-vibe min level 
	{DRV2605_REG_AUDIOMAXLVL,   0x40}, //! DRV2605 -  audio-to-vibe max level
	{DRV2605_REG_AUDIOMINDRV,   0x20}, //! DRV2605 - audio-to-vibe min drive
	{DRV2605_REG_AUDIOMAXDRV,   0x64}, //! DRV2605 - audio-to-vibe max drive
	{DRV2605_REG_FEEDBACK,      0x36}, //! DRV2605 - feedback tuning
	{DRV2605_REG_CONTROL3,      0x20}, //! DRV2605 - ERM open loop
	{ACTUATOR_SCRIPT_END,       0x00}  //! DRV2605 - end of script flag
};
#define SCRIPT_ERM_BASIC 2

//! haptic_ERM_coin[] - DRV2605 setup for ERM Disc/Coin types :
//! register load table for basic device/actuator initialization 
//! (Actuator dependent setup commands)
const struct scr_type haptic_ERM_coin[] = {
	{DRV2605_REG_MODE,          0x00}, //! DRV2605 - out of standby 
	{DRV2605_REG_LIBRARY,       0x01}, //! DRV2605 - Library B
	{DRV2605_REG_RTPIN,         0x00}, //! DRV2605 - no real-time-playback
	{DRV2605_REG_WAVESEQ1,         1}, //! DRV2605 - strong click
	{DRV2605_REG_WAVESEQ2,         0}, //! DRV2605 - strong click
	{DRV2605_REG_OVERDRIVE,        0}, //! DRV2605 - no overdrive
	{DRV2605_REG_SUSTAINPOS,       0}, //! DRV2605 - Sustain Time Offset 
	{DRV2605_REG_SUSTAINNEG,       0}, //! DRV2605 - Sustain Time Offset
	{DRV2605_REG_BRAKE,            0}, //! DRV2605 - Braking Time Offset
	{DRV2605_REG_AUDIOVIBECTRL, 0x00}, //! DRV2605 - audio to vibe control   (0x00) 10ms, 100hz
	{DRV2605_REG_AUDIOMINLVL,   0x00}, //! DRV2605 - audio-to-vibe min level 
	{DRV2605_REG_AUDIOMAXLVL,   0x40}, //! DRV2605 -  audio-to-vibe max level
	{DRV2605_REG_AUDIOMINDRV,   0x20}, //! DRV2605 - audio-to-vibe min drive
	{DRV2605_REG_AUDIOMAXDRV,   0x64}, //! DRV2605 - audio-to-vibe max drive
	{DRV2605_REG_FEEDBACK,      0x36}, //! DRV2605 - feedback tuning
	{DRV2605_REG_CONTROL3,      0x20}, //! DRV2605 - ERM open loop
	{ACTUATOR_SCRIPT_END,       0x00}  //! DRV2605 - end of script flag
};
#define SCRIPT_ERM_COIN 3

//! array of pointers to arrays of FX scripts...
const scr_type* scriptList[ACTUATOR_SCRIPT_MAX];

//! Define a structure to hold the actuator instance data for the haptic driver
static struct haptic_driver actuator;

/**************************************************************************/
/*! 
    @brief  Instantiate a new DRV2605 class
	        Use -1 for any unused pin assignments
			i.e. only GPIO0 for PWM on Pin #5, no Interrupts
			Haptic_DRV2605 actuator( -1, 5, -1, -1);
*/
Haptic_DRV2605::Haptic_DRV2605() {
  // I2C Only
  _gp0_pin = -1;
}

Haptic_DRV2605::Haptic_DRV2605(int8_t gp0_pin) {
  // I2C with Multi-Input GPI0 Pin
  _gp0_pin = gp0_pin;
}

/********************************************************************/

int Haptic_DRV2605::readReg(uint8_t reg) {
  uint8_t dada;
    Wire.beginTransmission(Haptic_I2C_Address);
    Wire.write((byte)reg);
    Wire.endTransmission();
    Wire.requestFrom((byte)Haptic_I2C_Address, (byte)1);
    dada = Wire.read();
  return((int)dada);  //! a career in dada processing!
}

int Haptic_DRV2605::writeReg(uint8_t reg, uint8_t dada) {
    Wire.beginTransmission(Haptic_I2C_Address);
    Wire.write((byte)reg);
    Wire.write((byte)dada);
    Wire.endTransmission();
	return(SUCCESS);
}

int Haptic_DRV2605::writeRegBits(uint8_t reg, uint8_t mask, uint8_t bits) {
	uint8_t val = readReg(reg);
	if (val < 0) return(FAIL);
	val = val & ~mask; val |= bits;
	return(writeReg(reg, val));
}

int Haptic_DRV2605::writeRegBulk(uint8_t reg, uint8_t *dada, uint8_t size) {
	for (int i = 0; i < size; i++) {
		int ret = writeReg(reg + i, *dada++);
		if (ret != 0) return(FAIL);
	}
	return(SUCCESS);
}

int Haptic_DRV2605::writeRegScript(const struct scr_type script[])
{
	for (int i = 0; script[i].reg != ACTUATOR_SCRIPT_END; i++) {
		if (script[i].reg == ACTUATOR_SCRIPT_DELAY) {
			delay(script[i].val);
		} else if (writeReg(script[i].reg, script[i].val)) {
			Serial.println("ERROR DRV2605: writeRegScript failure\n");
			return(FAIL);
		}
	}
	return(SUCCESS);
}

int Haptic_DRV2605::getDeviceID(void) {
  int id = (readReg(DRV2605_REG_STATUS)& 0xE0 ) >> 5;
  return(id); // read chip ID register
}

/**************************************************************************/
/*! 
    @brief  Initialize the DRV2605 Controller
*/
int Haptic_DRV2605::begin() {
  // _gp0_pin (IN/TRIG) can be a GPIO trigger (default), PWM output or an Audio Output
  if (_gp0_pin >= 0){
	  pinMode(_gp0_pin, OUTPUT);
  }
  //! initialize an array of pointers to the scripts 
  scriptList[SCRIPT_RESET]     =  haptic_reset;       //! 'reset' script always at index zero
  scriptList[SCRIPT_LRA_BASIC] =  haptic_LRA_basic;   //! LRA coin basic setup
  scriptList[SCRIPT_ERM_BASIC] =  haptic_ERM_basic;   //! ERM (bar) basic setup
  scriptList[SCRIPT_ERM_COIN]  =  haptic_ERM_coin;    //! ERM (coin) basic setup
  actuator.dev_scripts_max = SCRIPT_ERM_COIN + 1;

  Wire.begin();                             //! start the I2C interface
  playScript(SCRIPT_RESET);                 //! play the reset script
  int haptic_DeviceID = getDeviceID();      //! verify that it is a DRV2605L
  if (haptic_DeviceID != HAPTIC_CHIP_ID) {
    return (FAIL);
  } else {
    return(SUCCESS);
  }
}

/**************************************************************************/
/*!
    Settings and control API's
*/
int Haptic_DRV2605::setActuatorType(enum haptic_dev_t type) {
	if (type >= HAPTIC_DEV_MAX) {
		return (FAIL);
	}
    actuator.dev_type = type;
	switch (actuator.dev_type)
	{
	  case LRA :
		// setup for LRA Type
	    playScript(SCRIPT_LRA_BASIC); //! the LRA "setup"/init script
		break;
	  case ERM :
		// setup for ERM type
	    playScript(SCRIPT_ERM_BASIC); //! the ERM "setup"/init script
		break;
	  case ERM_COIN :
		// setup for ERM coin type
	    playScript(SCRIPT_ERM_COIN);  //! the ERM Disc "setup"/init script
	    break;
	  case ERM_DMA :
        // setup for ERM Dual Mode type
	    playScript(SCRIPT_ERM_BASIC); //! the ERM "setup"/init script
	    break;
	  case LRA_DMA :
		// setup for LRA Dual Mode type
	    playScript(SCRIPT_LRA_BASIC); //! the LRA "setup"/init script
	    break;
	  default:
		break;
	}
    return(SUCCESS);
}

int Haptic_DRV2605::writeWaveform(uint8_t reg, uint8_t *wave, uint8_t size) {
		Serial.println("ERROR DRV2605: Waveforms are FIXED in ROM, not changeable \n");
		return(FAIL);
}

int Haptic_DRV2605::readWaveform(uint8_t reg, uint8_t *wave, uint8_t size) {
		Serial.println("ERROR DRV2605: Waveforms are FIXED in ROM, not readable \n");
		return(FAIL);
}

int Haptic_DRV2605::setWaveform(uint8_t slot, uint8_t wave) {
	if (slot > 8) {
		Serial.println("ERROR DRV2605: Invalid Parameter - Only 8 Slots \n");
		return(FAIL);
	}
    writeReg(DRV2605_REG_WAVESEQ1+slot, wave);
  return(SUCCESS);
}

int Haptic_DRV2605::setWaveformLib(uint8_t lib){
	if (lib > 8) {
		Serial.println("ERROR DRV2605: Invalid Parameter - Only 7 ROM Waveform Libraries \n");
		return(FAIL);
	}
	writeReg(DRV2605_REG_LIBRARY, lib);
	return(SUCCESS);
}

int Haptic_DRV2605::setScript(int num) {
	if (num < 0 || num > actuator.dev_scripts_max) {
		return(FAIL);
	}
	actuator.dev_script = num;
  return(SUCCESS);
}

int Haptic_DRV2605::getScripts(void) {
  return(actuator.dev_scripts_max);
}

int Haptic_DRV2605::playScript(int num) {
	if (num < 0 || num > actuator.dev_scripts_max) {
		return(FAIL);
	}
	actuator.dev_script = num;
    return(writeRegScript(scriptList[num]));
}

int Haptic_DRV2605::go(void) {
  writeReg(DRV2605_REG_GO, 1);
  return(SUCCESS);
}

int Haptic_DRV2605::stop(void) {
  writeReg(DRV2605_REG_GO, 0);
  return(SUCCESS);
}

/*!
    @brief setMode(enum op_mode mode) - set operating mode
*/
int Haptic_DRV2605::setMode(enum op_mode mode)
{
	if (mode < 0 || mode >= HAPTIC_MODE_MAX) {
		return(FAIL);
	}
	actuator.op_mode = mode;

	switch (actuator.op_mode)
	{
	  case INACTIVE_MODE :
		// setup for inactive/software standby mode
	    writeReg(DRV2605_REG_MODE, DRV2605_MODE_STANDBY);
		break;
	  case STREAM_MODE :
		// setup for Direct Write/Realtime modes
	    writeReg(DRV2605_REG_MODE, DRV2605_MODE_REALTIME);
		break;
	  case PWM_MODE :
		// PWM input (GPIO-0) controls vibration
	    writeReg(DRV2605_REG_MODE, DRV2605_MODE_PWMANALOG);
	    break;
	  case REGISTER_MODE :
		// register triggered playback
	    writeReg(DRV2605_REG_MODE, DRV2605_MODE_INTTRIG);
	    break;
	  case GPIO_MODE :
		// GPIO controlled playback
	    // TODO: add edge/level and polarity setting/logic
	    writeReg(DRV2605_REG_MODE, DRV2605_MODE_EXTTRIGLVL); 
	    break;
	  // "modes" above DRV2605_OPERATION_MODE_MASK are "soft" modes
	  case SLEEP_MODE :
		// setup for low power/sleep mode
	    writeReg(DRV2605_REG_MODE, DRV2605_MODE_STANDBY);
	    break;
	  default:
		break;
	}

	return(SUCCESS);
}

int Haptic_DRV2605::setRealtimeValue(uint8_t rtp) {
  return(SUCCESS);
}

