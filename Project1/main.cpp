#include "utilities.h"
#include <iostream>
#include "EthernetPacket.h"
#include "EthernetGenerator.h"

uint32_t calculateCRC(std::vector<uint8_t> payLoad) {
	//this value is defined in standard
	const uint32_t generatorPoly = 0x04C11DB7;

	// Initial CRC value (all ones, 32 bits)
	uint32_t crc = 0xFFFFFFFF;
	for (size_t byteIndex = 0; byteIndex < payLoad.size(); ++byteIndex) {
		// XOR the next byte into the top of the CRC
		crc ^= (static_cast<uint32_t>(payLoad[byteIndex]) << 24);

		// Process each bit in the byte
		for (int bitIndex = 0; bitIndex < 8; ++bitIndex) {
			// If the top bit (MSB) is 1, XOR with the polynomial
			if (crc & 0x80000000) {
				crc = (crc << 1) ^ generatorPoly;
			}
			else {
				crc <<= 1;
			}
		}
	}

	// Finalize the CRC by inverting all the bits
	//crc = ~crc;

	// Ensure CRC is within 32 bits
	//return crc & 0xFFFFFFFF;
	return crc;

}


template <typename T>
void printVector(const std::vector<T>& vec) {
	for (const T& elem : vec) {
		std::cout << elem << " ";
	}
	std::cout << std::endl;
}


//int main()
//{
//
//	//std::vector<uint8_t> srcMacAdress(4, 0);
//	/*for (int x : defaultMacAdress)
//		std::cout << x << " ";*/
//
//	//make a simple class for testing 
//	std::cout << "\n";
//	std::vector<uint8_t> data = {0x02,0x02 ,0x02 ,0x02 ,0x02 };
//	//std::vector<uint8_t> data = { 5 };
//	uint32_t crc = calculateCRC(data);
//	for (int i = 0; i < data.size(); i++) {
//		std::cout << std::hex << data[i];
//	}
//	std::cout  << std::hex << crc;
//
//	std::vector<uint8_t> src = { 0,0,0,0  };
//	std::vector<uint8_t> dest = { 0,1,1,0 };
//	EthernetPacket etherpacket = EthernetPacket(src, dest, 0x02, 5, 0xffff);
//	std::vector<uint8_t> packet = etherpacket.GetPacket();
//	//printVector(packet);
//
//	std::cout << packet.size() << "\n";
//
//	for (size_t i = 0; i < packet.size(); ++i) {
//		std::cout << std::hex << (int)packet[i] << " ";
//	}
//
//
//	return 0;
//}


//int main() {
//	uint16_t original_value = 0xABCD;  // Example 16-bit integer
//
//	// Splitting the 16-bit integer into two 8-bit integers
//	uint8_t low_byte = original_value & 0xFF;        // Get the lower 8 bits
//	uint8_t high_byte = (original_value >> 8) & 0xFF; // Get the upper 8 bits
//
//	// Output the values
//	std::cout << "High Byte: " << std::hex << (int)high_byte << std::endl;
//	std::cout << "Low Byte: " << std::hex << (int)low_byte << std::endl;
//
//	return 0;
//}


//int main() {
//	//std::cout << (float)1e2;
//	//EthernetGenerator generator = EthernetGenerator("first_milestone.txt", "WriteFile.txt");
//	TextParser parser = TextParser("readFile.txt", "WriteFile.txt" , "iq_file.txt");
//	parser.OpenFileIQ("iq_file.txt");
//	std::vector<uint8_t> data = parser.ReadIQData(10);
//	for (int i = 0; i < 9; i++) {
//		std::cout << std::hex <<  data[i];
//	}
//	return 0;
//}

int ORANPacket::frameID = 0;
int main() {
	EthernetGenerator generator = EthernetGenerator("second_milstone.txt", "WriteFile.txt");
	generator.GeneratePacketsWithORAN();

	//TextParser pars = TextParser("readFile.txt", "WriteFile.txt", "iq_file.txt");
	//pars.OpenFileIQ("iq_file.txt");

	//pars.OpenFileIQ("iq_file.txt");
	//std::ifstream readIQData;
	//readIQData.open("iq_file.txt");
	return 0;
}


//int main() {
//	EthernetGenerator generator = EthernetGenerator("first_milestone.txt" , "WriteFile.txt");
//	generator.GeneratePacketsDump();
//
//
//	return 0;
//}


//#include <iostream>
//#include <vector>
//#include <winsock2.h>    // Winsock header
//#include <ws2tcpip.h>    // For inet_ntop
//#pragma comment(lib, "Ws2_32.lib")  // Link with Ws2_32.lib
//
//#define BUFFER_SIZE 1024  // Buffer size for receiving data
//
//int main() {
//    // Server details
//    const int server_port = 8080;   // Example port
//
//    // Step 1: Initialize Winsock
//    WSADATA wsaData;
//    int wsaStartupResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
//    if (wsaStartupResult != 0) {
//        std::cerr << "WSAStartup failed: " << wsaStartupResult << std::endl;
//        return -1;
//    }
//
//    // Step 2: Create socket
//    SOCKET server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//    if (server_socket == INVALID_SOCKET) {
//        std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
//        WSACleanup();
//        return -1;
//    }
//
//    // Step 3: Bind the socket to a port
//    struct sockaddr_in server_address;
//    server_address.sin_family = AF_INET;
//    server_address.sin_addr.s_addr = INADDR_ANY;  // Listen on all interfaces
//    server_address.sin_port = htons(server_port); // Convert port to network byte order
//
//    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == SOCKET_ERROR) {
//        std::cerr << "Bind failed: " << WSAGetLastError() << std::endl;
//        closesocket(server_socket);
//        WSACleanup();
//        return -1;
//    }
//
//    // Step 4: Listen for incoming connections
//    if (listen(server_socket, SOMAXCONN) == SOCKET_ERROR) {
//        std::cerr << "Listen failed: " << WSAGetLastError() << std::endl;
//        closesocket(server_socket);
//        WSACleanup();
//        return -1;
//    }
//
//    std::cout << "Server is listening on port " << server_port << std::endl;
//
//    // Step 5: Accept a client connection
//    SOCKET client_socket;
//    struct sockaddr_in client_address;
//    int client_address_len = sizeof(client_address);
//    client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_address_len);
//    if (client_socket == INVALID_SOCKET) {
//        std::cerr << "Accept failed: " << WSAGetLastError() << std::endl;
//        closesocket(server_socket);
//        WSACleanup();
//        return -1;
//    }
//
//    std::cout << "Client connected!" << std::endl;
//
//    // Step 6: Receive data from the client
//    char buffer[BUFFER_SIZE];
//    int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
//    if (bytes_received == SOCKET_ERROR) {
//        std::cerr << "Receive failed: " << WSAGetLastError() << std::endl;
//        closesocket(client_socket);
//        closesocket(server_socket);
//        WSACleanup();
//        return -1;
//    }
//
//    std::cout << "Data received from client: " << bytes_received << " bytes" << std::endl;
//
//    // Step 7: Interpret the data as a vector of integers
//    std::vector<int> received_data(bytes_received / sizeof(int));
//    std::memcpy(received_data.data(), buffer, bytes_received);
//
//    // Step 8: Display the received integers
//    std::cout << "Received integers:" << std::endl;
//    for (int value : received_data) {
//        std::cout << value << " ";
//    }
//    std::cout << std::endl;
//
//    // Step 9: Close the client socket and clean up
//    closesocket(client_socket);
//    closesocket(server_socket);
//    WSACleanup();
//
//    return 0;
//}
