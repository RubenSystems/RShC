//
//  table.c
//  RShC
//
//  Created by Ruben Ticehurst-James on 24/06/2023.
//

#include "table.h"
#include <string.h>
#include <stdlib.h>

void table_init(struct table * tab) {
	tab->left   = NULL;
	tab->right  = NULL;
	tab->lookup = 0;
	memset(tab->values, 0, sizeof(tab->values));
}

struct table * talloc(void) {
	struct table * ret = malloc(sizeof(struct table));
	table_init(ret);
	return ret;
}

static ENUM_TYPE
__lookup_table_get(LOOKUP_TABLE_TYPE table, LOOKUP_TABLE_TYPE index) {
	// << 1 == * 2
	return (table >> (index << 1)) & LOOKUP_TABLE_MASK;
}

static LOCAL_KEY_TYPE __mirror_key(LOCAL_KEY_TYPE local_key) {
	return TABLE_SIZE - local_key;
}

static void
__set_lookup(struct table * tab, LOOKUP_TABLE_TYPE index, ENUM_TYPE new_value) {
	uint64_t new_mask =
		~(LOOKUP_TABLE_MASK << (index << 1)); // Calculate the new mask
	tab->lookup = (tab->lookup & new_mask) | (new_value << (index << 1));
}

static void __force_insert(
	struct table * tab, KEY_TYPE index, KEY_TYPE key, VALUE_TYPE value
) {
	tab->keys[index]   = key;
	tab->values[index] = value;
	__set_lookup(tab, index, NODE_DATA);
}

struct table_get_result table_get(struct table * tab, LOCAL_KEY_TYPE layer_key, KEY_TYPE key) {
	
	ENUM_TYPE      k1t	  = __lookup_table_get(tab->lookup, layer_key);
	struct table_get_result res;
		//use k1t
	
	if (k1t == NODE_DATA && tab->keys[layer_key] == key) {
		res.response_type = GET_COMPLETED;
		res.value = tab->values[layer_key];
		return res;
	} else if (k1t == NODE_EMPTY || k1t == NODE_BRIDGE) {
		res.response_type = GET_NOTFOUND;
		return res;
	}
	
	uint16_t       mirror_key = __mirror_key(layer_key);
	ENUM_TYPE      k2t	  = __lookup_table_get(tab->lookup, mirror_key);
	
	if (k2t == NODE_DATA && tab->keys[mirror_key] == key) {
		res.response_type = GET_COMPLETED;
		res.value = tab->values[mirror_key];
		return res;
	} else if (k2t == NODE_EMPTY || k2t == NODE_BRIDGE) {
		res.response_type = GET_NOTFOUND;
		return res;
	}

	res.response_type = (layer_key > (TABLE_SIZE >> 1)) ? GET_LEFT : GET_RIGHT;
	return res;
}

enum table_insert_res table_insert(
	struct table * tab, LOCAL_KEY_TYPE layer_key, KEY_TYPE key,
	VALUE_TYPE value
) {
	
	ENUM_TYPE      k1t	  = __lookup_table_get(tab->lookup, layer_key);

	if (k1t == NODE_EMPTY || k1t == NODE_BRIDGE) {
		__force_insert(tab, layer_key, key, value);
		return INSERT_COMPLETED;
	} else if (tab->keys[layer_key] == key) {
		tab->values[layer_key] = value;
		return UPSERT_COMPLETED;
	}
	
	uint16_t       mirror_key = __mirror_key(layer_key);
	ENUM_TYPE      k2t	  = __lookup_table_get(tab->lookup, mirror_key);

	if (k2t == NODE_EMPTY || k2t == NODE_BRIDGE) {
		__force_insert(tab, mirror_key, key, value);
		return INSERT_COMPLETED;
	} else if (tab->keys[k2t] == key) {
		tab->values[k2t] = value;
		return UPSERT_COMPLETED;
	}

	return (layer_key > (TABLE_SIZE >> 1)) ? INSERT_LEFT : INSERT_RIGHT;
}
