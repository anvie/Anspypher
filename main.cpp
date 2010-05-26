#include <Common/Compat.h>

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

#include <Common/System.h>
#include <Common/Error.h>

#include <Hypertable/Lib/Client.h>
#include <Hypertable/Lib/KeySpec.h>
#include <boost/program_options.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/intrusive_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/format.hpp>

#include "src/defs.h"
#include "src/haszh.h"
#include "src/keyword.h"
#include "src/table_scanner.h"
#include "src/smartptr.h"

using namespace Hypertable;
using namespace std;
namespace popts = boost::program_options;
using boost::intrusive_ptr;

extern "C"
const char* an_bm_matcher(const unsigned int start_pos, const char* lp_source, size_t src_length, 
						  const char* lp_searchfor, size_t searchfor_length);


int main(int argc, char* argv[]) {
	
	
	ClientPtr client;
	TablePtr table_ptr;
	TablePtr cachedTbl;
	ScanSpecBuilder ssb;
	TableMutatorPtr mutator;
	TableScannerPtr scanner;
	KeySpec key;
	string keyword;
	
	int count = 0;
	int total_count = 0;
	
	bool verbose = false;
	bool cache_result = false;
	bool found_by_cache = false;
	
	
	cout << "Anspypher " << Anspypher::VERSION << endl;
	
	
	popts::options_description desc("Options");
	desc.add_options()
	("help","show help")
	("verbose","show result")
	("cache-result","simpan hasil result sebagai cache sebagai index")
	;
	popts::variables_map vm;
	popts::store(popts::parse_command_line(argc,argv,desc),vm);
	popts::notify(vm);
	
	if(vm.count("help")){
		cout << desc << endl;
		return 1;
	}
	verbose = vm.count("verbose") > 0;
	cache_result = vm.count("cache-result");
	
	if ( argc > 1 ){
		keyword = string(argv[1]);
		transform(keyword.begin(),keyword.end(),keyword.begin(),::tolower);
	}else{
		keyword = "blackberry";
	}
	
	string keyword_hash = keyword::GetHash(keyword);
	if(verbose)printf("keyword `%s`, hash: `%s`\n",keyword.c_str(),keyword_hash.c_str());
	
	const char* install_dir = "/opt/hypertable/current/bin/hypertable";
	
	try
	{
		client = new Client( System::locate_install_dir(install_dir) );
		
		printf("Starting...\n");
		
		// search by cache first
		cachedTbl = client->open_table("CachedKeywords");
		
		ssb.clear();
		ssb.add_row_interval(keyword_hash.c_str(),true,keyword_hash.c_str(),true);
		ssb.add_column("rowkey");
		
		scanner = cachedTbl->create_scanner(ssb.get());
		
		Cell cell;
		
		while(scanner->next(cell)){
			
			count++;
			
			list<string> cols;
			cols.push_back("title");
			cols.push_back("url");
			string value((const char*)cell.value,cell.value_len);
			
			TableScannerPtr sdscanner = table_scanner::SearchCells(client,"AnspiderData",value,cols);
			
			if(verbose){
				Cell sdscell;
				while(sdscanner->next(sdscell))
				{	
					printf("==> %s ", sdscell.column_family);
					printf("    %s\n", string((const char*)sdscell.value,sdscell.value_len).c_str());
				}
			}
			
		}
		
		if(count > 0){
			found_by_cache = true;
		}else
		{
			table_ptr = client->open_table("AnspiderData");
			
			ScanSpecBuilder ssb;
			ssb.clear();
			ssb.add_row_interval("anspychunk:",true,"anspychunk\xFF",true);
			ssb.add_column("title");
			
			scanner = table_ptr->create_scanner(ssb.get());
			
			mutator = cachedTbl->create_mutator();	
			
			
			boost::shared_ptr<Cell> cell(new Cell());
			Cells cells;
			std::list< boost::shared_ptr<Cell> > cell_saver;
			Smartptr::ContentSaver<string> content_saver_str;
			int count_to_push = 0;
			
			cells.clear();
			
			cout << "Searching for `" << keyword.c_str() << "`...\n";
			
			while(scanner->next(*cell.get()))
			{
				
				string value((const char*)cell->value,cell->value_len);
				
				transform(value.begin(),value.end(),value.begin(),::tolower);
				
				if(an_bm_matcher(0,value.c_str(),value.size(),keyword.c_str(),keyword.size())){
					//if(verbose){
					//printf("%s\t%s ", cell->row_key,cell->column_family);
					//printf("=> %s\n", value.c_str());
					//}
					
					if(cache_result){
						cell_saver.push_back(cell);
						
						//if(verbose){
						//	printf("caching result... [hash]:key = [%s]:`%s`\n",keyword_hash.c_str(),cell->row_key);
						//}
						Smartptr::WeakData<string>* cell_row_key = new Smartptr::WeakData<string>(cell->row_key,&content_saver_str);
						cells.push_back(
										Cell(
											 (const char*)keyword_hash.c_str(),
											 (const char*)"rowkey",
											 (const char*)0,
											 ::AUTO_ASSIGN,
											 ::AUTO_ASSIGN,
											 (uint8_t*)cell_row_key->d.c_str(),
											 (uint32_t)cell_row_key->d.length(),
											 ::FLAG_INSERT
											 )
										);
						
						Smartptr::WeakData<string>* splited_keywords = 
						new Smartptr::WeakData<string>(keyword::SplitCommas(keyword),&content_saver_str);
						
						cells.push_back(
										Cell(
											 (const char*)keyword_hash.c_str(),
											 (const char*)"keywords",
											 (const char*)0,
											 ::AUTO_ASSIGN,
											 ::AUTO_ASSIGN,
											 (uint8_t*)splited_keywords->d.c_str(),
											 (uint32_t)splited_keywords->d.length(),
											 ::FLAG_INSERT
											 )
										);
					}
					
					++count;
					++count_to_push;
				}
				
				// push ke database kalo udah lebih besar dari 10
				if (count_to_push > 10){
					
					mutator->set_cells(cells);
					
					//cout << content_saver_str.obj_count() << " pushed to db." << endl;
					
					// flush weak data
					content_saver_str.flush();
					cells.clear();
					count_to_push = 0;
				}
				
				
				++total_count;
			}
			
			// push sisanya
			if (count_to_push > 0) {
				mutator->set_cells(cells);
			}
			
			//printf("mau masupin...\n");
			mutator->flush();
			
		}
		
	}
	catch (std::exception &e) {
		cerr << "error: " << e.what() << endl;
		return 1;
	}
	
	if(found_by_cache){
		printf("found (cached) %d items...\n", count);
	}else
	{
		printf("found %d from %d items...\n", count, total_count);
	}
	
	if(!found_by_cache && cache_result && count>0) printf("result has been cached.\n");
	printf("end...\n");
	
	return 0;
}

