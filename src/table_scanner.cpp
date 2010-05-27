/*
 *  table_scanner.cpp
 *  This file is part of Anspypher project
 *
 *  Created by Robin Marufi on 5/27/10.
 *	Contact: r@nosql.asia
 *  Copyright 2010 Anlab Software. All rights reserved.
 *
 */

#include <Common/Compat.h>

#include <iostream>
#include "table_scanner.h"

using namespace boost;
using namespace std;
using namespace Hypertable;

namespace table_scanner{

TableScannerPtr SearchCells(ClientPtr client,const string& tbname,const string& rowkey,list<string>& columns)
{
	TablePtr tbl = client->open_table(tbname.c_str());
	
	ScanSpecBuilder m_ssb;
	m_ssb.clear();
	m_ssb.add_row_interval(rowkey.c_str(),true,rowkey.c_str(),true);
	//m_ssb.set_max_versions(3);
	
	for(list<string>::iterator it=columns.begin();
		it != columns.end();
		++it)
	{
		m_ssb.add_column((*it).c_str());
	}
	
	TableScannerPtr scanner = tbl->create_scanner(m_ssb.get());
	return scanner;
}


} // namespace


