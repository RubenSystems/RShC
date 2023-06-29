//
//  config.h
//  RShC
//
//  Created by Ruben Ticehurst-James on 24/06/2023.
//

#ifndef config_h
#define config_h

#include <stdint.h>

#define DBG 0

#if DBG
#include <stdio.h>
#define dbg(fmt, ...) printf(#fmt __VA_OPT__(, ) __VA_ARGS__)
#else
#define dbg(fmt, ...)
#endif

#define KEY_TYPE	 uint64_t
#define VALUE_TYPE	 uint64_t
#define LOCAL_KEY_TYPE	 uint16_t

#define ENUM_TYPE	 uint8_t

#define TABLE_WIDTH	 4
#define TABLE_SIZE	 (2 << TABLE_WIDTH)
#define TABLE_WIDTH_MASK (1 << (TABLE_WIDTH + 1)) - 1

/*
 2 bits (0 = none, 01 = bridge, 10 = full)
 8 cells per table
 
 2 * 8 = 16 bits for the lookup table
 */
#define LOOKUP_TABLE_TYPE      uint64_t
#define LOOKUP_TABLE_TYPE_SIZE sizeof(LOOKUP_TABLE_TYPE) * 8
#define LOOKUP_TABLE_SET_MASK  (2 << LOOKUP_TABLE_TYPE_SIZE) - 1
#define LOOKUP_TABLE_MASK      0b11

#endif /* config_h */
