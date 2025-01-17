

/**
 * @file common_types.h
 * @author meng_yu (imyumeng@qq.com)
 * @brief
 * @version 0.1
 * @date 2020-06-06
 *
 * @copyright imyumeng@qq.com Copyright (c) 2020
 *
 */
#ifndef _COMMON_TYPES_H_
#define _COMMON_TYPES_H_

typedef void (*fptr_t)(void);

typedef signed char char_t;
typedef signed short int short_t;
typedef signed int int_t;
typedef signed long int long_t;

typedef unsigned char byte_t;
typedef unsigned short int word_t;
typedef unsigned int dword_t;
typedef unsigned long int qword_t;

typedef unsigned char u8;
typedef unsigned short int u16;
typedef unsigned int u32;
typedef unsigned long int u64;

typedef volatile byte_t vu8;
typedef volatile word_t vu16;
typedef volatile dword_t vu32;
typedef volatile qword_t vu64;

#endif
