/*
 *  matcher.c
 *  anspypher
 *
 *  Created by Robin Marufi on 5/27/10.
 *	Contact: r@nosql.asia
 *  Copyright 2010 Anlab Software. All rights reserved.
 *
 */

#include "matcher.h"




#include "matcher.h"


#define		EAN8(d)		(*(unsigned char*)(d))
static volatile unsigned int shift_table[256];

const unsigned char* an_bm_matcher( const unsigned int start_pos, const char* lp_source, size_t src_length, 
						  const char* lp_searchfor, size_t searchfor_length )
{
	
	unsigned char* rv=0;
	unsigned int cval;
	unsigned int i;
	unsigned char* el;
	
	if ( searchfor_length < 1L ) return ( 0 );
	
	el=(unsigned char*)((lp_source + src_length) - searchfor_length);
	
	// persiapkan shift_table
	
	for(i=0;i<(sizeof( shift_table ) / sizeof(unsigned int));i++)
		shift_table[i]=searchfor_length;
	
	// load decending count
	
	unsigned char* lps=(unsigned char*)lp_searchfor;
	unsigned int dcv=(unsigned int)searchfor_length;
	
	while(dcv--)
	{
		
		shift_table[(int32_t)EAN8(lps++)] = dcv;
		
	}
	
	cval = (unsigned int)(searchfor_length-1);
	unsigned int tcval = cval;
	
	unsigned char* lpsrc = (unsigned char*) ( lp_source + start_pos  );
	unsigned char* lpsf = (unsigned char*) lp_searchfor;
	
	for(;;)
	{
		
		if ( EAN8((lpsrc+tcval)) != EAN8((lpsf+tcval)) )
		{
			
			lpsrc++;
			
			if ( searchfor_length == shift_table[(unsigned long)EAN8((unsigned long)(lpsrc+tcval))])
			{
				lpsrc+=tcval;
			}
			
			tcval=cval;
			
			if ( lpsrc <= el ) continue;
			
			return ( 0 );
			
		}
		else
		{
			tcval--;
			if ( (signed)tcval > -1 ) continue;
			
			rv = (unsigned char*) lpsrc;
			break;
		}
	}
	return ( rv );
	
}

