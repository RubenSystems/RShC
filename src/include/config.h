//
//  config.h
//  RShC
//
//  Created by Ruben Ticehurst-James on 24/06/2023.
//

#ifndef config_h
#define config_h

#include <stdint.h>

#define KEY_TYPE uint64_t
#define VALUE_TYPE uint64_t

#define ENUM_TYPE uint8_t

#define TABLE_SIZE 8

/*
 2 bits (0 = none, 1 = bridge, 2 = full)
 16 cells per table
 
 2 * 16 = 32 bits for the lookup table
 */
#define LOOKUP_TABLE_TYPE uint16_t
#define LOOKUP_TABLE_MASK 0b11

#endif /* config_h */
