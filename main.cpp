

#include <cstdio>
#include <iostream>

#include "src/defs.h"
#include "src/daemon.h"

using namespace std;

int main(int argc,char** argv)
{
	
	cout << "Anspypher " << Anspypher::VERSION << endl;
	
	Anspypher::serve_daemon(argc, argv);
	
	return 0;
}

