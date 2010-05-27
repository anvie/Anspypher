/*
 *  dbman.h
 *  anspypher
 *
 *  Created by Robin Marufi on 5/27/10.
 *  Copyright 2010 Anlab Software. All rights reserved.
 *
 */

#include <Common/Compat.h>

#include <list>
#include <string>

#include <Common/System.h>
#include <Common/Error.h>

#include <Hypertable/Lib/Client.h>
#include <Hypertable/Lib/KeySpec.h>

#include "smartptr.h"

namespace Anspypher {
	
	using namespace std;
	using namespace Smartptr;

	typedef list<string> DbColName;
	typedef vector<string> DbRecord;
	typedef list< DbRecord > DbRecords;
	typedef Cells OneCell;
	typedef list<OneCell> MultiCell;
	
	class DbManager {
		
	public:
		DbManager();
		virtual ~DbManager(){return;};
	
		void init();
		
		bool insertData(const string& table, const string& row, DbColName& colname, DbRecord& record);
		bool insertData(const string& table, const string& row, DbColName& colname, DbRecords& record);
		void updateData(const string& table, const string& row, DbColName& colname, DbRecords& record);
		bool deleteData(const string& table, const string& row);
		bool createTable(const string& name, const string& schema);
		bool dropTable(const string& name);
		bool tableExists(const string& name);
		bool rowExists(const string& tablename,const string& rowname);
		QuickWeakData<DbRecord>::type findRow(const string& tablename,const string& rowname);
		
		ClientPtr getClient() const { return this->client; };
		
		
	private:
		OneCell DbRecord2Cell(const string& row, DbColName& colname,const DbRecord& dbr);
		MultiCell DbRecords2Cells(const string& row, DbColName& colname, DbRecords& dbrs);
		
	private:
		static ClientPtr client;
	};
	
	inline DbRecord& operator,(DbRecord& a,const string& s)
	{
		a.push_back(s);
	}
	
	inline DbColName& operator,(DbColName& a,const string& s)
	{
		a.push_back(s);
	}
	

}
