/* 
 * File:   util.h
 * Author: lubos
 *
 * Created on May 31, 2012, 4:30 PM
 */

#ifndef UTIL_H
#define	UTIL_H

#include <stdlib.h>
#include <cstdio>


#define SAFE_DELETE( p )       { if( p ) { delete ( p );     ( p ) = NULL; } }
#define SAFE_DELETE_ARRAY( p ) { if( p ) { delete[] ( p );   ( p ) = NULL; } }


void printMessage(const char* msg, const char* data = NULL);    

#endif	/* UTIL_H */

