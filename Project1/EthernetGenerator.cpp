#include "EthernetGenerator.h"



EthernetGenerator::EthernetGenerator(int lineRate_, int captureSizeMs_,
	int minNumOfIFGsPerPacket_,
	std::vector<uint8_t> srcMacAdress_,
	std::vector<uint8_t> destMacAdress_,
	int maxPacketSize_,
	int BurstSize_,
	int BurstPeriodicty_us_,
	uint8_t IFG_,
	std::vector<uint8_t> preamble_SFD_) {

	LineRate = lineRate_;
	minNumOfIFGsPerPacket = minNumOfIFGsPerPacket_;
	srcMacAdress = srcMacAdress_;
	destMacAdress = destMacAdress_;
	maxPacketSize = maxPacketSize_;
	BurstSize = BurstSize_;
	BurstPeriodicty_us = BurstPeriodicty_us_;
	IFG = IFG_;
	preamble_SFD = preamble_SFD_;

	bitRate = LineRate;
	numGenertedPackets = 0;
	byteRate = LineRate * 1e9 / 8;

}


void EthernetGenerator::GeneratePacketsDump() {
	float time_elapsed = 0;
	while (1) {
		//the numbers below are generated basaed on nothing shoudl be modified
		EthernetPacket* packet_arr = new EthernetPacket[BurstSize];  //(srcMacAdress,destMacAdress,2,50,25);
		for (int i = 0; i < BurstSize; i++) {
			packet_arr[i] = EthernetPacket(srcMacAdress, destMacAdress, 2, 50, 25);
			int packet_size = packet_arr[0].GetPacketSize();
			//add IFG(currently i am just determing how many of them will be written)
			int IFGadded = AddIFG(packet_size);

			

			int time_needed = (packet_size + IFGadded) / byteRate;
			int remainingTime = (captureSizeMs * 10e-3) - time_elapsed;
			//now check whether this packet can be fully sent or not 
			if (remainingTime < time_needed) {
				//we exceded time should now quit
				//first we need to determine where exactly we stopped 
				int numBytesPossibleToSend = (remainingTime - time_needed) / byteRate;
				int IFGadded_quitearly = AddIFG(numBytesPossibleToSend);
				//write using another special function that only write till a specfic point and pads with ifg
				return;
			}

			//write this should be done after making the text class 

			//return
		}

		//int numBytesInPacket= packet.GetPacketSize();
		//delete packet_arr;
	}


	return;
}


int EthernetGenerator::AddIFG(int packetSize) {
	int extraIFG = 4 - (packetSize + minNumOfIFGsPerPacket % 4);
	return minNumOfIFGsPerPacket + extraIFG;
}


int EthernetGenerator::TimeToBytesSent() {
	return 0;
}