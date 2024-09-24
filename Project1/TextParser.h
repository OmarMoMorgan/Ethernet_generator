#pragma once
#include <string>
#include "utilities.h"
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>

/*this class is reposnsible for dealing with all the text files that could be needed
*/


class TextParser
{
private:
	std::string readFile;
	std::string dumpFile;
	std::string IQFile;
	std::ifstream readFileStream;
	std::ofstream WriteFileStream;
	std::ifstream readIQData;


	//ethernet_Generation_data
public:
	TextParser();
	TextParser(std::string readFile_, std::string dumpFile_);
	TextParser(std::string readFile_, std::string dumpFile_ , std::string IQFile_);
	ethernet_Generation_data ReadFromFile();
	void WriteWholePacket(std::vector <uint8_t> packet_,uint64_t preamble, uint8_t IFG_val, int numIFG);
	void WritePacketTill(std::vector <uint8_t> packet_, uint64_t preamble, uint8_t IFG_val, int numIFG,int stopByte);
	void OpenFileRead(const std::string& filename);
	void OpenFileWrite(const std::string& filename);
	void CloseFileWrite();
	void CloseFileRead();
	void OpenFileIQ(const std::string& filename);
	std::vector<uint8_t> ReadIQData(int linesToRead);
	//void ReadFile()
};

