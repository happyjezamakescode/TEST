//===================================================================
// File: intdefs.h
// first lines: defines of Riccardo Paoletti

#ifndef __INTDEFS__
#define __INTDEFS__
#define int2 	short
#define uint2 	unsigned int2
#define int4 	long
#define uint4 	unsigned int4
#define uchar	unsigned char
#endif

//
//===================================================================
// Date: 8 July 2003
// Rev.: 31 Jul 2007
// Auth: Markus Fras, Dept. Electronics, MPI f. Physics, Munich
//
// CVS info:
// File             : $Source: /CVS/Magic2DAQ/M2_mir/intdefs.h,v $
// Last modified by : $Author: mazin $                                    
//               at : $Date: 2009/03/30 13:46:15 $                           
// Revision number  : $Revision: 1.4 $
//
// This header file defines some basic types.
//===================================================================

#ifndef __TCUC_TYPES_H
#define __TCUC_TYPES_H

/* standard integer types */
typedef signed char         CHAR;
typedef signed short        SHORT;
typedef signed int          INT;
typedef signed long         LONG;
typedef unsigned char       U_CHAR;
typedef unsigned short      U_SHORT;
typedef unsigned int        U_INT;
typedef unsigned long       U_LONG;
#ifndef __cplusplus
typedef int					bool;
#endif

/* hardware specific integer types */
typedef LONG                INT32;
typedef U_LONG              U32;
typedef CHAR                INT8;
typedef U_CHAR              BYTE;

/* boolean values */
//#ifndef __cplusplus
//#undef true
//#undef false
//#define true  (bool) 0
//#define false (bool) 1
//#endif

#endif
