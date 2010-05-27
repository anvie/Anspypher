

#ifndef __ANSPYPHER_ICE__
#define __ANSPYPHER_ICE__

module AnspypherIce {
       
       ["python:protobuf:anspypher_pb2.SearchResponse"] sequence<byte> SearchResponse;
       
       interface Printer {
       		 	  void printString(string s);
			  };
			  
			  interface Transporter {
			  	    void sendSR(SearchResponse sr);
				    };
				    
};

#endif // __ANSPYPHER_ICE__
