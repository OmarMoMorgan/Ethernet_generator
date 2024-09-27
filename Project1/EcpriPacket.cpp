#include "EcpriPacket.h"


EcpriPacket::EcpriPacket() {
	headerSize = 8;
}

std::vector<uint8_t> EcpriPacket::GetEcpri(std::vector<uint8_t> &ORANpack) {
	int payloadSize = ORANpack.size();
	
	std::vector<uint8_t> ecpriPacket(headerSize+payloadSize,payloadSize);
	//just add the payload here, this solution is really stupid for now
	addEcpriHeader(ecpriPacket, payloadSize);
	for (int i = 0; i < payloadSize; i++) {
		ecpriPacket[8 + i] = ORANpack[i];
	}
	return ecpriPacket;
}


void EcpriPacket::addEcpriHeader(std::vector<uint8_t>& emptyEcpri,int payloadSize) {
	emptyEcpri[0] = 0; //this was asked in requirments for now
	emptyEcpri[1] = 0; //this is the message type we are assuming it is u plane
	emptyEcpri[2] = (payloadSize >> 8) & 0xff;
	emptyEcpri[3] = payloadSize & 0xff;
	emptyEcpri[4] = 0; //also for now fixed as zero
	emptyEcpri[5] = 0; //also for now fixed as zero
	emptyEcpri[6] = seqID;
	emptyEcpri[7] = 0;
	seqID++;
}


int EcpriPacket::seqID = 0;
