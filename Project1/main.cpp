#include "utilities.h"
#include <iostream>
#include "EthernetPacket.h"
#include "EthernetGenerator.h"

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


template <typename T>
void printVector(const std::vector<T>& vec) {
	for (const T& elem : vec) {
		std::cout << elem << " ";
	}
	std::cout << std::endl;
}


//int main()
//{
//
//	//std::vector<uint8_t> srcMacAdress(4, 0);
//	/*for (int x : defaultMacAdress)
//		std::cout << x << " ";*/
//
//	//make a simple class for testing 
//	std::cout << "\n";
//	std::vector<uint8_t> data = {0x02,0x02 ,0x02 ,0x02 ,0x02 };
//	//std::vector<uint8_t> data = { 5 };
//	uint32_t crc = calculateCRC(data);
//	for (int i = 0; i < data.size(); i++) {
//		std::cout << std::hex << data[i];
//	}
//	std::cout  << std::hex << crc;
//
//	std::vector<uint8_t> src = { 0,0,0,0  };
//	std::vector<uint8_t> dest = { 0,1,1,0 };
//	EthernetPacket etherpacket = EthernetPacket(src, dest, 0x02, 5, 0xffff);
//	std::vector<uint8_t> packet = etherpacket.GetPacket();
//	//printVector(packet);
//
//	std::cout << packet.size() << "\n";
//
//	for (size_t i = 0; i < packet.size(); ++i) {
//		std::cout << std::hex << (int)packet[i] << " ";
//	}
//
//
//	return 0;
//}


//int main() {
//	uint16_t original_value = 0xABCD;  // Example 16-bit integer
//
//	// Splitting the 16-bit integer into two 8-bit integers
//	uint8_t low_byte = original_value & 0xFF;        // Get the lower 8 bits
//	uint8_t high_byte = (original_value >> 8) & 0xFF; // Get the upper 8 bits
//
//	// Output the values
//	std::cout << "High Byte: " << std::hex << (int)high_byte << std::endl;
//	std::cout << "Low Byte: " << std::hex << (int)low_byte << std::endl;
//
//	return 0;
//}


//int main() {
//	std::cout << (float)1e2;
//	return 0;
//}


int main() {
	EthernetGenerator generator = EthernetGenerator();
	generator.GeneratePacketsDump();


	return 0;
}