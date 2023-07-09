//
//  table.h
//  RShC
//
//  Created by Ruben Ticehurst-James on 05/07/2023.
//

#ifndef table_h
#define table_h

#include <stdio.h>
#include "config.h"

#define NODE_EMPTY     0
#define NODE_DATA      1
#define NODE_POINTER   2

#define LAYER_KEY_TYPE uint16_t

#ifdef __cplusplus
extern "C" {
#endif

struct table {
	uint8_t lookup[LOOKUP_SIZE];

	KEY_TYPE keys[TABLE_SIZE];
	union {
		VALUE_TYPE     data;
		struct table * pointer;
	} values[TABLE_SIZE];
};

enum table_get_types { TABLE_GET_NOTFOUND, TABLE_GET_DATA, TABLE_GET_POINTER };

struct table_get_res {
	KEY_TYPE key;

	union {
		VALUE_TYPE     data;
		struct table * pointer;
	} value;

	enum table_get_types type;
};

void table_unsafe_insert_data(
	struct table *, LAYER_KEY_TYPE, KEY_TYPE, VALUE_TYPE
);

void table_unsafe_insert_pointer(struct table *, LAYER_KEY_TYPE, struct table *);

void table_init(struct table *);

struct table_get_res table_get(struct table * tab, LAYER_KEY_TYPE layer_key);

#ifdef __cplusplus
}
#endif

#endif /* table_h */
