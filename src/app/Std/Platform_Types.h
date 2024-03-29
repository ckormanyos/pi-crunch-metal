// *****************************************************************************
// Filename    : Platform_Types.h
// 
// Author      : Chalandi Amine
//
// Owner       : Chalandi Amine
// 
// Date        : 27.12.2017
// 
// Description : Platform types definition header file
// 
// *****************************************************************************

#ifndef PLATFORM_TYPES_2017_12_27_H
#define PLATFORM_TYPES_2017_12_27_H

typedef unsigned char uint8;
typedef signed char sint8;
typedef unsigned short uint16;
typedef signed short sint16;
typedef unsigned long uint32;
typedef signed long sint32;
typedef unsigned long long uint64;
typedef signed long long sint64;

typedef void (*pFunc)(void);

typedef enum
{
  FALSE = 0,
  TRUE
}boolean;


typedef unsigned char u_int8_t;
typedef unsigned char u8;
typedef unsigned char u_char;
typedef signed char int8_t;
typedef unsigned short u_int16_t;
typedef unsigned short u16;
typedef signed short int16_t;
typedef unsigned long u_int32_t;
typedef unsigned long u_int;
typedef signed long int32_t;
typedef unsigned long long u_long;

#endif /* PLATFORM_TYPES_2017_12_27_H */
