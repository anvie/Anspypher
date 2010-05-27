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
	
	class PrinterI : public Printer  {
	public:
		virtual void printString(const string& s, const Ice::Current&);
	};
	
	void PrinterI::printString( const string& s, const Ice::Current&)
	{ 
		cout << "data dari client: " << s << endl; 
	}
	
	int serve_daemon( int argc, char** argv)
	{
		
		Ice::CommunicatorPtr ic;
		
		ic = Ice::initialize(argc,argv);
		Ice::ObjectAdapterPtr adapter = ic->createObjectAdapterWithEndpoints("Anspypher","default -p 5000");
		Ice::ObjectPtr object = new PrinterI;
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




