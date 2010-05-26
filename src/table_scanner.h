

#ifndef __TABLE_SCANNER_H__
# define __TABLE_SCANNER_H__

#include <Common/ReferenceCount.h>

#include <string>
#include <vector>
#include <list>

#include <Hypertable/Lib/Client.h>
#include <Hypertable/Lib/KeySpec.h>
#include <boost/intrusive_ptr.hpp>

namespace table_scanner{
	
	using namespace boost;
	using namespace std;
	using namespace Hypertable;

	TableScannerPtr SearchCells(ClientPtr client,const string& tbname,const string& rowkey,list<string>& columns);
	
}

using boost::intrusive_ptr;

template <class T> class WeakData;

template <class T>
class ContentSaver{
public:
	
	~ContentSaver(){
		printf("deleting %d refs...\n",ds.size());
	}
	
	void save(intrusive_ptr< WeakData<T> > d){ds.push_back(d);}
	
	std::list< intrusive_ptr< WeakData<T> > > ds;
};

template <class T>
class WeakData: public ReferenceCount
{
public:
	WeakData(T d_){};
	WeakData(T d_,ContentSaver<T>* cs)
	: d(d_)
	{ 
		cs->save(this); 
	}
	//~WeakData()
	//{
	//	printf("item count deleted...\n");
	//}
	T d;
};

#endif // __TABLE_SCANNER_H__

