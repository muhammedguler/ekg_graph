#include <mcp3208.h>

Mcp3208::Mcp3208()
	: INITIALIZED(false)
	, RASPI_SPI_CHANNEL(0)
{
}

bool Mcp3208::initMcp3208(const char raspiSpiChannel, const int busSpeed)
{
	std::cout << "Initializing...";
	if (wiringPiSPISetup(raspiSpiChannel, busSpeed) < 0)
	{
		std::cerr << "FAIL!\nError while connecting to SPI device!" << std::endl;
		return false;
	}

	std::cout << "OK!" << std::endl;
	RASPI_SPI_CHANNEL = raspiSpiChannel;
	INITIALIZED = true;
	return true;
}

short Mcp3208::readMcp3208(const char adcReadChannel)
{
	if (!INITIALIZED)
	{
		std::cerr << "Error! Initialize it first." << std::endl;
	}

	short int adcValue;
	unsigned char buff[3];
	buff[0] = (0xC0 | ((adcReadChannel & 0x07) << 3));
	buff[1] = 0x0;
	buff[2] = 0x0;
	wiringPiSPIDataRW(RASPI_SPI_CHANNEL, buff, 3);
	adcValue  = (buff[0] & 0x01) << 11;
	adcValue += buff[1] << 3;
	adcValue += (buff[2] & 0b11100000) >> 5;
	return adcValue;
}
