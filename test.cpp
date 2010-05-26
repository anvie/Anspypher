
#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <string>
#include <vector>
#include <gtest/gtest.h>

#include "src/haszh.h"
#include "src/keyword.h"


using namespace std;


TEST(CoreEngine,Hashing){
	uint32_t hash1 = haszh::HaszhIt("The sheep jumping over the sun");
	uint32_t hash2 = haszh::HaszhIt("sheep   jumping the sun over the");
	
	EXPECT_EQ(hash1,hash2);
	
	hash1 = haszh::HaszhIt("motor yamaha murah");
	hash2 = haszh::HaszhIt("yamaha murah motor");
	
	EXPECT_EQ(hash1,hash2);
	
	string hexaconv = haszh::ToHexadecimal(hash1).c_str();
	EXPECT_EQ(hexaconv.compare("2FABDF14"),0);
	
	string splited_keyword = keyword::SplitCommas("jual honda, - jupiter \" bagus,...mantaf!!!lah v567 112");
	EXPECT_EQ(splited_keyword.compare("jual,honda,jupiter,bagus,mantaf,lah,v567,112"),0);
}

int main(int argc,char** argv)
{
	testing::InitGoogleTest(&argc,argv);
	return RUN_ALL_TESTS();
}


