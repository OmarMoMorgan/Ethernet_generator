#pragma once
#include <vector>
#include "EthernetPacket.h"
#include "utilities.h"
#include "TextParser.h"
#include <cmath>
#include "ORANPacket.h"
#include "EcpriPacket.h"

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

	//this varilables are for oran should be moved later on
	int SCS;
	int MaxNrb;
	int NrbPerPacket;
	std::string PayloadType;
	std::string Payload;

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
	EthernetGenerator(std::string readFileName_, std::string writeFileName_);

	void GeneratePacketsDump();
	void GeneratePacketsWithORAN();

	int AddIFG(int packetSize);

	int TimeToBytesSent();
};

