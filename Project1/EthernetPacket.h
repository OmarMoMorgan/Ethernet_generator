#pragma once
#include <string>
#include <vector>

//this class is responsible for ethernet packet 

class EthernetPacket
{
private:
	std::string srcMacAdress;
	std::string destMacAdress;
	uint16_t etherType;
	std::vector<uint16_t> payLoad;
public:
	EthernetPacket(std::string srcMacAdress_, std::string destMacAdress_, std::vector<uint16_t> payLoad_, uint16_t etherType_);
	std::vector<uint16_t> generatePacket();
	
};

