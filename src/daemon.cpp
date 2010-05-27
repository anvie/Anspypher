/*
 *  daemon.cpp
 *  anspypher
 *
 *  Created by Robin Marufi on 5/27/10.
 *	Contact: r@nosql.asia
 *  Copyright 2010 Anlab Software. All rights reserved.
 *
 */


#include "defs.h"
#include "daemon.h"
#include <gen/anspypher.pb.h>
#include <gen/anspypher.h>


namespace Anspypher {
	
	using namespace std;
	using namespace AnspypherIce;
	using namespace AnspypherPb;
	
	class PrinterI : public AnspypherIce::Printer  {
	public:
		virtual void printString(const string& s, const Ice::Current& );
	};
	
	class TransporterI : public AnspypherIce::Transporter {
	public:
		virtual void sendSR( const SearchResponse& sr, const Ice::Current& );
	};
	
	void PrinterI::printString( const string& s, const Ice::Current& )
	{ 
		cout << "data dari client: " << s << endl; 
	}
	
	void TransporterI::sendSR( const SearchResponse& srs, const Ice::Current& )
	{
		cout << "Search response accepted:" << endl;

		for (int i=0; i < srs.search_results_size(); ++i) {
			const SearchResult& sr = srs.search_results(i);
			cout << i+1 << " " << sr.title() << endl;
			cout << "  " << sr.snippet() << endl;
			cout << "  " << sr.url() << endl;
		}
	}
	
	int serve_daemon( int argc, char** argv)
	{
		
		Ice::CommunicatorPtr ic;
		
		ic = Ice::initialize(argc,argv);
		Ice::ObjectAdapterPtr adapter = ic->createObjectAdapterWithEndpoints("Anspypher","default -p 5000");
		Ice::ObjectPtr object = new TransporterI;
		adapter->add(object,ic->stringToIdentity("Anspypherd"));
		adapter->activate();
		ic->waitForShutdown();
		
		if (ic) {
			try {
				ic->destroy();
			}
			catch (const Ice::Exception& e) {
				cerr << e << endl;
			}
		}
		return 0;
	}
	
}




