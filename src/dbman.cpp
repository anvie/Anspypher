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
	
	ClientPtr DbManager::client = 0;
	
	DbManager::DbManager()
	{
		init();
	}
	
	void DbManager::init()
	{
		if (client.get() == 0) {
			client = new Client();
		}
	}
	
	bool DbManager::insertData(const string& table, const string& row, DbColName& colname, DbRecord& record)
	{
		TablePtr tbl;
		TableMutatorPtr mutator;
		
		assert(client);
		
		try {
			tbl = client->open_table(table.c_str());
		}
		catch (Hypertable::Exception e) {
			cerr << "Cannot open table. " << e.what() << endl;
			return false;
		}
		
		mutator = tbl->create_mutator();
		
		OneCell cell = this->DbRecord2Cell(row, colname, record);
		
		mutator->set_cells(cell);
		
		mutator->flush();
		
		return !mutator->need_retry();		
	}
	
	bool DbManager::insertData(const string& tablename, const string& row, DbColName& colname, DbRecords& records)
	{
		TablePtr table;
		TableMutatorPtr mutator;
		
		assert(client);
		
		table = client->open_table(tablename.c_str());
		
		mutator = table->create_mutator();
		
		MultiCell cells = this->DbRecords2Cells(row, colname, records);
		
		MultiCell::iterator it;
		
		for (it = cells.begin(); it != cells.end(); ++it) {
			mutator->set_cells( *it );
		}
		
		mutator->flush();
		
		return !mutator->need_retry();
	}
	
	bool DbManager::deleteData(const string& table, const string& row)
	{
		TablePtr tbl = client->open_table(table.c_str());
		TableMutatorPtr mutator = tbl->create_mutator();
		KeySpec ks((const char*)row.c_str(),(const char*)0,(const char*)0,::AUTO_ASSIGN);
		mutator->set_delete(ks);
		return !mutator->need_retry();
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
	
	bool DbManager::createTable(const string& name, const string& schema)
	{
		assert(client);
		try {
			client->create_table( name, schema );
		}
		catch (Hypertable::Exception e) {
			return false;
		}
		return true;
	}
	
	bool DbManager::dropTable(const string& name)
	{
		assert(client);
		client->drop_table( name, true );
		return !tableExists( name );
	}
	
	bool DbManager::tableExists(const string& name)
	{
		bool rv = false;
		try {
			rv = client->get_table_id( name ) > 0;
		}
		catch (Hypertable::Exception&){}
		return rv;
	}
	
	QuickWeakData<DbRecord>::type DbManager::findRow(const string& tablename,const string& rowname)
	{
		TablePtr tbl = client->open_table(tablename.c_str());
		
		ScanSpecBuilder ssb; ssb.clear();
		ssb.add_row_interval(rowname.c_str(), true, rowname.c_str(), true);
		TableScannerPtr scanner = tbl->create_scanner(ssb.get());
		
		QuickWeakData<DbRecord>::type dbrs = new QuickWeakData<DbRecord>(new DbRecord());
		Cell cell;
		
		while (scanner->next(cell)) {
			dbrs->d->push_back(string((const char*)cell.value,cell.value_len));
		}
		return dbrs;
	}
	
	bool DbManager::rowExists(const string& tablename,const string& rowname)
	{
		return findRow(tablename,rowname)->d->size() > 0;
	}
}





