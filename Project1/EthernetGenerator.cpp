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
	captureSizeMs = captureSizeMs_;
	minNumOfIFGsPerPacket = minNumOfIFGsPerPacket_;
	srcMacAdress = srcMacAdress_;
	destMacAdress = destMacAdress_;
	maxPacketSize = maxPacketSize_;
	BurstSize = BurstSize_;
	BurstPeriodicty_us = BurstPeriodicty_us_;
	IFG = IFG_;
	//preamble_SFD = preamble_SFD_;

	bitRate = LineRate;
	numGenertedPackets = 0;
	byteRate = LineRate * 1e9 / 8;

}


EthernetGenerator::EthernetGenerator() {
	LineRate = 1;
	captureSizeMs = 1;
	minNumOfIFGsPerPacket = 1;
	srcMacAdress = { 0x01,0x01,0x01,0x01,0x00,0x00 };
	destMacAdress = {0x00,0x00,0x00,0x00,0x00,0x00};
	maxPacketSize = 1500;
	BurstSize = 1;
	BurstPeriodicty_us = 100;
	IFG = 0x07;
	preamble_SFD = 0xFB555555555555D;

	bitRate = LineRate;
	numGenertedPackets = 0;
	byteRate = LineRate * 1e9 / 8;
}


void EthernetGenerator::GeneratePacketsDump() {
	TextParser parser = TextParser("readFile.txt" , "WriteFile.txt");
	parser.OpenFileWrite("WriteFile.txt");
	float time_elapsed = 0;
	while (1) {
		//the numbers below are generated basaed on nothing shoudl be modified
		EthernetPacket* packet_arr = new EthernetPacket[BurstSize];  //(srcMacAdress,destMacAdress,2,50,25);
		for (int i = 0; i < BurstSize; i++) {
			packet_arr[i] = EthernetPacket(srcMacAdress, destMacAdress, 2, maxPacketSize, 25);
			int packet_size = packet_arr[0].GetPacketSize();
			//add IFG(currently i am just determing how many of them will be written)
			int IFGadded = AddIFG(packet_size);

			

			float time_needed = (packet_size + IFGadded) / byteRate;
			float remainingTime = (captureSizeMs * 10e-3) - time_elapsed;
			
			//now check whether this packet can be fully sent or not 
			if (remainingTime < time_needed) {
				//we exceded time should now quit
				//first we need to determine where exactly we stopped 
				int numBytesPossibleToSend = (remainingTime - time_needed) / byteRate;
				int IFGadded_quitEarly = AddIFG(numBytesPossibleToSend);
				//write using another special function that only write till a specfic point and pads with ifg
				parser.CloseFileWrite();
				return;
			}
			time_elapsed += time_needed;

			//write this should be done after making the text class 
			parser.WriteWholePacket(packet_arr[i].GetPacket(), preamble_SFD, IFG, IFGadded);

			//return
		}
		time_elapsed += BurstPeriodicty_us * 1e-6;

		//int numBytesInPacket= packet.GetPacketSize();
		//delete packet_arr;
	}


	return;
}


void EthernetGenerator::GeneratePacketsWithORAN() {
	//this function will do what we have doen above except now
	//we have ORAN packets so we will generate them here
	//encasulate them in ecpri and 
}


int EthernetGenerator::AddIFG(int packetSize) {
	int extraIFG = 4 - ((packetSize + minNumOfIFGsPerPacket) % 4);
	return minNumOfIFGsPerPacket + extraIFG;
}


int EthernetGenerator::TimeToBytesSent() {
	return 0;
}


EthernetGenerator::EthernetGenerator(std::string readFileName_ , std::string writeFileName_) {
	TextParser reader = TextParser(readFileName_,writeFileName_);
	ethernet_Generation_data eth_data = reader.ReadFromFile();
	LineRate = eth_data.LineRate;
	captureSizeMs = eth_data.captureSizeMs;
	minNumOfIFGsPerPacket = eth_data.minNumOfIFGsPerPacket;
	srcMacAdress = eth_data.srcMacAdress;
	destMacAdress = eth_data.destMacAdress;
	maxPacketSize = eth_data.maxPacketSize;
	BurstSize = eth_data.BurstSize;
	BurstPeriodicty_us = eth_data.BurstPeriodicty_us;
	//IFG = eth_data.IFG;
	//preamble_SFD = preamble_SFD_;
	IFG = 0x07;
	preamble_SFD = 0xFB555555555555D;
	bitRate = LineRate;
	numGenertedPackets = 0;
	byteRate = LineRate * 1e9 / 8;
}