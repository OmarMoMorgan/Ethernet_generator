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

	payLoad = std::vector<uint8_t>(0, 1500);

	//calculate the CRC here

}

std::vector<uint8_t> EthernetPacket::GetPacket() {
	//this funciton just returns the packet
	//pass
	short int payLoadSize = payLoad.size();
	packetResult = std::vector<uint8_t>(42 + payLoad.size());
	for (uint8_t i = 0; i < destMacAdress.size(); ++i) {
		packetResult[0 + i] = destMacAdress[i];
	}
	for (uint8_t i = 0; i < srcMacAdress.size(); ++i) {
		packetResult[4 + i] = srcMacAdress[i];
	}
	for (uint8_t i = 0; i < payLoad.size(); ++i) {
		packetResult[8 + i] = payLoad[i];
	}
	for (uint8_t i = 0; i < payLoad.size(); ++i) {
		packetResult[payLoadSize + 8 + i] = payLoad[i];
	}
	//calaculate CRC
	
	return packetResult;
}



//std::vector<uint8_t> EthernetPacket::generatePacket() {
//	dummy_vec = 
//	return vec
//}