#include <Wire.h>

// generate this array using: Intel Hex Export to cdce.HEX
// using linux commands: objcopy -I ihex -O binary cdce.HEX
// then run xxd -i -a cdce.bin > cdce.h
// and copy in the contents of the .h file or include it below
// 77.76 MHz input to 21.424 MHz
// note byte 1 lower two bits!
// will change the address for subsequent writes, default = 1 (0x09)
unsigned char cdce_bin[] = {
  0x01, 0x09, 0x3c, 0x01, 0x02, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6f, 0x03, 0x0a, 0x00,
  0x53, 0xb0, 0xa2, 0xcf, 0x53, 0xb0, 0xa2, 0xcc
};
unsigned int cdce_bin_len = 32;

#define CDCEADDR 0x65 // change this, or you will have to rewrite the program slightly if setting something other than 0x65

void setup()
{
  Wire.begin(); // join i2c bus
  Serial.begin(115200);
  Serial.write("\r\nStartup\r\n");
 
}

byte count;

void loop()
{
  bool loop_1 = true;  
  bool write_data = false;

  Serial.write("Writing: ");

  // write config registers
  Wire.beginTransmission(CDCEADDR);
  Wire.write(0x01);
  Wire.write(5);
  Wire.write(&cdce_bin[1], 5);
  uint8_t retval = Wire.endTransmission(true);

  delay(1);

  // write PLL1 registers
  Wire.beginTransmission(CDCEADDR);
  Wire.write(0x10);
  Wire.write(0x10);
  Wire.write(&cdce_bin[0x10], 0x10);
  retval += Wire.endTransmission(true);

  // check error state
  if (retval == 0)
  {
    Serial.println("Write OK.");

  }
  else
  {
    Serial.println("Write Error.");
      while (true){}
  }

// wait for input if the EEPROM update should be set
Serial.print("Send y to write EEPROM, send n to only set RAM: ");
      while (loop_1)
  {
      if (Serial.available() > 0) 
      {
          switch(Serial.read())
          {
          case 'y':
                Serial.println("y");
                //cdce_bin[6] = 0x01;
                write_data = true;
                loop_1 = false;
              break;
              case 'n':
              Serial.println("n");
              loop_1 = false;
              break;
          }
      }
  }    

  if (write_data)
  {
    // write the EEPROM offset 6 -> 1
    Serial.println("EEPROM Write");
    Wire.beginTransmission(CDCEADDR);
    Wire.write(0x86); // byte write to offset 6
    Wire.write(0x01); // EEWRITE = 1
      uint8_t retval = Wire.endTransmission(true);
      // should technically wait for some time and poll EEPIP, but this program terminates here
  if (retval == 0)
  {
    Serial.println("Write OK.");

  }
  else
  {
    Serial.println("Write Error.");
  }
  }
  

while(1) {}
}
