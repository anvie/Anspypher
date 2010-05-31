/**
 *
 * Anspypher unit test.
 * copyright (C) 2010 Anlab Software r[4t]nosql.asia
 *
 */


#include <Common/Compat.h>
#include <Common/Init.h>
#include <AsyncComm/Config.h>

#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <gtest/gtest.h>

#include "config.h"
#include "src/haszh.h"
#include "src/keyword.h"
#include "src/dbman.h"

using namespace Hypertable;
using namespace Hypertable::Config;
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

	DbManager dbman;
	
	const string table_name = "UnitTest";
	
	if (dbman.tableExists(table_name)) {
		// drop first
		dbman.dropTable(table_name);
	}
	
}

TEST(Database,ManipulationTest){
	
	using namespace Anspypher;
	
	const string table_name = "UnitTest";
	
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
	DbRecord rec;
	rec["nama"] = "obin";
	rec["alamat"] = "wonosobo";
	
	const string row = "test-ajah";
	
	EXPECT_TRUE(dbman.insertData(table_name,row,&rec));
	
	
	// delete data test
	EXPECT_TRUE(dbman.deleteData(table_name,row));
	
}

TEST(Database,SearchTest)
{
	using namespace Anspypher;
	const string table_name = "UnitTest";
	DbManager dbman;
	
	// insert data test	
	DbRecord rec;
	rec["nama"] = "obin";
	rec["alamat"] = "wonosobo";
	
	string row = "test-ajah";
	
	EXPECT_TRUE(dbman.insertData(table_name,row,&rec));
	
	DbRecords dbrs = dbman.findRow(table_name, row);
	
	// map test
	DbRecord kv;
	
	kv["nama"] = "obin";
	kv["alamat"] = "wonosobo";
	
	row = "another-row";
	
	EXPECT_TRUE(dbman.insertData(table_name,row,&kv));
	
	dbrs = dbman.findRow(table_name, row);
	
	//cout << rec_ptr;
	
	EXPECT_TRUE((*dbrs.at(0).get())["nama"].compare("obin") == 0);
	EXPECT_TRUE((*dbrs.at(1).get())["alamat"].compare("wonosobo") == 0);
	
	//cout << rec_ptr;
	
}

TEST(Database,DropTableTest)
{
	using namespace Anspypher;
	const string table_name = "UnitTest";
	DbManager dbman;
	// drop table test
	EXPECT_TRUE(dbman.dropTable(table_name));
}

void show_info()
{
	cout << "OS: " << SYSTEM_NAME << endl;
	cout << "Machine: " << SYSTEM_MACHINE << endl;
	cout << "Compiler: " << COMPILER_NAME << endl;
	cout << "Hypertable config: " << HYPERTABLE_CONF << endl;
}

int main(int argc,char** argv)
{
	show_info();

	testing::InitGoogleTest(&argc,argv);
	return RUN_ALL_TESTS();
}


