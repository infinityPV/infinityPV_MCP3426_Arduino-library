/**************************************************************************/
/*!
  @file     InfinityPV_MCP3426.cpp
  @author   rava (infinityPV ApS)
  @license  BSD (see license.txt)

  This is a library for all the infinityPV boards using the ADC/I2C converter
  MCP3426:
  - 1. Multiplatform LiBAT5V2A Boost
  ----> https://infinitypv.com/products/electronics

  InfinityPV makes printed organic solar cells. We invests part of our
  time and resources providing open source examples (hardware and code) to
  make easier the use of our solar cells in your projects.
  Printed organic solar cells are flexible, rollable and super light unlike
  other solar cell technologies, please visit our website to find more !

  (*)This libray is a modified version of MCP342X library which is
  protected by BSD license, Copyright 2013 by Chip Schnarel.

  @section  HISTORY

    v1.0  - First release Sep 2017
*/
/**************************************************************************/
#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include <Wire.h>

#include "InfinityPV_MCP3426.h"

/*	static float	stepSizeTbl[] = {
		0.001,		// 12-bit, 1X Gain
		0.0005,		// 12-bit, 2X Gain
		0.00025,	// 12-bit, 4X Gain
		0.000125,	// 12-bit, 8X Gain
		0.00025,	// 14-bit, 1X Gain
		0.000125,	// 14-bit, 2X Gain
		0.0000625,	// 14-bit, 4X Gain
		0.00003125,	// 14-bit, 8X Gain
		0.0000625,	// 16-bit, 1X Gain
		0.00003125,	// 16-bit, 2X Gain
		0.000015625,	// 16-bit, 4X Gain
		0.0000078125,	// 16-bit, 8X Gain
		};
*/

/******************************************
 * Default constructor, uses default I2C address.
 * @see MCP342X_DEFAULT_ADDRESS
 */
MCP342X::MCP342X() {
    devAddr = MCP342X_DEFAULT_ADDRESS;
}

/******************************************
 * Specific address constructor.
 * @param address I2C address
 * @see MCP342X_DEFAULT_ADDRESS
 * @see MCP342X_A0GND_A1GND, etc.
 */
MCP342X::MCP342X(uint8_t address) {
    devAddr = address;
}


/******************************************
 * Verify the I2C connection.
 * Make sure the device is connected and responds as expected.
 * @return True if connection is valid, false otherwise
 */
bool MCP342X::testConnection() {
    Wire.beginTransmission(devAddr);
    return (Wire.endTransmission() == 0);
}


/******************************************
 * Set the configuration shadow register for CH1
 * CH1 is configured at the board as an input
 * for measuring a external voltage signal
 * through a voltage divider RD/(RD+RUP) and
 * referenced to GND.
 * The function calculates the voltage divider ratio
 * and the GAIN that better fit the input range
 */
void MCP342X::setSettingsCH1(float v_max,float RUP, float RD) {
  uint8_t configData=0;
  VD_ratio=RD/(RD+RUP);
  if (v_max*VD_ratio<=0.256)
  {
    configData= configData|MCP342X_GAIN_8X;
    GAIN_CH1=8;
  }
  else
  {
    if(v_max*VD_ratio<=0.512)
    {
      configData= configData|MCP342X_GAIN_4X;
      GAIN_CH1=4;
    }
    else
    {
      if (v_max*VD_ratio<=1.024)
      {
        configData= configData|MCP342X_GAIN_2X;
        GAIN_CH1=2;
      }
      else
      {
        configData= configData|MCP342X_GAIN_1X;
        GAIN_CH1=1;
      }
    }
  }
  configData= configData|MCP342X_MODE_CONTINUOUS
  |MCP342X_CHANNEL_1|MCP342X_SIZE_16BIT;
  configRegShdw_CH1 = configData;
}


