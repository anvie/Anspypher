
#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <string>
#include <vector>

#include "haszh.h"
#include "keyword.h"

using namespace std;

int main(int argc,char** argv)
{

	uint32_t hash1 = haszh::HaszhIt("The sheep jumping over the sun");
	uint32_t hash2 = haszh::HaszhIt("sheep   jumping the sun over the");
	
	printf("hash A1: %08X\n", hash1);
	printf("hash A2: %08X\n", hash2);
	
	hash1 = haszh::HaszhIt("motor yamaha murah");
	hash2 = haszh::HaszhIt("yamaha murah motor");
	
	printf("hash B1: %08X\n", hash1);
	printf("hash B2: %08X\n", hash2);
	
	printf("To hexa test: %d = %s\n", hash1, haszh::ToHexadecimal(hash1).c_str());
	
	string splited_keyword = keyword::SplitCommas("jual honda, - jupiter \" bagus,...mantaf!!!lah v567 112");
	cout << "splited_keyword: " << splited_keyword.c_str() << endl;
	
	return 0;
}


