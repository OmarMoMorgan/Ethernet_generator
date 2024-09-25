#pragma once
#include <iostream>
#include <vector>
#include "TextParser.h"

class ORANPacket
{
	//here we will define the main parameters
	//after some thinking the class will be reposonble for generating all the 
	//oran packets and just returns the packet that should be sent

private:
	int scs;
	int maxNrb;
	int NrbPerPacket;
	int payLoadType;
	int PayloadFile;
	static int packetNumber;
	void getNumerology();
	int getNumberPacketsPerFrame();

	int slotCount;		//all of those vars are per frame
	int subFrameCount;
	int SymbolsCount;
	int mu;


	std::vector<std::vector<uint8_t>> frameVector;
	//make packet header
	void MakeHeader(int packetID); //this will make the header for each packet
	void MakeSmallHeader(int packetID);
	void MakeIQPacketData(int packetID);
	//void ReadDataIntoVector(); //this will read the data that is coming from file

	int frameID;
	int subFrameID;
	int slotID;
	int symbolID;
	int sectionID; // probably will leave this on one 
	int rb;
	int symInc; //will probably also leave this blank
	int numPrbu; // also will leave it as aconstant for now	

	int startPrbu; //this will change to be somehing to get passed to function
	//int numPrbu; //same as above should be removed 

	TextParser* parser; //this shoudl be intialized in consturcotr
	int locationAtPacketGen;

public:
	ORANPacket(int scs_,
	int maxNrb_,
	int NrbPerPacket_,
	int payLoadType_,
	int PayloadFile_);


	std::vector<std::vector<uint8_t>> GeneratePackets();
	//the current aproach i am thikning of is make the whole frame
	//then send packet by packet when needed 
	//the other approach i am thinking of is making 
	//a packet by packet when requested 
	//make this class only make the frame and when you want another one make another 
	//frame 
	//void GetNextPacket();


};

