

ccflags = -O3 -mmacosx-version-min=10.6 -headerpad_max_install_names -Wall -pedantic -Wno-long-long -Wno-variadic-macros
incdir = -I/usr/local/BerkeleyDB.4.8/include -I/usr/local/include -I/opt/local/include -I/opt/hypertable/0.9.3.1/include
libdir = -L/usr/local/BerkeleyDB.4.8/lib -L/opt/local/lib/db46/ -L/opt/hypertable/0.9.3.1/lib -L/opt/local/lib -L/usr/local/lib
libs = -lHypertable -lHyperDfsBroker -lexpat -lHyperspace -ldb_cxx -lHyperTools -lHyperComm -lHyperCommon \
		-lboost_iostreams -lboost_program_options -lboost_filesystem -lboost_thread -lboost_system -llog4cpp.4 -lreadline \
		-lncurses -lz -lsigar-universal-macosx -lpthread

obj = htass.o myass.o haszh.o keyword.o table_scanner.o
target = myass

all: $(target)
	
$(target): $(obj)
	g++ $(ccflags) -Wl,-search_paths_first $(incdir) $(libdir) $(libs) -o myass $^
	
test: test.o haszh.o keyword.o
	g++ $(ccflags) $(incdir) $(libdir) $(libs) -o $@ $^

test.o: test.cpp
	g++ -c $(ccflags) $(incdir) -o $@ $^

myass.o: myass.c
	gcc -c -O3 -o $@ myass.c
	
htass.o: htass.cpp
	g++ -c $(ccflags) $(incdir) -o $@ htass.cpp

keyword.o: keyword.cpp
	g++ -c $(ccflags) $(incdir) -o $@ $^
	
table_scanner.o: table_scanner.cpp table_scanner.h
	g++ -c $(ccflags) $(incdir) -o $@ table_scanner.cpp
	
haszh.o: haszh.cpp
	g++ -c $(ccflags) $(incdir) -o $@ $^

deploy: $(target)
	cp ./myass /opt/hypertable/current/bin/
	
testrun:
ifeq ($(ARG),)
	time /opt/hypertable/current/bin/ht myass "edition shio macan" --verbose --cache-result
else
	time /opt/hypertable/current/bin/ht myass "$(ARG)"
endif

clean:
	rm *.o
	