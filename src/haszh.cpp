

#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/functional/hash.hpp>
#include <boost/format.hpp>
#include "haszh.h"
#include "keyword.h"

using namespace std;

namespace haszh{

uint32_t StrHash(const string& str)
{
	boost::hash<string> string_hash;
	return string_hash(str);
}

uint32_t HaszhIt(const string& str)
{
	vector<string> strs;
	uint32_t hash = 0;
	
	strs = keyword::Split(str);
	
	for(vector<string>::iterator it=strs.begin();
		it < strs.end();
		++it)
	{
		boost::to_lower(*it);
		if((*it).size()>0){
			hash ^= StrHash(*it);
		}
	}
	return hash;
}

const string ToHexadecimal(uint32_t hash)
{
	boost::format formater("%1$X");
	return str(formater % hash);
}

} // namespace haszh



