//
//  container.c
//  RShC
//
//  Created by Ruben Ticehurst-James on 28/06/2023.
//

#include "container.h"

#include <stdbool.h>
#include <stdlib.h>

#define LAYER_TYPE uint16_t

void container_init(struct container * c) {
	table_init(&c->root);
}

static size_t __layer_key(KEY_TYPE key, LAYER_TYPE layer) {
	return TABLE_WIDTH_MASK & (key >> (layer * TABLE_WIDTH));
}

VALUE_TYPE container_get(struct container * cont, KEY_TYPE key) {
	struct table * tab = &cont->root;
	uint16_t layer_key = 0;
	LAYER_TYPE layer = 0;
	
	while (true) {
		layer_key = __layer_key(key, layer++);
		
	}
}

void container_insert(struct container * cont, KEY_TYPE key, VALUE_TYPE val) {
	struct table * tab = &cont->root;
	uint16_t	       layer_key;
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
