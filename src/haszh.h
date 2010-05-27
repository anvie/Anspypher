/*
 *  haszh.h
 *  This file is part of Anspypher project
 *
 *  Created by Robin Marufi on 5/27/10.
 *	Contact: r@nosql.asia
 *  Copyright 2010 Anlab Software. All rights reserved.
 *
 */


#ifndef __HASZH_H__
#define __HASZH_H__

#include <stdint.h>
#include <string>

namespace haszh{
using namespace std;

uint32_t StrHash(const string& str);
uint32_t HaszhIt(const string& str);
const string ToHexadecimal(uint32_t hash);

} // namespace

#endif // __HASZH_H__