/******************************************
 * Set the configuration shadow register for CH2
 * CH2 is configured at the board as an input
 * for measuring a external current signal
 * through a shunt resistor RS1.
 * The function calculates the voltage divider ratio
 * and the GAIN that better fit the input range
 */
void MCP342X::setSettingsCH2(float i_max,float RS1) {
  uint8_t configData=0;
  RS=RS1;
  if (i_max*RS1<=0.256)
  {
    configData= configData|MCP342X_GAIN_8X;
    GAIN_CH2=8;
  }
  else
  {
    if(i_max*RS1<=0.512)
    {
      configData= configData|MCP342X_GAIN_4X;
      GAIN_CH2=4;
    }
    else
    {
      if (i_max*RS1<=1.024)
      {
        configData= configData|MCP342X_GAIN_2X;
        GAIN_CH2=2;
      }
      else
      {
        configData= configData|MCP342X_GAIN_1X;
        GAIN_CH2=1;
      }
    }
  }
  configData= configData|MCP342X_MODE_CONTINUOUS
  |MCP342X_CHANNEL_2|MCP342X_SIZE_16BIT;
  configRegShdw_CH2 = configData;
}


/******************************************
 * Get the configuration shadow register for the
 * specified channel
 */
uint8_t MCP342X::getConfigRegShdw(uint8_t channel) {
  switch (channel)
  {
  case 1:
  return configRegShdw_CH1;
  break;
  case 2:
  return configRegShdw_CH2;
  break;
  default:
  return configRegShdw_CH1;
  break;
  }

}

/******************************************
 * Start a conversion using configuration settings from
 *   the shadow configuration register for
 *  the specific channel
 */
bool MCP342X::startConversion(uint8_t channel) {
 uint8_t configRegShdw=0;
  switch (channel)
  {
    case 1:
    {
    configRegShdw=configRegShdw_CH1;
    }
    break;
    case 2:
    {
    configRegShdw=configRegShdw_CH2;
    break;
    }
    default:
    {
    configRegShdw=configRegShdw_CH1;
    }
    break;
  }
  Wire.beginTransmission(devAddr);
  Wire.write(configRegShdw | MCP342X_RDY);
  return (Wire.endTransmission() == 0);
}

/******************************************
 * Read the conversion value (12, 14 or 16 bit)
 *  Spins reading status until ready then
 *  fills in the supplied location with the 16-bit (two byte)
 *  conversion value and returns the status byte
 *  Note: status of -1 "0xFF' implies read error
 */
uint8_t MCP342X::getResult(uint8_t channel,int16_t *dataPtr) {
  uint8_t adcStatus;
  uint8_t configRegShdw=0;
   switch (channel)
   {
     case 1:
     {
     configRegShdw=configRegShdw_CH1;
     }
     break;
     case 2:
     {
     configRegShdw=configRegShdw_CH2;
     break;
     }
     default:
     {
     configRegShdw=configRegShdw_CH1;
     }
     break;
   }
  if((configRegShdw & MCP342X_SIZE_MASK) == MCP342X_SIZE_18BIT) {
    return 0xFF;
  }

  do {
     if(Wire.requestFrom(devAddr, (uint8_t) 3) == 3) {
       ((char*)dataPtr)[1] = Wire.read();
       ((char*)dataPtr)[0] = Wire.read();
       adcStatus = Wire.read();
     }
     else return 0xFF;
  } while((adcStatus & MCP342X_RDY) != 0x00);
  return adcStatus;
}

/******************************************
 * get the CH1 result and translate it
 * to the external Voltage to measure
 * according to the user configuration
 */

float MCP342X::getCH1Voltage_V(void){
  static int16_t  result;
  startConversion(1);
  getResult(1,&result);
  return (float) result*2048/(32768*GAIN_CH1)/(VD_ratio*1000);
}


/******************************************
 * get the CH2 result and translate it
 * to the external current to measure
 * according to the user configuration
 */

