#include "utilities.h"
#include <iostream>

int main()
{

	//std::vector<uint8_t> srcMacAdress(4, 0);
	for (int x : defaultMacAdress)
		std::cout << x << " ";

	return 0;
}