/*
 *  dbman.cpp
 *  The part of Anspypher project.
 *
 *  Created by Robin Marufi on 5/27/10.
 *	Contact: r@nosql.asia
 *  Copyright 2010 Anlab Software. All rights reserved.
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
	
	/**
	 * Inisialisasi Database Client
	 * static global.
	 */
	void DbManager::init()
	{
		if (client.get() == 0) {
			client = new Client();
		}
	}
	
	/**
	 * To insert data.
	 * \param table a table name.
	 * \param row a row name used as key.
	 * \param colname is a column family name.
	 * \param record is a record to insert.
	 * \return boolean. true if success otherwise false.
	 * \sa insertData(const string& tablename, const string& row, DbColName& colname, DbRecords& records)
	 */
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
	
	/**
	 * Same as insertData(const string& table, const string& row, DbColName& colname, DbRecord& record)
	 * but with multiple records
	 * \param tablename a table name.
	 * \param row a row name.
	 * \param colname a column name.
	 * \param records list of DbRecord.
	 */
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
	
	
	/**
	 * To delete a single data.
	 * \param table a table name where data reside.
	 * \param row row name to delete. warning! all column family will deleted also.
	 * \return boolean, true if succeed otherwise false.
	 */
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
	
	// test
	/**
	 * Create a table.
	 * \param name a table name to create.
	 * \param schema a XML schema to used for table creation.
	 * \return boolean, true if succeed otherwise false.
	 */
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

	/**
	 * Delete a table.
	 * \param name a table name to delete.
	 * \param check_result if set to false then no error checking performed
	 *				so return always false. set to true to return a status.
	 * \return boolean always false if check_result parameter is set to false, if not
	 *				return true if succeed otherwise false.
	 */
	bool DbManager::dropTable(const string& name, bool check_result)
	{
		assert(client);
		client->drop_table( name, true );
		if (check_result) {
			return !tableExists(name);
		}
		return false;
	}
	
	//! same as dropTable(const string& name, bool check_result) \n but will always check a result
	bool DbManager::dropTable(const string& name)
	{
		assert(client);
		return dropTable(name, true);
	}

	/**
	 * Check if table already exists in database.
	 * \param name table name.
	 * \return boolean. true if succeed otherwise false.
	 */
	bool DbManager::tableExists(const string& name)
	{
		bool rv = false;
		try {
			rv = client->get_table_id( name ) > 0;
		}
		catch (Hypertable::Exception&){}
		return rv;
	}
	
	/**
	 * Find a row.
	 * \param tablename name of table to find.
	 * \param rowname name of row.
	 * \return DbRecord.
	 */
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
	
	/**
	 * Check if row exists.
	 * \param tablename table name.
	 * \param rowname row name.
	 * \return boolean.
	 */
	bool DbManager::rowExists(const string& tablename,const string& rowname)
	{
		return findRow(tablename,rowname)->d->size() > 0;
	}
}