float MCP342X::getCH2Current_mA(void){
  static int16_t  result;
  startConversion(2);
  getResult(2,&result);
  return (float) result*2048/(32768*GAIN_CH2)/RS;
}


/******************************************
 * Check to see if the conversion value (12, 14 or 16 bit)
 *  is available.  If so, then
 *  fill in the supplied location with the 16-bit (two byte)
 *  conversion value and status the config byte
 *  Note: status of -1 "0xFF' implies read error
 */
uint8_t MCP342X::checkforResult(uint8_t channel, int16_t *dataPtr) {
  uint8_t adcStatus;
  uint8_t configRegShdw=0;
   switch (channel)
   {
     case 1:
     {
     configRegShdw=configRegShdw_CH1;
     }
     break;
     case 2:
     {
     configRegShdw=configRegShdw_CH2;
     break;
     }
     default:
     {
     configRegShdw=configRegShdw_CH1;
     }
     break;
   }
  if((configRegShdw & MCP342X_SIZE_MASK) == MCP342X_SIZE_18BIT) {
    return 0xFF;
  }

  if(Wire.requestFrom(devAddr, (uint8_t) 3) == 3) {
    ((char*)dataPtr)[1] = Wire.read();
    ((char*)dataPtr)[0] = Wire.read();
    adcStatus = Wire.read();
  }
  else return 0xFF;

  return adcStatus;
}


/******************************************
 * Read the conversion value (18 bit)
 *  Spins reading status until ready then
 *  fills in the supplied location (32 bit) with
 *  the 24-bit (three byte) conversion value
 *  and returns the status byte
 *  Note: status of -1 "0xFF' implies read error
 */
uint8_t MCP342X::getResult(uint8_t channel, int32_t *dataPtr) {
  uint8_t adcStatus;
  uint8_t configRegShdw=0;
   switch (channel)
   {
     case 1:
     {
     configRegShdw=configRegShdw_CH1;
     }
     break;
     case 2:
     {
     configRegShdw=configRegShdw_CH2;
     break;
     }
     default:
     {
     configRegShdw=configRegShdw_CH1;
     }
     break;
   }

  do {
     if(Wire.requestFrom((uint8_t) devAddr, (uint8_t) 4) == 4) {
       ((char*)dataPtr)[3] = Wire.read();
       ((char*)dataPtr)[2] = Wire.read();
       ((char*)dataPtr)[1] = Wire.read();
       adcStatus = Wire.read();
     }
     else return 0xFF;
  } while((adcStatus & MCP342X_RDY) != 0x00);
  *dataPtr = (*dataPtr)>>8;
  return adcStatus;
}


/******************************************
 * Check to see if the conversion value (18 bit)
 *  is available.  If so, then
 *  fill in the supplied location (32 bit) with
 *  the 24-bit (three byte) conversion value
 *  and return the status byte
 *  Note: status of -1 "0xFF' implies read error
 */
uint8_t MCP342X::checkforResult(uint8_t channel, int32_t *dataPtr) {
  uint8_t adcStatus;
  uint8_t configRegShdw=0;
   switch (channel)
   {
     case 1:
     {
     configRegShdw=configRegShdw_CH1;
     }
     break;
     case 2:
     {
     configRegShdw=configRegShdw_CH2;
     break;
     }
     default:
     {
     configRegShdw=configRegShdw_CH1;
     }
     break;
   }
  if((configRegShdw & MCP342X_SIZE_MASK) != MCP342X_SIZE_18BIT) {
    return 0xFF;
  }

  if(Wire.requestFrom((uint8_t) devAddr, (uint8_t) 4) == 4) {
    ((char*)dataPtr)[3] = Wire.read();
    ((char*)dataPtr)[2] = Wire.read();
    ((char*)dataPtr)[1] = Wire.read();
    adcStatus = Wire.read();
  }
  else return 0xFF;

  *dataPtr = (*dataPtr)>>8;
  return adcStatus;
}
