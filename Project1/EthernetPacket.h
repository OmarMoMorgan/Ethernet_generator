#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <cstdint>

//this class is responsible for ethernet packet 

class EthernetPacket
{
private:
	std::vector<uint8_t> srcMacAdress;
	std::vector<uint8_t> destMacAdress;
	std::vector<uint8_t> packetResult; //I think this needs to be changed as it is not necessary 
	uint16_t etherType; //this is defeind as 16bits according to the standard 
	std::vector<uint8_t> payLoad;
	std::vector<uint8_t> generatePacket();
	std::vector<uint8_t> calculateCRC();

	

public:
	EthernetPacket(std::vector<uint8_t> srcMacAdress_, std::vector<uint8_t> destMacAdress_, std::vector<uint8_t> payLoad_, uint8_t etherType_);
	EthernetPacket(std::vector<uint8_t> srcMacAdress_, std::vector<uint8_t> destMacAdress_,
		uint8_t payloadValue, int numBytes, uint16_t etherType_);  //this constructor will generate the same data
	std::vector<uint8_t> GetPacket();
	
};

