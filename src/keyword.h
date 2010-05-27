/*
 *  keyword.h
 *  Part of Anspypher project
 *
 *  Created by Robin Marufi on 5/27/10.
 *	Contact: r@nosql.asia
 *  Copyright 2010 Anlab Software. All rights reserved.
 *
 */

#ifndef __KEYWORD_H__
# define __KEYWORD_H__

namespace keyword{
	
	using namespace std;
	
	const string SplitCommas(const string& str);
	vector<string> Split(const string str);
	const string GetHash(const string str);	
}

#endif // __KEYWORD_H__
