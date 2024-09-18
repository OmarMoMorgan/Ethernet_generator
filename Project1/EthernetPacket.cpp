#include "EthernetPacket.h"
//#include "utilities.h"


EthernetPacket::EthernetPacket(std::vector<uint8_t> srcMacAdress_,
							   std::vector<uint8_t> destMacAdress_,
							   std::vector<uint8_t> payLoad_, 
							   uint8_t etherType_) 
{

}
EthernetPacket::EthernetPacket(std::vector<uint8_t> srcMacAdress_,
	std::vector<uint8_t> destMacAdress_,
	uint8_t payloadValue, int numBytes,
	uint16_t etherType_) {
	srcMacAdress = std::vector<uint8_t>(4);
	destMacAdress = std::vector<uint8_t>(4);

	srcMacAdress = srcMacAdress_;
	destMacAdress = destMacAdress_;
	if (numBytes > 1500) {
		std::cout << "warning exceed the max number of bytes for ethernet packets 1500 will be assumed \n";
		numBytes = 1500;
	}

	payLoad = std::vector<uint8_t>(numBytes, payloadValue);
	std::cout << "debuggin\n";
	for (int i = 0; i < payLoad.size(); i++) {
		std::cout << std::hex << payLoad[i];
	}

	etherType = etherType_;

	//calculate the CRC here

}

std::vector<uint8_t> EthernetPacket::GetPacket() {
	//this funciton just returns the packet
	//pass
	short int payLoadSize = payLoad.size();
	std::cout <<"paylaod size is " << payLoadSize << "\n";
	//dest adress
	packetResult = std::vector<uint8_t>(18 + payLoad.size());
	for (uint8_t i = 0; i < destMacAdress.size(); ++i) {
		packetResult[0 + i] = destMacAdress[i];
	}
	//source adress
	for (uint8_t i = 0; i < srcMacAdress.size(); ++i) {
		packetResult[6 + i] = srcMacAdress[i];
	}
	/*for (uint8_t i = 0; i < srcMacAdress.size(); ++i) {
		packetResult[8 + i] = & 0xFF;
	}*/
	//ether type
	packetResult[12] = etherType & 0xFF;
	packetResult[13] = (etherType >> 8) & 0xFF;

	//payload 
	for (uint8_t i = 0; i < payLoad.size(); ++i) {
		packetResult[14 + i] = payLoad[i];
	}
	/*for (uint8_t i = 0; i < payLoad.size(); ++i) {
		packetResult[payLoadSize + 10 + i] = payLoad[i];
	}*/
	//calaculate CRC
	uint32_t crc = this->calculateCRC();
	std::cout << std::hex  << "crc os " << crc << "\n";
	for (uint8_t i = 0; i < 4; ++i) {
		packetResult[payLoadSize + 14 + i] = (crc >> (24-8*(i)) & 0xff);
	}
	
	return packetResult;
}


uint32_t EthernetPacket::calculateCRC() {
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
	return crc & 0xFFFFFFFF;

}



//std::vector<uint8_t> EthernetPacket::generatePacket() {
//	dummy_vec = 
//	return vec
//}