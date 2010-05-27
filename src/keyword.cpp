/*
 *  keyword.cpp
 *  This file is part of Anspypher project
 *
 *  Created by Robin Marufi on 5/27/10.
 *	Contact: r@nosql.asia
 *  Copyright 2010 Anlab Software. All rights reserved.
 *
 */

#include <vector>
#include <string>
#include <boost/algorithm/string.hpp>
#include "keyword.h"
#include "haszh.h"

using namespace std;

namespace keyword{
	
const string SplitCommas(const string& str)
{
	vector<string> strs;
	//string str_tmp = boost::erase_all_copy(str,boost::is_any_of(",.-`'\"!@#$%^&*()[]}{}=+-/\\?"));
	strs = Split(str);
	for(vector<string>::iterator it = strs.begin();
		it<strs.end();
		++it)
		{
			if((*it).size()==0){
				strs.erase(it);
				--it;
			}
		}
	return boost::join(strs,",");
}

vector<string> Split(const string str)
{
	vector<string> strs;
	//string str_tmp = boost::erase_all_copy(str,boost::is_any_of(",.-`'\"!@#$%^&*()[]}{}=+-/\\?"));
	boost::split(strs,str,boost::is_any_of(" ,.-`'\"!@#$%^&*()[]}{}=+-/\\?;:"));
	for(vector<string>::iterator it = strs.begin();
		it<strs.end();
		++it)
	{
		if((*it).size()==0){
			strs.erase(it);
			--it;
		}
	}
	return strs;
}

const string GetHash(const string str)
{
	return haszh::ToHexadecimal(haszh::HaszhIt(str));
}

} // namespace keyword
