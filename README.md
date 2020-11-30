# Arduino_CDCE913_Programmer
A programmer for the CDCE913 PLL chip running on an Arduino

To use:

Generate a configuration using TI Clock Pro

Export an Intel Hex file, I called it cdce.HEX

Convert it to a binary file:
`objcopy -I ihex -O binary cdce.HEX cdce.bin`

Generate a .h file:
`xxd -i -a cdce.bin > cdce.h`

Either include it in the program or just cut and paste it into the top of the .ino file

Warning: If you set the address offset ("SlaveAddress" in Clock Pro) field to something other than 1, or are programming a non-blank device with this field set to something other than 1, the program will fail.
This is because the program currently assumes the address is 0x65 all the time (0x64 base + 1 offset). You will need to update the program to handle changing the address of the device during the write if this is required.

The program automatically programs the device using the Wire interface (standard I2C port on the Arduino Due). It will then ask you to send 'y' to program the EEPROM. Don't do this unless you're sure you know the programming worked and is working as intended.
