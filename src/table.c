//
//  table.c
//  RShC
//
//  Created by Ruben Ticehurst-James on 24/06/2023.
//

#include "include/table.h"
#include <string.h>
#include <stdlib.h>

void table_init(struct table * tab) {
	tab->left   = NULL;
	tab->right  = NULL;
//	tab->lookup = 0;
	memset(&tab->lookup, 0, sizeof(tab->lookup));
	memset(tab->values, 0, sizeof(tab->values));
}

struct table * talloc(void) {
	struct table * ret = malloc(sizeof(struct table));
	table_init(ret);
	return ret;
}

static LOCAL_KEY_TYPE __mirror_key(LOCAL_KEY_TYPE local_key) {
	return MIRROR_KEY_SUBTRACTOR - local_key;
}

static ENUM_TYPE
__lookup_table_get(LOOKUP_TABLE_TYPE table, LOOKUP_TABLE_TYPE index) {
	//n << 1 == n * 2
	return (table >> (index << 1)) & LOOKUP_TABLE_MASK;
}

static LOOKUP_TABLE_TYPE __lookup_table_update(
	LOOKUP_TABLE_TYPE lk_table, LOOKUP_TABLE_TYPE index,
	LOOKUP_TABLE_TYPE new_value
) {
	LOOKUP_TABLE_TYPE idx_shift = index << 1;
	LOOKUP_TABLE_TYPE new_mask  = ~(LOOKUP_TABLE_MASK << idx_shift);
	return (lk_table & new_mask) | (new_value << idx_shift);
}

static void __force_insert(
	struct table * tab, KEY_TYPE index, KEY_TYPE key, VALUE_TYPE value
) {
	tab->keys[index]   = key;
	tab->values[index] = value;
	tab->lookup = __lookup_table_update(tab->lookup, index, NODE_DATA);
}

struct table_get_result
table_get(struct table * tab, LOCAL_KEY_TYPE layer_key, KEY_TYPE key) {
	ENUM_TYPE k1t = __lookup_table_get(tab->lookup, layer_key);
	struct table_get_result res;
	if (k1t == NODE_DATA && tab->keys[layer_key] == key) {
		res.response_type = GET_COMPLETED;
		res.value	  = tab->values[layer_key];
		return res;
	} else if (k1t == NODE_EMPTY || k1t == NODE_BRIDGE) {
		res.response_type = GET_NOTFOUND;
		return res;
	}

	uint16_t  mirror_key = __mirror_key(layer_key);
	ENUM_TYPE k2t	     = __lookup_table_get(tab->lookup, mirror_key);

	if (k2t == NODE_DATA && tab->keys[mirror_key] == key) {
		res.response_type = GET_COMPLETED;
		res.value	  = tab->values[mirror_key];
		return res;
	} else if (k2t == NODE_EMPTY || k2t == NODE_BRIDGE) {
		res.response_type = GET_NOTFOUND;
		return res;
	}

	res.response_type = (layer_key > (TABLE_SIZE >> 1)) ? GET_LEFT :
							      GET_RIGHT;
	return res;
}

enum table_insert_res table_insert(
	struct table * tab, LOCAL_KEY_TYPE layer_key, KEY_TYPE key,
	VALUE_TYPE value
) {
	ENUM_TYPE k1t = __lookup_table_get(tab->lookup, layer_key);

	if (k1t == NODE_EMPTY || k1t == NODE_BRIDGE) {
		__force_insert(tab, layer_key, key, value);
		return INSERT_COMPLETED;
	} else if (tab->keys[layer_key] == key) {
		tab->values[layer_key] = value;
		return UPSERT_COMPLETED;
	}

	uint16_t  mirror_key = __mirror_key(layer_key);
	ENUM_TYPE k2t	     = __lookup_table_get(tab->lookup, mirror_key);

	if (k2t == NODE_EMPTY || k2t == NODE_BRIDGE) {
		__force_insert(tab, mirror_key, key, value);
		return INSERT_COMPLETED;
	} else if (tab->keys[k2t] == key) {
		tab->values[k2t] = value;
		return UPSERT_COMPLETED;
	}

	return (layer_key > (TABLE_SIZE >> 1)) ? INSERT_LEFT : INSERT_RIGHT;
}
