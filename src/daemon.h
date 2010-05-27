/*
 *  daemon.h
 *  anspypher
 *
 *  Created by Robin Marufi on 5/27/10.
 *	Contact: r@nosql.asia
 *  Copyright 2010 Anlab Software. All rights reserved.
 *
 */


#ifndef __DAEMON_H__
#define __DAEMON_H__

#include <Ice/Ice.h>


namespace Anspypher {
	
	int serve_daemon( int argc, char** argv);
	
}


#endif // __DAEMON_H__

