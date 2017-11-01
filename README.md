infinityPV_MCP3426_Arduino-library
========================================

![LiBAT5V2A](https://github.com/infinityPV/infinityPV_MCP3426_Arduino-library/blob/master/extras/LiBAT5V2A%20picture1.jpg)

[*LiBAT5V2A Multiplatform *](https://www.infinitypv.com) 


This is an Arduino IDE library for using the MCP3426 ADC on board in the infinityPV LiBAT5V2A evaluation board, where the first channel measures an external voltage through a voltage divider and the second channel measures a current signal using a shunt resistor. However, you can use the library for your own designs including  MCP3426 ADCs :).  

Repository Contents
-------------------

* **/examples** - Example sketches for the library (.ino). Run these from the Arduino IDE. 
* **/extras** - Additional documentation for the user. These files are ignored by the IDE. 
* Source files for the library (.cpp, .h). 
* **library.properties** - General library properties for the Arduino package manager. 

Documentation
--------------
* **[LiBAT5V2A Brochure](https://infinitypv.com/images/infinityPV_OPV3W60V_applicationnote_Rev100.pdf)** - Short description for the LiBAT5V2A evaluation board. 
* **[LiBAT5V2A Application Note](https://infinitypv.com/images/infinityPV_OPV3W60V_applicationnote_Rev100.pdf)** - Application Note for the LiBAT52A evaluation board. Here you can find where is placed the MCP3426 ADC and how to configure the voltage divider and shunt resistor to get highest accuracy measuring for your application. 
* **[MCP3426 datasheet](http://ww1.microchip.com/downloads/en/DeviceDoc/22226a.pdf)** - MCP3426 ADC/I2C datasheet from Microchip.

Product Versions
----------------
* [LiBAT5V2A Multiplatform V0](https://infinitypv.com/products/electronics)- Boost converter from Li batteries to 5V/2A including INA219 and MCP3426 ADCs as monitoring devices. 

Version History
---------------
* [V0.0](https://github.com/infinityPV/infinityPV_INA219-library/releases/tag/v0.0) - Test Release


License Information
-------------------

This product is _**open source**_! 

Please review the license.txt file for license information. 

If you have any questions or concerns on licensing, please contact info@infinitypv.com.

Distributed as-is; no warranty is given.
