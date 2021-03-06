/*
 *  matcher.h
 *  anspypher
 *
 *  Created by Robin Marufi on 5/27/10.
 *	Contact: r@nosql.asia
 *  Copyright 2010 Anlab Software. All rights reserved.
 *
 */

#ifndef __MATCHER_H__
#define __MATCHER_H__

#include <stdlib.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif 
const unsigned char* an_bm_matcher(const unsigned int start_pos, const char* lp_source, size_t src_length, 
						  const char* lp_searchfor, size_t searchfor_length);
#ifdef __cplusplus
}
#endif

#endif 

