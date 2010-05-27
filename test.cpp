/**
 *
 * Anspypher unit test.
 * copyright (C) 2010 Anlab Software r[4t]nosql.asia
 *
 */



#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <string>
#include <vector>
#include <gtest/gtest.h>

#include "src/haszh.h"
#include "src/keyword.h"
#include "src/dbman.h"


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

TEST(Database,ConnectionTest){
	
	using namespace Anspypher;
	
	bool rv = false;
	DbManager dbman;
	
	// create table test
	EXPECT_TRUE(
	dbman.createTable("UnitTest","<Schema>"
											"<AccessGroup name=\"default\">"
											"<ColumnFamily>"
											"<Name>nama</Name>"
											"<deleted>false</deleted>"
											"</ColumnFamily>"
											"<ColumnFamily>"
											"<Name>alamat</Name>"
											"<deleted>false</deleted>"
											"</ColumnFamily>"
											"</AccessGroup>"
											"</Schema>")
	);
	
	// insert data test
	DbColName colname;
	colname , "nama", "alamat";
	
	DbRecord rec;
	rec , "obin", "wonosobo";
	
	cout << colname.front() << " " << colname.back() << endl;
	
	const string table_name = "UnitTest";
	const string row = "test-ajah";
	
	EXPECT_TRUE(dbman.insertData(table_name,row,colname,rec));
	
	// delete data test
	EXPECT_TRUE(dbman.deleteData(table_name,row));
	
}

TEST(Database,ConnectionTest2)
{
	using namespace Anspypher;
	const string table_name = "UnitTest";
	DbManager dbman;
	// drop table test
	EXPECT_TRUE(dbman.dropTable(table_name));
}




int main(int argc,char** argv)
{
	testing::InitGoogleTest(&argc,argv);
	return RUN_ALL_TESTS();
}


