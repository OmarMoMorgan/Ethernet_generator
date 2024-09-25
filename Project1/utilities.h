#pragma once
#include <vector>
#include <string>

static const std::vector<uint8_t> defaultMacAdress(6,0xf);
//std::vector<uint8_t> defaultMacAdress(6, 0xf);

struct ethernet_Generation_data {
	int LineRate;
	int captureSizeMs;
	int minNumOfIFGsPerPacket;
	std::vector<uint8_t> srcMacAdress;
	std::vector<uint8_t> destMacAdress;
	int maxPacketSize;
	int BurstSize;
	int BurstPeriodicty_us;

	//this variables should be removed later
	//ORAN stuff
	int SCS;
	int MaxNrb;
	int NrbPerPacket;
	std::string PayloadType;
	int Payload;
	 
	
	//uint8_t IFG;
	//std::vector<uint8_t> preamble_SFD;
	//float bitRate;

    // Constructor with default values
    /*ethernet_Generation_data(int intVal = 0, float floatVal = 0.0f, std::string strVal = "")
        : intValue(intVal), floatValue(floatVal), stringValue(strVal) {}*/
};
