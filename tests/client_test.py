

import sys

sys.path += ["/opt/Ice-3.4/python", "../gen"]

import traceback, Ice

Ice.loadSlice('anspypher_py.ice')
import AnspypherIce
import anspypher_pb2 as pb


try:
	communicator = Ice.initialize(sys.argv)
	trans = AnspypherIce.TransporterPrx.checkedCast(communicator.stringToProxy("Anspypherd:tcp -p 5000"))
	# hello.printString("hai aku di sini...")
	# for x in xrange(0,10):
	# 	hello.printString("hai aku di sini... %d" % x)
	
	srs = pb.SearchResponse()
	
	r1 = srs.search_results.add()
	r2 = srs.search_results.add()
	
	r1.title = "Motor Vespa tua"
	r1.url = "http://www.ansvia.com/m/motor-tua.html"
	r1.snippet = "ini baru keren!"
	
	r2.title = "Harley full modif"
	r2.url = "http://www.ansvia.com/m/harley-modif.html"
	r2.snippet = "ini baru keren!"
	
	trans.sendSR(srs);
	
	communicator.destroy()
except:
	traceback.print_exc()
	sys.exit(1)




