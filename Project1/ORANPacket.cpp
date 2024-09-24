#include "ORANPacket.h"



ORANPacket::ORANPacket(int scs_,
	int maxNrb_,
	int NrbPerPacket_,
	int payLoadType_,
	int PayloadFile_) {


	scs = scs_;
	maxNrb = maxNrb_;
	NrbPerPacket = NrbPerPacket_;
	payLoadType = payLoadType_;
	PayloadFile = PayloadFile_;
	getNumerology();
	//determine the pacekts in a vecotr that will contaitn a vector of vectors
	frameVector = std::vector<std::vector<uint8_t>>(getNumberPacketsPerFrame());
}

void ORANPacket::getNumerology() {
	mu = scs / 15;
	if (mu > 6 || mu < 0) {
		std::cout << "there is an error in the scs value assuming scs is 15";
		scs = 15;
		mu = 0;
	}
	subFrameCount = 1 * pow(2, mu);
	slotCount = 10 * subFrameCount;
	SymbolsCount = 14 * slotCount;
}


int ORANPacket::getNumberPacketsPerFrame() {
	return 14 * SymbolsCount * maxNrb / NrbPerPacket;
}

//dtermeine the packet size 
void ORANPacket::MakeHeader(int packetID) {
	//general packet header 
	locationAtPacketGen = 0;
	frameVector[packetID][0] = 0x00;
	frameVector[packetID][1] = frameID;
	frameVector[packetID][2] = ((subFrameID << 4) & 0xff) | (slotID >> 4) &0x0f;
	frameVector[packetID][3] = ((slotID << 6) & 0xc0) | (symbolID);

	//the header for every packet 
	//consider tunring this into a function
	

	//frameVector[0][2] = 0;
	locationAtPacketGen = 4;
}

void ORANPacket::MakeSmallHeader(int packetID) {
	/*frameVector[packetID][4] = sectionID << 8;
	frameVector[packetID][5] = (sectionID & 0x0f) | rb << 3 | symInc << 2 | ((startPrbu << 8) & 0x03);
	frameVector[packetID][6] = startPrbu << 8;
	frameVector[packetID][7] = numPrbu;
	locationAtPacketGen = 8;*/

	frameVector[packetID][locationAtPacketGen] = sectionID << 8;
	frameVector[packetID][locationAtPacketGen+1] = (sectionID & 0x0f) | rb << 3 | symInc << 2 | ((startPrbu << 8) & 0x03);
	frameVector[packetID][locationAtPacketGen+2] = startPrbu << 8;
	frameVector[packetID][locationAtPacketGen+3] = numPrbu;
	locationAtPacketGen = locationAtPacketGen + 4;

}

//this function will get the iq data and put it insdie the packet 
void ORANPacket::MakeIQPacketData(int packetID) {
	//parser->OpenFileIQ(); //this line should also be aboeve in constructor
	std::vector<uint8_t> dataRead = parser->ReadIQData(12); //12 is choosed according to standard
	for (int i = 0; i < (12 * 4 ); i++) {
		frameVector[packetID][i + locationAtPacketGen] = dataRead[i];
	}
	locationAtPacketGen = locationAtPacketGen + (12 * 4);

}


std::vector<std::vector<uint8_t>> ORANPacket::GeneratePackets() {
	//this is the maestor of generating the frame and oran packets
	int numPackets = getNumberPacketsPerFrame();
	int nrbUsed = 0;
	for(int i=0;i<numPackets;i++) {
		//make the header first of the whole thing 
		MakeHeader(i);
		//get the number of how many rb will be used per packet
		for (int j = 0; j < NrbPerPacket; j++) {
			//this method is not the best computatiaonally but for now will leave it like this
			if (nrbUsed > maxNrb) {
				return frameVector;
			}

			MakeSmallHeader(j);
			MakeIQPacketData(j);
			nrbUsed++;
		}
	}
	return frameVector;
}
