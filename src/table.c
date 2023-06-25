//
//  table.c
//  RShC
//
//  Created by Ruben Ticehurst-James on 24/06/2023.
//

#include "table.h"


static ENUM_TYPE __lookup_table_get(LOOKUP_TABLE_TYPE table, LOOKUP_TABLE_TYPE index) {
	return (table >> (index * 2)) & LOOKUP_TABLE_MASK;
}

static void __force_insert(
						   struct table * tab,
						   KEY_TYPE index,
						   KEY_TYPE key,
						   VALUE_TYPE value
						   ) {
	tab->keys[index] = key;
	tab->values[index] = value;
}

enum table_insert_res table_insert(
								   struct table * tab,
								   KEY_TYPE layer_key,
								   KEY_TYPE key,
								   VALUE_TYPE value
								) {
	
	uint16_t mirror_key = TABLE_SIZE - layer_key;
	
	ENUM_TYPE k1t = __lookup_table_get(tab->lookup, layer_key);
	ENUM_TYPE k2t = __lookup_table_get(tab->lookup, mirror_key);

	KEY_TYPE k_storage;
	
	if (k1t == NODE_EMPTY || k1t == NODE_BRIDGE) {
		__force_insert(tab, layer_key, key, value);
		return INSERT_COMPLETED;
	} else if ((k_storage = tab->keys[layer_key]) == key) {
		tab->values[layer_key] = value;
		return UPSERT_COMPLETED;
	}
	
	if (k2t == NODE_EMPTY || k2t == NODE_BRIDGE) {
		__force_insert(tab, mirror_key, key, value);
		return INSERT_COMPLETED;
	} else if ((k_storage = tab->keys[k2t]) == key) {
		tab->values[k2t] = value;
		return UPSERT_COMPLETED;
	}
	
	
	return (layer_key > (TABLE_SIZE >> 1)) ? INSERT_LEFT : INSERT_RIGHT; 
	
}
