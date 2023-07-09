//
//  config.h
//  RShC
//
//  Created by Ruben Ticehurst-James on 05/07/2023.
//

#ifndef config_h
#define config_h

#include <stdint.h>

#define KEY_TYPE	 uint64_t
#define VALUE_TYPE	 uint64_t

#define PACKED_ENUM_TYPE uint8_t

#define TABLE_BYTE_SIZE	 4
#define TABLE_SIZE	 2 << TABLE_BYTE_SIZE
#define TABLE_MASK	 (1 << (TABLE_BYTE_SIZE + 1)) - 1

#define LOOKUP_SIZE	 2 * TABLE_BYTE_SIZE
#define LOOKUP_MASK	 0b11

#endif /* config_h */
