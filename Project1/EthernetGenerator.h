#pragma once
#include <vector>
#include "EthernetPacket.h"
#include "utilities.h"

class EthernetGenerator
{
private:
	int LineRate;
	int captureSizeMs;
	int minNumOfIFGsPerPacket;
	std::vector<uint8_t> srcMacAdress;
	std::vector<uint8_t> destMacAdress;
	int maxPacketSize;
	int BurstSize;
	int BurstPeriodicty_us;
	uint8_t IFG;
	//std::vector<uint8_t>
	uint64_t preamble_SFD;
	float bitRate;
	int numGenertedPackets;
	float byteRate;

	std::vector<EthernetPacket> packetsList;
	//std::vector<float> packetsTimeList;

	struct packetEthernetGenerated
	{
		EthernetPacket* packet;
		float timeSent;
	};

public:
	EthernetGenerator(int lineRate_, int captureSizeMs,
					  int minNumOfIFGsPerPacket,
					  std::vector<uint8_t> srcMacAdress,
					  std::vector<uint8_t> destMacAdress,
					  int maxPacketSize,
					  int BurstSize,
					  int BurstPeriodicty_us,
					  uint8_t IFG,
					  std::vector<uint8_t> preamble_SFD
					  );

	EthernetGenerator();

	void GeneratePacketsDump();

	int AddIFG(int packetSize);

	int TimeToBytesSent();
};

