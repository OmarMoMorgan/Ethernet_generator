#pragma once
#include <vector>
class ParserPackets
{
private:
	uint8_t IFG = 0x07;
	uint64_t preamble_SFD = 0xFB555555555555D;
	std::vector<uint8_t> PreambleVec;
	std::vector<uint8_t> srcMac;
	std::vector<uint8_t> destMac;
	int OranPacketSize;
	uint16_t etherType;
	int ecpriSeqId;
	std::vector<uint8_t> ORANData;
	std::vector<uint8_t> CRC;



	bool CompareEthernetHeader(const std::vector<uint8_t> &data, int start);
	std::vector<uint8_t> GetMacAdress(const std::vector<uint8_t> &data , int pos);

	


public:
	ParserPackets();
	void GetHeaders(std::vector<uint8_t> data);

};

