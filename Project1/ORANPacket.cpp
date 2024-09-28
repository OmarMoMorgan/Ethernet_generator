#include "ORANPacket.h"



ORANPacket::ORANPacket(int scs_,
	int maxNrb_,
	int NrbPerPacket_,
	std::string payLoadType_,
	std::string PayloadFile_,
	TextParser* parser_) {


	scs = scs_;
	maxNrb = maxNrb_;
	NrbPerPacket = NrbPerPacket_;
	payLoadType = payLoadType_;
	PayloadFile = PayloadFile_;
	//frameID++;
	getNumerology();
	//determine the pacekts in a vecotr that will contaitn a vector of vectors
	frameVector = std::vector<std::vector<uint8_t>>(getNumberPacketsPerFrame());
	parser = parser_;
	parser->OpenFileIQ(PayloadFile);
}

ORANPacket::ORANPacket() {
}

void ORANPacket::getNumerology() {
	mu = (scs / 15)-1;
	if (mu > 6 || mu < 0) {
		std::cout << "there is an error in the scs value assuming scs is 15";
		scs = 15;
		mu = 0;
	}
	subFrameCount = 10; // 1 * pow(2, mu);
	slotCount = subFrameCount * pow(2, mu); //10 * subFrameCount;
	SymbolsCount = 14 * slotCount;
}


int ORANPacket::getNumberPacketsPerFrame() {
	int num =  SymbolsCount* ceil((float)maxNrb / (float)NrbPerPacket);
	return num;
}

//dtermeine the packet size 
void ORANPacket::MakeHeader(int packetID , int flagSize) {
	//general packet header 
	locationAtPacketGen = 0;
	int maxPacketBytes =  4 + 4 + ( 12 * 2 * 2) * NrbPerPacket;
	int theOtherSize =  4 + 4 + (12 * 2 * 2) * (maxNrb % NrbPerPacket);
	if (flagSize == 1) {
		frameVector[packetID] = std::vector<uint8_t>(theOtherSize);
	}
	else {
		
		frameVector[packetID] = std::vector<uint8_t>(maxPacketBytes);
	}
	frameVector[packetID][0] = 0x00;
	frameVector[packetID][1] = frameID;
	frameVector[packetID][2] = ((subFrameID << 4) & 0xf0) | (slotID >> 4) &0x0f;
	frameVector[packetID][3] = ((slotID << 6) & 0xc0) | (symbolID);
	
	

	/*frameVector[packetID].push_back(0x00);
	frameVector[packetID].push_back(frameID);
	frameVector[packetID].push_back(((subFrameID << 4) & 0xff) | (slotID >> 4) & 0x0f);
	frameVector[packetID].push_back(((slotID << 6) & 0xc0) | (symbolID));*/

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
	frameVector[packetID][locationAtPacketGen+1] = (sectionID & 0x0f) | rb << 3 | symInc << 2 | ((startPrbu >> 8) & 0x03);
	frameVector[packetID][locationAtPacketGen+2] = startPrbu;
	frameVector[packetID][locationAtPacketGen+3] = numPrbu;
	
	locationAtPacketGen = locationAtPacketGen + 4;
	/*frameVector[packetID].push_back(sectionID << 8);
	frameVector[packetID].push_back((sectionID & 0x0f) | rb << 3 | symInc << 2 | ((startPrbu << 8) & 0x03));
	frameVector[packetID].push_back(startPrbu << 8);
	frameVector[packetID].push_back(numPrbu);*/


}

//this function will get the iq data and put it insdie the packet 
void ORANPacket::MakeIQPacketData(int packetID) {
	//parser->OpenFileIQ(); //this line should also be aboeve in constructor
	std::vector<uint8_t> dataRead = parser->ReadIQData(12); //12 is choosed according to standard
	for (int i = 0; i < (12 * 4 ); i++) {
		frameVector[packetID][i + locationAtPacketGen] = dataRead[i];
		//frameVector[packetID].push_back(dataRead[i]);
	}
	locationAtPacketGen = locationAtPacketGen + (12 * 4);

}


std::vector<std::vector<uint8_t>> ORANPacket::GeneratePackets() {
	//this is the maestor of generating the frame and oran packets
	frameID++;
	int numPackets = getNumberPacketsPerFrame();
	int nrbUsed = 0;
	int counterPerSym = 0;
	int peridocityPacketsSize = ceil(maxNrb / NrbPerPacket);
	subFrameID = 0;
	slotID = 0;
	symbolID = 0;

	sectionID = 0;
	startPrbu = 0;

	rb = 0;
	symInc = 0;
	numPrbu;

	for(int i=0;i<numPackets;i++) {
		
		//make the header first of the whole thing 
		
		if (peridocityPacketsSize == counterPerSym) {
			MakeHeader(i,1);
			counterPerSym = -1;
		}
		else {
			MakeHeader(i, 0);
		}
		counterPerSym++;
		
		if (nrbUsed < (maxNrb - NrbPerPacket)) {
			startPrbu = nrbUsed;
			numPrbu = NrbPerPacket;
		}
		else {
			startPrbu = nrbUsed;
			numPrbu = NrbPerPacket - (maxNrb - nrbUsed);
		}
		MakeSmallHeader(i);
		//get the number of how many rb will be used per packet
		//this is per symbol
		for (int j = 0; j < NrbPerPacket; j++) {
			//this method is not the best computatiaonally but for now will leave it like this
			if (nrbUsed >= maxNrb) {
				nrbUsed = 0;
				break;
			}		
			MakeIQPacketData(i);
			nrbUsed++;
		}
		//subFrameID
		if ((i+1) % (numPackets / 10) == 0) {
			subFrameID++;
			if (subFrameID >= 10) {
				subFrameID = 0;
			}
		}

		//slotID
		if ((i+1) % (numPackets / slotCount) == 0) {
			slotID++;
			if (slotID >= (slotCount/subFrameCount)) {
				slotID = 0;
			}
		}

		//symbolID
		if ((i+1) % (numPackets / SymbolsCount) == 0) {
			symbolID++;
			if (symbolID >= (SymbolsCount/slotCount)) {
				symbolID = 0;
			}
		}
	}
	//parser->CloseFileIQ();
	return frameVector;
}
