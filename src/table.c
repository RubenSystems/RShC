//
//  table.c
//  RShC
//
//  Created by Ruben Ticehurst-James on 05/07/2023.
//

#include <string.h>
#include <stdlib.h>

#include "include/config.h"
#include "include/table.h"

void table_init(struct table * tab) {
	memset(tab->lookup, 0, sizeof(tab->lookup));
}

static uint8_t __lookup_table_get(uint8_t * lookup, LAYER_KEY_TYPE index) {
	LAYER_KEY_TYPE table_index = index >> 2;
	LAYER_KEY_TYPE cell_index  = (index & (4 - 1)) << 1;
	return (lookup[table_index] >> cell_index) & LOOKUP_MASK;
}

static void
__lookup_table_set(uint8_t * lookup, LAYER_KEY_TYPE index, uint8_t value) {
	LAYER_KEY_TYPE table_index = index >> 2;
	LAYER_KEY_TYPE cell_index  = (index & (4 - 1)) << 1;

	lookup[table_index] &= ~(LOOKUP_MASK << cell_index);
	lookup[table_index] |= value << cell_index;
}

void table_unsafe_insert_data(
	struct table * tab, LAYER_KEY_TYPE lk, KEY_TYPE key, VALUE_TYPE val
) {
	tab->keys[lk]	     = key;
	tab->values[lk].data = val;
	__lookup_table_set(tab->lookup, lk, NODE_DATA);
}

void table_unsafe_insert_pointer(
	struct table * tab, LAYER_KEY_TYPE lk, struct table * ptr
) {
	tab->values[lk].pointer = ptr;
	__lookup_table_set(tab->lookup, lk, NODE_POINTER);
}

struct table_get_res table_get(struct table * tab, LAYER_KEY_TYPE layer_key) {
	uint8_t lookup = __lookup_table_get(tab->lookup, layer_key);
	struct table_get_res res;
	switch (lookup) {
	case NODE_EMPTY:
		res.type = TABLE_GET_NOTFOUND;
		return res;
	case NODE_DATA:
		res.type       = TABLE_GET_DATA;
		res.value.data = tab->values[layer_key].data;
		res.key	       = tab->keys[layer_key];
		return res;
	case NODE_POINTER:
		res.type = TABLE_GET_POINTER;
		return res;
	default:
		printf("ERROR: TABLE_CORRUPTION\n");
		exit(0);
	}
}
