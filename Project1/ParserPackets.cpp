#include "ParserPackets.h"


ParserPackets::ParserPackets() {
	//0xFB555555555555D
	PreambleVec.push_back(0xFB);
	PreambleVec.push_back(0x55);
	PreambleVec.push_back(0x55);
	PreambleVec.push_back(0x55);
	PreambleVec.push_back(0x55);
	PreambleVec.push_back(0x55);
	PreambleVec.push_back(0x55);
	PreambleVec.push_back(0x5D);
}

bool ParserPackets::CompareEthernetHeader(const std::vector<uint8_t> &data, int start) {
	for (int i = 0; i < 8; i++) {
		if (data[i + start] != PreambleVec[i]) {
			return false;
		}
	}
	return true;
}

std::vector<uint8_t> ParserPackets::GetMacAdress(const std::vector<uint8_t>& data, int pos) {
	std::vector<uint8_t> adress;
	for (int i = 0; i < 6; i++) {
		adress.push_back(data[pos]);
	}
	return adress;
}

void ParserPackets::GetHeaders(std::vector<uint8_t> data) {
	int dataSize = data.size();
	for (int i = 0; i < dataSize; i++) {
		if (CompareEthernetHeader(data, i)) {
			destMac = GetMacAdress(data, i);
			i += 6;
			srcMac = GetMacAdress(data, i);

			//get the ether Type
			etherType = (data[i] << 8) | data[ i + 1 ];
			i++;

			//get the other ecpri header 
			//ignore it for now just get the payload size 
			OranPacketSize = data[i + 2] << 8 | data[i + 3];

			//get the seqid
			ecpriSeqId = data[i + 6];

			i += 8;

			i += OranPacketSize;

			for (int j = 0; j < 4; j++) {
				CRC.push_back(data[j]);
			}
			i += 4;

			if (data[i] == 7) {
				i++;
			}

		}
		return;
	}
}
