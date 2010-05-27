

#ifndef __SMARTPTR_H__
# define __SMARTPTR_H__

#include <Common/ReferenceCount.h>

#include <string>
#include <list>

#include <boost/intrusive_ptr.hpp>

namespace Smartptr {

	using boost::intrusive_ptr;

	template <class T> class WeakData;

	template <class T>
	class ContentSaver{
	public:

		~ContentSaver(){
			return; //printf("deleting %d refs...\n",ds.size());
		}

		void save(intrusive_ptr< WeakData<T> > d){ds.push_back(d);}
		void flush(){ ds.clear(); }
		size_t obj_count(){ return ds.size(); }

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
	
	template <class T>
	class QuickWeakData : public ReferenceCount
	{
	public:
		typedef intrusive_ptr< QuickWeakData > type;
		QuickWeakData(T* d_): d(d_){};
		~QuickWeakData(){ delete d; }
		T* d;
	};

	
} // namespace


#endif // __SMARTPTR_H__

