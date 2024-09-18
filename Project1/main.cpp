#include "utilities.h"
#include <iostream>


uint32_t calculateCRC(std::vector<uint8_t> payLoad) {
	//this value is defined in standard
	const uint32_t generatorPoly = 0x04C11DB7;

	// Initial CRC value (all ones, 32 bits)
	uint32_t crc = 0xFFFFFFFF;
	for (size_t byteIndex = 0; byteIndex < payLoad.size(); ++byteIndex) {
		// XOR the next byte into the top of the CRC
		crc ^= (static_cast<uint32_t>(payLoad[byteIndex]) << 24);

		// Process each bit in the byte
		for (int bitIndex = 0; bitIndex < 8; ++bitIndex) {
			// If the top bit (MSB) is 1, XOR with the polynomial
			if (crc & 0x80000000) {
				crc = (crc << 1) ^ generatorPoly;
			}
			else {
				crc <<= 1;
			}
		}
	}

	// Finalize the CRC by inverting all the bits
	//crc = ~crc;

	// Ensure CRC is within 32 bits
	//return crc & 0xFFFFFFFF;
	return crc;

}

int main()
{

	//std::vector<uint8_t> srcMacAdress(4, 0);
	for (int x : defaultMacAdress)
		std::cout << x << " ";

	//make a simple class for testing 
	std::cout << "\n";
	std::vector<uint8_t> data = {0x05};
	//std::vector<uint8_t> data = { 5 };
	uint32_t crc = calculateCRC(data);
	std::cout  << std::hex << crc;

	return 0;
}