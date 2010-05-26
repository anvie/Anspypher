

#ifndef __ANSPYPHER_ICE__
#define __ANSPYPHER_ICE__

module Demo {
	
	["cpp:protobuf:anspypher::SearchResponse"] sequence<byte> SearchResponse;
	
	interface Printer {
       		 void printString(string s);
	};
	interface Transporter {
		void sendSR(SearchResponse sr);
	};
	
};

#endif // __ANSPYPHER_ICE__
