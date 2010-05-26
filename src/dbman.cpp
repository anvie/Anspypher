/*
 *  dbman.cpp
 *  anspypher
 *
 *  Created by Robin Marufi on 5/27/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "dbman.h"



namespace Anspypher {
	
	
	using namespace Hypertable;
	using namespace std;
	namespace popts = boost::program_options;
	
	DbManager::DbManager()
	{
		
	}
	
	void DbManager::init()
	{
		client = new Client();
	}
	
	void DbManager::insertData(const string& tablename, const string& row, DbColName colname, DbRecords record)
	{
		TablePtr table;
		TableMutatorPtr mutator;
		Cell cell;
		
		assert(client);
		
		table = client->open_table(tablename.c_str());
		
		mutator = table->create_mutator();
		
		MultiCell cells = this->DbRecords2Cells(row, colname, record);
		
		MultiCell::iterator it;
		
		for (it = cells.begin(); it != cells.end(); ++it) {
			mutator->set_cells( *it );
		}
		
		mutator->flush();
		
	}
	
	OneCell DbManager::DbRecord2Cell(const string& row, DbColName& colname,const DbRecord& dbr)
	{
		OneCell onec;
		int i = 0;
		DbColName::iterator it;
		for(it = colname.begin(); it != colname.end(); ++it)
		{
			onec.push_back(
									Cell(
											 (const char*)row.c_str(),
											 (const char*)(*it).c_str(),
											 (const char*)0,
											 ::AUTO_ASSIGN,
											 ::AUTO_ASSIGN,
											 (uint8_t*)dbr[i].c_str(),
											 (uint32_t)dbr[i].length(),
											 ::FLAG_INSERT
											 )
			);
			++i;
		}
		return onec;
	}
	
	MultiCell DbManager::DbRecords2Cells(const string& row, DbColName& colname, DbRecords& dbrs)
	{
		MultiCell cells;
		DbRecords::iterator it;
		for( it = dbrs.begin(); it != dbrs.end(); ++it )
		{
			cells.push_back(this->DbRecord2Cell(row,colname,*it));
		}
		return cells;
	}
	
}

