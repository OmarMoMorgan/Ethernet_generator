#include "TextParser.h"


TextParser::TextParser(std::string readFile_, std::string dumpFile_) {
	readFile = readFile_;
	dumpFile = dumpFile_;

}

TextParser::TextParser() {

}

void TextParser::OpenFileRead(const std::string& filename) {
    //std::ifstream readFileStream(filename);
    readFileStream.open(filename);
    if (!readFileStream.is_open()) {
        std::cerr << "Error: Could not open file." << std::endl;
        return;
    }
    return;
}

void TextParser::OpenFileWrite(const std::string& filename) {
    //std::ofstream writeFileStream(filename);
    WriteFileStream.open(filename);
    if (!WriteFileStream.is_open()) {
        std::cerr << "Error: Could not open file." << std::endl;
        //return;
    }
    
}

ethernet_Generation_data TextParser::ReadFromFile() {
    std::ifstream file(readFile);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file." << std::endl;
        //return NULL;
    }

    ethernet_Generation_data eth;
    uint64_t adress;
    eth.destMacAdress = std::vector<uint8_t>(6);
    eth.srcMacAdress = std::vector<uint8_t>(6);

    std::string line;
    while (std::getline(file, line)) {
        // Ignore comments
        size_t commentPos = line.find("//");
        if (commentPos != std::string::npos) {
            line = line.substr(0, commentPos);
        }

        // Ignore empty lines
        if (line.empty()) continue;

        // Parse the key-value pair
        std::istringstream iss(line);
        std::string key, equals;
        iss >> key >> equals;

        // Ensure proper assignment format (key = value)
        if (equals != "=") {
            std::cerr << "Error: Invalid format in line: " << line << std::endl;
            continue;
        }

        // Handle specific variables and raise a warning for unknown ones
        if (key == "Eth.LineRate") {
            iss >> eth.LineRate;
        }
        else if (key == "Eth.CaptureSizeMs") {
            iss >> eth.captureSizeMs;
        }
        else if (key == "Eth.MinNumOfIFGsPerPacket") {
            iss >> eth.minNumOfIFGsPerPacket;
        }
        else if (key == "Eth.DestAddress") {
            iss >> adress;
            for (int i = 0; i < 6; i++) {
                eth.destMacAdress[i] = (adress >> (48 - 8 * (i)) & 0xff);
            }
        }
        else if (key == "Eth.SourceAddress") {
            iss >> adress;
            for (int i = 0; i < 6; i++) {
                eth.srcMacAdress[i] = (adress >> (48 - 8 * (i)) & 0xff);
            }
        }
        else if (key == "Eth.MaxPacketSize") {
            iss >> eth.maxPacketSize;
        }
        else if (key == "Eth.BurstSize") {
            iss >> eth.BurstSize;
        }
        else if (key == "Eth.BurstPeriodicity_us") {
            iss >> eth.BurstPeriodicty_us;
        }
        else {
            std::cerr << "Warning: Unknown variable " << key << " in line: " << line << std::endl;
        }
}

    file.close();

    return eth;
}


void TextParser::WriteWholePacket(std::vector <uint8_t> packet_, uint64_t preamble, uint8_t IFG_val, int numIFG) {
    /*std::ofstream file(dumpFile);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file." << std::endl;
        return;
    }*/
    //WriteFileStream;
    //print preamble and sop
    for (size_t i = 0; i < 4; i++) {
        WriteFileStream << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(preamble >> (64 - (i+1)*8) & 0xff);
    }
    WriteFileStream << std::endl;
    for (size_t i = 4; i < 8; i++) {
        WriteFileStream << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(preamble >> (64 - (i+1) * 8) & 0xff);
    }
    //file << std::hex << static_cast<int>(IFG_val);
    WriteFileStream << std::endl;
    

    // Loop through packet_
    int outerLloop = packet_.size() - (packet_.size() % 4);
    for (size_t i = 0; i < outerLloop; i += 4) {
        for (size_t j = 0; j < 4 && (i + j) < packet_.size(); ++j) {
            WriteFileStream << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(packet_[i + j]);
        }
        WriteFileStream << std::endl;
    }
    //writing the last n bytes
    int extraPacketBytes = packet_.size() % 4;
    for (size_t j = 0; j < extraPacketBytes; j++) {
        WriteFileStream << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(packet_[outerLloop + j]);
    }

    //start writing IFG till they align again
    for (size_t j = 0; j < 4-extraPacketBytes; j++) {
        WriteFileStream << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(IFG_val);
    }
    WriteFileStream << std::endl;


    for (size_t i = 0; i < numIFG-extraPacketBytes; i += 4) {
        for (size_t j = 0; j < 4 && (i + j) < numIFG; ++j) {
            WriteFileStream << std::hex <<   std::setw(2) << std::setfill('0') << static_cast<int>(IFG_val);
        }
        WriteFileStream << std::endl;
    }

   

    //file.close();
}



void TextParser::CloseFileWrite() {
    WriteFileStream.close();
    return;
}

void TextParser::CloseFileRead() {
    readFileStream.close();
    return;
}