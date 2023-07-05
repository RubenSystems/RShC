//
//  container.c
//  RShC
//
//  Created by Ruben Ticehurst-James on 28/06/2023.
//

#include "include/container.h"

#include <stdbool.h>
#include <stdlib.h>

#define LAYER_TYPE uint16_t

void container_init(struct container * c) {
	table_init(&c->root);
}

static size_t __layer_key(KEY_TYPE key, LAYER_TYPE layer) {
	return TABLE_WIDTH_MASK & (key >> (layer * TABLE_WIDTH));
}

struct container_get_result
container_get(struct container * cont, KEY_TYPE key) {
	static max_lay3rs = 0;
	struct table *		    tab	      = &cont->root;
	uint16_t		    layer_key = 0;
	LAYER_TYPE		    layer     = 0;
	struct table_get_result	    res;
	struct container_get_result ret;

	do {
		layer_key = __layer_key(key, layer++);
		res	  = table_get(tab, layer_key, key);
		switch (res.response_type) {
			case GET_NOTFOUND:
				ret.response = CONTAINER_GET_NOTFOUND;
				return ret;
			case GET_LEFT:
				tab = tab->left;
				break;
			case GET_RIGHT:
				tab = tab->right;
				break;
			case GET_COMPLETED:
				ret.value    = res.value;
				ret.response = CONTAINER_GET_COMPLETED;
				return ret;
			default:
				printf("[CONATINER] - table corrupted");
				exit(1);
		}
		if (layer > max_lay3rs) {
			printf("%i\n", layer);
			max_lay3rs = layer;
		}
	} while (tab);
	
	ret.response = CONTAINER_GET_NOTFOUND;
	return ret;
}

void container_insert(struct container * cont, KEY_TYPE key, VALUE_TYPE val) {
	struct table * tab = &cont->root;
	uint16_t       layer_key;
	LAYER_TYPE     layer = 0;

	while (true) {
		layer_key = __layer_key(key, layer++);

		enum table_insert_res res =
			table_insert(tab, layer_key, key, val);
		switch (res) {
		case INSERT_LEFT: {
			if (!tab->left)
				tab->left = talloc();
			tab = tab->left;
		} break;
		case INSERT_RIGHT: {
			if (!tab->right)
				tab->right = talloc();
			tab = tab->right;
		} break;
		case INSERT_COMPLETED:
		case UPSERT_COMPLETED:
			return;
		default:
			printf("[CONTAINER_INSERT] - table corrupted");
			exit(0);
			break;
		}
	}
}
