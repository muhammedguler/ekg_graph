#ifndef MCP3208_H
#define MCP3208_H

#include <iostream>
#include <wiringPiSPI.h>

class Mcp3208
{
	private:
		bool INITIALIZED;
		char RASPI_SPI_CHANNEL;

	public:
		Mcp3208();
		bool initMcp3208( const char raspiSpiChannel = 0, const int busSpeed = 500000);
		short int readMcp3208(const char adcReadChannel);
};

#endif
