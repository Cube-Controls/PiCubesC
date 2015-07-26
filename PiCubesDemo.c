#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>


//*********************************************************************************
// Control and Read Pi Cubes UI Board
// signed long readUI(int module, int input, int typeui, signed long *value) 
// module - Module address(1 - 6)
// input - Input number(1 - 4)
// typeui - Input type
//            0 = Resistance(Return value 0 - 350000)
//            1 = Digital(Return value 0 - 1)
//            2 = Voltage / Current(Return value 0 - 100 % , represent 0 - 10V / 0 - 20mA)
//            3 = 10K Type II Thermistor(Return value - 4000 to 14000, scale 0.01)
//            4 = Pulse Input(Return value is counter value)
// value - read value of universal input
//*********************************************************************************

int readUI(int module, int input, int typeui, signed long *value)
{
	char *fileName = "/dev/i2c-1";
	unsigned char buf[10];
	unsigned char address;
	int fd;

	// check is it module parameter in range
	if ((module < 1) || (module > 6)) return -1;
	// check is it output parameter in range
	if ((input < 1) || (input > 4)) return -1;

	// Open port for reading and writing
	if ((fd = open(fileName, O_RDWR)) < 0)
	{
		printf("Failed to open i2c-1 port.\n");
		return -1;
	}

	// Set the port options and set the address of the device we wish to speak to
	if (ioctl(fd, I2C_SLAVE, 0x1C) < 0)
	{
		printf("Unable to get bus access to talk to Pi-Cubes I/O module.\n");
		return -1;
	}

	// calculate address to write typeui
	address = (unsigned char)((module - 1) * 8 + (input - 1) * 2);

	// setup buffer
	buf[0] = address;
	buf[1] = (unsigned char)(typeui);

	// Send address and type value first to I/O module
	if ((write(fd, buf, 2)) != 2)
	{
		printf("Error writing to Pi-Cubes I/O module.\n");
		return -1;
	}

	// calculate address to read ui value
	address = (unsigned char)(48 + (module - 1) * 16 + (input - 1) * 4);

	// setup buffer
	buf[0] = address;

	// Send address that we want to read
	if ((write(fd, buf, 1)) != 1)
	{
		printf("Error writing to Pi-Cubes I/O module.\n");
		return -1;
	}

	// Read data back to buf
	if ((read(fd, buf, 4)) != 4)
	{
		printf("Error reading from Pi-Cubes I/O module.\n");
		return -1;
	}

	*value = buf[0] | (buf[1] << 8) | (buf[2] << 16) | (buf[3] << 24);

	// Close Port
	close(fd);

	return 0;
}

//*********************************************************************************
// Control Pi Cubes UO Board
// int writeUO(int module, int output, int typeuo, int value) 
// module - Module address(1 - 6)
// output - Output number(1 - 4)
// type - Universal output type :
//			 0 = Digital Output(0 / 12VDC)
//           1 = Modulation(0 - 10VDC)
//           2 - 255 = Digital otput PWM with this period in sec.
// value - Universal output value :
//           0 / 1 = For digital type
//           0 - 100 % = for Modulation / PWM
//*********************************************************************************

int writeUO(int module, int output, int typeuo, int value)
{
	char *fileName = "/dev/i2c-1";
	unsigned char buf[10];
	unsigned char address;
	int fd;

	// check is it module parameter in range
	if ((module < 1) || (module > 6)) return -1;
	// check is it output parameter in range
	if ((output < 1) || (output > 4)) return -1;

	// Open port for reading and writing
	if ((fd = open(fileName, O_RDWR)) < 0)
	{
		printf("Failed to open i2c-1 port.\n");
		return -1;
	}

	// Set the port options and set the address of the device we wish to speak to
	if (ioctl(fd, I2C_SLAVE, 0x1C) < 0)
	{
		printf("Unable to get bus access to talk to Pi-Cubes I/O module.\n");
		return -1;
	}

	// calculate address
	address = (unsigned char)((module - 1) * 8 + (output - 1) * 2);

	// setup buffer
	buf[0] = address;
	buf[1] = (unsigned char)(typeuo);
	buf[2] = (unsigned char)(value);

	// Send address and value to I/O module
	if ((write(fd, buf, 3)) != 3)
	{
		printf("Error writing to Pi-Cubes I/O module.\n");
		return -1;
	}
	// Close Port
	close(fd);

	return 0;
}

//*********************************************************************************
// Control Pi Cubes DO Board
// int writeDO(int module, int output, int value)
// module - Module address(1 - 6)
// output - Output number(1 - 4)
// value - Digital output value :
//            0 = Off
//            1 = On
//*********************************************************************************

int writeDO(int module, int output, int value)
{
	char *fileName = "/dev/i2c-1";
	unsigned char buf[10];
	unsigned char address;
	int fd;

	// check is it module parameter in range
	if ((module < 1) || (module > 6)) return -1;
	// check is it output parameter in range
	if ((output < 1) || (output > 4)) return -1;

	// Open port for reading and writing
	if ((fd = open(fileName, O_RDWR)) < 0) 
	{					
		printf("Failed to open i2c-1 port.\n");
		return -1;
	}

	// Set the port options and set the address of the device we wish to speak to
	if (ioctl(fd, I2C_SLAVE, 0x1C) < 0) 
	{					
		printf("Unable to get bus access to talk to Pi-Cubes I/O module.\n");
		return -1;
	}

	// calculate address
	address = (unsigned char)((module - 1) * 8 + (output - 1) * 2);

	// setup buffer
	buf[0] = address;
	buf[1] = (unsigned char)(value);

	// Send address and value to I/O module
	if ((write(fd, buf, 2)) != 2) 
	{									
		printf("Error writing to Pi-Cubes I/O module.\n");
		return -1;
	}
	// Close Port
	close(fd);

	return 0;
}

int main(int argc, char *argv[])
{
	signed long retvalue;
	int i;

	// Test readUI - Read Resistance on Module 2 Input 1
	readUI(2, 1, 0, &retvalue);
	printf("Resistance Input 1 : %i\n", retvalue);
	sleep(2);

	//Test writeDO - Send off to Digital Output 1 on Module 1 
	writeDO(1, 1, 0); 
	sleep(2);

	//Test writeDO - Send on to Digital Output 1 on Module 1 
	writeDO(1, 1, 0);
	sleep(2);

	//Test writeUO - Send off to Universal Output 1 on Module 3
	writeUO(3, 1, 0, 0);
	sleep(2);
		
	//Test writeUO - Send on to Universal Output 1 on Module 3
	writeUO(3, 1, 0, 1);
	sleep(2);

	//Test writeUO - Send 5V Modulation signal to Universal Output 1 on Module 3
	writeUO(3, 1, 1, 50);
	sleep(2);

	return 0;
}