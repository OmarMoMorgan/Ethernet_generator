#include "TextParser.h"


TextParser::TextParser(std::string readFile_, std::string dumpFile_) {
	readFile = readFile_;
	dumpFile = dumpFile_;

}

TextParser::TextParser(std::string readFile_, std::string dumpFile_ , std::string IQFile_) {
    readFile = readFile_;
    dumpFile = dumpFile_;
    IQFile = IQFile_;
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

void TextParser::OpenFileIQ(const std::string& filename) {
    if (readIQData.is_open()) {
        return;
    }
    readIQData.open(filename);
    if (!readIQData.is_open()) {
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

//I will do something here that is not optimized 
//but will ignore it for now 
//which is i will add the other data for the ehternet structure 
//this is faster inimplementing but slower 
ethernet_Generation_data TextParser::ReadFromFile() {
    std::ifstream file(readFile);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file." << std::endl;
        //return NULL;
    }

    ethernet_Generation_data eth;
    std::string adress;
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
            eth.destMacAdress = stringToHex(adress); 
            //for (int i = 0; i < 6; i++) {
            //    eth.destMacAdress[i] = (adress >> (48 - 8 * (i)) & 0xff);
            //}
        }
        else if (key == "Eth.SourceAddress") {
            iss >> adress;
            eth.srcMacAdress = stringToHex(adress);
            //for (int i = 0; i < 6; i++) {
            //    eth.srcMacAdress[i] = (adress >> (48 - 8 * (i)) & 0xff);
            //}
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
        //ORAN stuff
        else if (key == "Oran.SCS") {
            iss >> eth.SCS;
        }
        else if (key == "Oran.MaxNrb") {
            iss >> eth.MaxNrb;
        }
        else if (key == "Oran.NrbPerPacket") {
            iss >> eth.NrbPerPacket;
        }
        else if (key == "Oran.PayloadType") {
            iss >> eth.PayloadType;
        }
        else if (key == "Oran.Payload") {
            iss >> eth.Payload;
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

void TextParser::WritePacketTill(std::vector <uint8_t> packet_, uint64_t preamble, uint8_t IFG_val, int numIFG,int stopByte) {
    std::vector<uint8_t> wholeData(packet_.size() +8 + numIFG);

    for (int i = 0; i < 8; i++) {
        wholeData[0] = preamble >> (64 - (i + 1) * 8) & 0xff;
    }

    wholeData.insert(wholeData.end(), packet_.begin(), packet_.end());

    //print till this vecotr gets done 
    int counter = 0;
    int outerLloop = wholeData.size() - (wholeData.size() % 4);
    for (size_t i = 0; i < outerLloop; i += 4) {
        for (size_t j = 0; j < 4 && (i + j) < stopByte; ++j) {
            WriteFileStream << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(packet_[i + j]);
            counter++;
        }
        if (counter >= stopByte) {
            break;
        }
        WriteFileStream << std::endl;
    }

    //get the ifg that needs to be added
    int ifgsend = 4 - (stopByte % 4);
    for (size_t j = 0; j < ifgsend; ++j) {
        WriteFileStream << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(IFG_val);
    }
    WriteFileStream << std::endl;
    return;

    
    
    //for (size_t i = 0; i < 4; i++) {
    //    WriteFileStream << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(preamble >> (64 - (i + 1) * 8) & 0xff);
    //}
    //WriteFileStream << std::endl;
    //for (size_t i = 4; i < 8; i++) {
    //    WriteFileStream << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(preamble >> (64 - (i + 1) * 8) & 0xff);
    //}
    ////file << std::hex << static_cast<int>(IFG_val);
    //WriteFileStream << std::endl;
    //// Loop through packet_
    //int outerLloop = packet_.size() - (packet_.size() % 4);
    //for (size_t i = 0; i < outerLloop; i += 4) {
    //    for (size_t j = 0; j < 4 && (i + j) < packet_.size(); ++j) {
    //        WriteFileStream << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(packet_[i + j]);
    //    }
    //    WriteFileStream << std::endl;
    //}
    ////writing the last n bytes
    //int extraPacketBytes = packet_.size() % 4;
    //for (size_t j = 0; j < extraPacketBytes; j++) {
    //    WriteFileStream << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(packet_[outerLloop + j]);
    //}
    ////start writing IFG till they align again
    //for (size_t j = 0; j < 4 - extraPacketBytes; j++) {
    //    WriteFileStream << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(IFG_val);
    //}
    //WriteFileStream << std::endl;
    //for (size_t i = 0; i < numIFG - extraPacketBytes; i += 4) {
    //    for (size_t j = 0; j < 4 && (i + j) < numIFG; ++j) {
    //        WriteFileStream << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(IFG_val);
    //    }
    //    WriteFileStream << std::endl;
    //}

}


//this function is reposnsible for reading the iq data
std::vector<uint8_t> TextParser::ReadIQData(int numLinesToRead) {
    //the paramter given above is to make sure
    //that we dont get an overlap of lines
    std::vector<uint8_t> dataRead(numLinesToRead*4);
    int linesRead = 0;
    std::string line;
    
    int val1, val2;
    //iss >> decimalValue1 >> decimalValue2;
    while (linesRead < numLinesToRead) {
        while (std::getline(readIQData, line)) {
            //data.push_back(parseDecimalLine(line));
            std::istringstream iss(line);
            iss >> val1 >> val2;
            dataRead[linesRead * 4] = val1 >> 8;
            dataRead[linesRead * 4 + 1] = val1 & 0x0ff;
            dataRead[linesRead * 4 + 2] = val2 >> 8;
            dataRead[linesRead * 4 + 3] = val2 & 0x0ff;
            linesRead++;
            if (linesRead >= numLinesToRead) break;
        }
        // If end of file is reached, reset the stream to start reading again
        if (readIQData.eof()) {
            readIQData.clear();   // Clear EOF flag
            readIQData.seekg(0);  // Rewind the file to the beginning
        }
    }
    return dataRead;

}



void TextParser::CloseFileWrite() {
    WriteFileStream.close();
    return;
}

void TextParser::CloseFileRead() {
    readFileStream.close();
    return;
}

void TextParser::CloseFileIQ() {
    readIQData.close();
    return;
}



void TextParser::WriteVector(std::vector<uint8_t> data) {
    int outerLloop = data.size() - (data.size() % 4);
    for (size_t i = 0; i < outerLloop; i += 4) {
        for (size_t j = 0; j < 4 && (i + j) < data.size(); ++j) {
            WriteFileStream << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(data[i + j]);
        }
        WriteFileStream << std::endl;
    }


    return;
}



//helper funciton to convert hex values 
std::vector<uint8_t> TextParser::stringToHex(const std::string& hexStr) {
    std::vector<uint8_t> result;

    // Remove the "0x" prefix from the string
    std::string cleanHexStr = hexStr.substr(2);

    // Iterate over the string, converting every two characters into a byte
    for (size_t i = 0; i < cleanHexStr.length(); i += 2) {
        std::string byteString = cleanHexStr.substr(i, 2);
        uint8_t byte = static_cast<uint8_t>(std::stoul(byteString, nullptr, 16));
        result.push_back(byte);
    }

    return result;
}