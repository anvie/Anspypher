/*
 *  test_raw.cpp
 *  anspypher
 *
 *  Created by Robin Marufi on 6/1/10.
 *	Contact: r@nosql.asia
 *  Copyright 2010 Anlab Software. All rights reserved.
 *
 */


#include <Common/Compat.h>
#include <Common/Init.h>

#include <Hypertable/Lib/Client.h>
#include <Hypertable/Lib/KeySpec.h>

#include <gtest/gtest.h>

using namespace Hypertable;
using namespace std;

TEST(Database,RawTest){
	
	ClientPtr client;
	TablePtr table;
	TablePtr cachedTbl;
	ScanSpecBuilder ssb;
	TableMutatorPtr mutator;
	TableScannerPtr scanner;
	KeySpec key;
	
	
	client = new Client("/opt/hypertable/current","/opt/hypertable/current/conf/hypertable.cfg");
	
	table = client->open_table("UnitTest");
	
	ssb.add_row_interval("raw-test",true,"raw-test",true);
	
	scanner = table->create_scanner(ssb.get());
	
	Cell cell;
	
	while (scanner->next(cell)) {
		cout << cell.column_family << " -> " << string((const char*)cell.value,cell.value_len) << endl;
	}
	
	
}



int main(int argc, char* argv[])
{
	testing::InitGoogleTest(&argc,argv);
	return RUN_ALL_TESTS();
}


