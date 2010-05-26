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

namespace Anspypher {
	
	using namespace std;

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
		
		void insertData(const string& table, const string& row, DbColName colname, DbRecords record);
		void updateData(const string& table, const string& row, DbColName colname, DbRecords record);
		
	private:
		OneCell DbRecord2Cell(const string& row, DbColName& colname,const DbRecord& dbr);
		MultiCell DbRecords2Cells(const string& row, DbColName& colname, DbRecords& dbrs);
		
	private:
		ClientPtr client;
	};
	

}
