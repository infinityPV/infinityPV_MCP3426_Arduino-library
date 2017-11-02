//////////////////////////////////////////////////////////////////////////
//                                                                          
// This is an example for using the Multiplatform LiBAT5V2A board   
// from infinityPV with Arduino.                                    
// The board contains 1 MCP3426 Analog to I2C converter, named IC1,             
// with two input channels:                      
// IC1_CH1: connected to a voltage divider (RUP & RD) to measure a 
// external voltage signal (VIN).
// IC1_CH2: connected to a shunt resistor to measure a external 
// current signal (IIN).
//
// The example allows you to configure and measure with  MCP3426
// on board according to your configuration and V/I range to measure.
// These values must be introduced in  2 functions during the setup: 
// - setSettingsCH1(VIN_MAX,RUP,RD), where VIN_MAX is the maximum 
// expected voltage in V at the external signal to measure, and RUP and RD
// are the values of the resistors at the voltage divider in kOhms.
//
// - setSettingsCH2(IIN_MAX,RS1), where IIN_MAX is the maximum 
// expected current in A at the external signal to measure, and RS1
// is the value of shunt resistor RS1 in Ohms.

// (!) Please consider that the maximum voltage input at CH1 and CH2 
// is 2.048V and any input voltage higher that will lead to overflow. 
// Therefore, please check that your configuration respects:
// - VIN_MAX*RD/(RUP+D)<= 2.048V at CH1
// - IIN_AX*RS1 <= 2.048V at CH2 
//
// setSettingsCH1 and setSettingsCH2 set the CONFIGURATION REGISTER at MCP3426.
// The function getConfigRegShdw()allows you to check the final value in this register.
// You do not have to worry about it, but if you are a pro, check the MCP3426
// datasheet for info about them. 
//
// Once calibrated you just have to use the functions:
// getCH1Voltage_V()and getCH2Current_mA() to get
// the measured values. 
//
// (*) The example uses the InfinityPV_MCP342X library, which is 
// a modification of the MCP342X library, which licensed 
// under BSD license. 
//
//////////////////////////////////////////////////////////////////////////////


#include  <InfinityPV_MCP3426.h>

// Instantiate objects used in this project
MCP342X IC1; 


void setup() {
  Wire.begin();  // join I2C bus
  TWBR = 12;  // SCL clock for I2C at 400 kHz (maximum)
  
  Serial.begin(9600); // Open serial connection to send info to the host
  while (!Serial) {}  // wait for Serial comms to become ready
  Serial.println("Starting up");
  Serial.println("Testing device connection...");
  Serial.println(IC1.testConnection() ? "IC1 connection successful" : "MCP342X connection failed");
  
  
  IC1.setSettingsCH1(60,510,18);      // set maximum voltage to measure (60V) and values for the used voltage divider (510k, and 18k)
  Serial.println(IC1.getConfigRegShdw(1), BIN);
  IC1.setSettingsCH2(0.1,1);          // set maximum current to measure (100mA) and value for the used shunt resistor (1 Ohm)
  Serial.println(IC1.getConfigRegShdw(2), BIN);
  
  
}  // End of setup()

void loop() {
  static int16_t  result;
  float IC1_Voltage_V;
  float IC1_Current_mA;
  
  IC1_Voltage_V=IC1.getCH1Voltage_V();
  IC1_Current_mA=IC1.getCH2Current_mA();
  
  Serial.print("Voltage IC1:");Serial.print(IC1_Voltage_V);Serial.println(" V");
  Serial.print("Current IC1:");Serial.print(IC1_Current_mA);Serial.println(" mA");
  
    delay(2000);
}  // End of loop()
