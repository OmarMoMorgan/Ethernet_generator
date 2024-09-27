#pragma once
#include <vector>
class EcpriPacket
{

private:
	static int seqID;
	int headerSize;

public:
	EcpriPacket();
	std::vector<uint8_t> GetEcpri(std::vector<uint8_t> &ORANpack); //this function will take a vector and convert it to ecpri
	void addEcpriHeader(std::vector<uint8_t>& emptyEcpri, int payloadSize);


};

