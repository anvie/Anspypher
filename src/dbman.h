/*
 *  dbman.h
 *  anspypher
 *
 *  Created by Robin Marufi on 5/27/10.
 *	Contact: r@nosql.asia
 *  Copyright 2010 Anlab Software. All rights reserved.
 *
 */

#include <Common/Compat.h>

#include <list>
#include <string>
#include <map>
#include <stdint.h>

#include <Common/System.h>
#include <Common/Error.h>
#include <Common/ReferenceCount.h>

#include <Hypertable/Lib/Client.h>
#include <Hypertable/Lib/KeySpec.h>
#include <boost/utility.hpp>
#include <boost/intrusive_ptr.hpp>

#include "defs.h"
#include "smartptr.h"

namespace Anspypher {
	
	using namespace std;
	using namespace Smartptr;
	using namespace boost;

	typedef Cells OneCell;
	typedef list<OneCell> MultiCell;
	
	
	class DbRecord : public ReferenceCount {
	public:
		DbRecord():null_str(""){}
		string& operator[](const string& key)
		{
			return d[key];
		}
		
		const string& operator[](int key)
		{
			map<string,string>::iterator it;
			int i = 0;
			for (it=d.begin(); it!=d.end(); it++) {
				if (i == key++){
					return (*it).second;
				}
			}
			return null_str;
		}
		
		map<string,string>::iterator
		begin(){return d.begin();}
		
		map<string,string>::iterator
		end(){return d.end();}
		
		typedef map<string,string>::iterator iterator;
		
	private:
		map<string,string> d;
		const string null_str;
	};
	
	
	class DbColName
	{
	public:
		typedef vector<string>::iterator iterator;
		
		DbColName(){};
		DbColName(DbRecord* dbr){ init(dbr); }
		DbColName(DbRecord& dbr){ init(&dbr); }
		void init(DbRecord* dbr)
		{
			DbRecord::iterator it;
			for (it=dbr->begin(); it!=dbr->end(); ++it) {
				const string key = (*it).first;
				if (!this->contains(key)) {
					d.push_back(key);
				}
			}
		}
		bool contains(const string& k)
		{
			vector<string>::iterator it;
			for (it=d.begin(); it!=d.end(); ++it) {
				if ((*it).compare(k) == 0) {
					return true;
				}
			}
			return false;
		}
		const string& operator[](int i){ return d.at(i); }
		iterator begin(){ return d.begin(); }
		iterator end(){ return d.end(); }
		friend DbColName& operator<<(DbColName& a,const string& b);
	private:
		vector<string> d;
	};
	
	inline DbColName& operator<<(DbColName& a,const string& b)
	{
		a.d.push_back(b);
		return a;
	}
	
	typedef intrusive_ptr< DbRecord > DbRecordPtr;
	typedef vector< DbRecordPtr > DbRecords;
	
	
	
	class DbManager : boost::noncopyable {
		
	public:
		
		DbManager(){
		  init("/opt/hypertable/current",HYPERTABLE_CONF);
		}
		DbManager(const string& install_dir, const string& config_file){
			init(install_dir, config_file);
		}
		virtual ~DbManager(){return;};
	
		void init(const string& install_dir,const string& config_file);
		
		bool insertData(const string& table, const string& row, DbRecord* record);
		bool insertData(const string& table, const string& row, DbRecords& record);
	
		bool insertDataCell(const string& table, const string& row, const OneCell& cell);
		
		void updateData(const string& table, const string& row, DbColName& colname, DbRecords& record);
		bool deleteData(const string& table, const string& row);
		
		bool createTable(const string& name, const string& schema);
		bool dropTable(const string& name, bool check_result);
		bool dropTable(const string& name);
		bool tableExists(const string& name);
		
		bool rowExists(const string& tablename,const string& rowname);
		
		DbRecords findRow(const string& table,const string& row, size_t limit=1);
		DbRecords findData(const string& table, const string& row, const string& col);
		
		/**
		 * get native client.
		 */
		ClientPtr getClient() const { return this->client; };
		
		
	private:
		OneCell DbRecord2Cell(const string& row, DbRecord* dbr);
		MultiCell DbRecords2Cells(const string& row, DbRecords& dbrs);
		
	private:
		static ClientPtr client;
	};
	

}
