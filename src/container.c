#include "include/container.h"
#include <stdbool.h>
#include <stdlib.h>

#define LAYER_TYPE uint16_t

void container_init(struct container * con) {
	table_init(&con->root);
}

static struct table * __alloc_table() {
	struct table * ptr = malloc(sizeof(struct table));
	table_init(ptr);
	return ptr;
}

static size_t __layer_key(KEY_TYPE key, LAYER_TYPE layer) {
	return TABLE_MASK & (key >> (layer * TABLE_BYTE_SIZE));
}

struct container_get_res container_get(struct container * cont, KEY_TYPE key) {
	struct table *		 tab = &cont->root;
	struct container_get_res ret;
	int			 layer = 0;
	while (true) {
		size_t		     lk	   = __layer_key(key, layer++);
		struct table_get_res t_res = table_get(tab, lk);

		switch (t_res.type) {
		case TABLE_GET_NOTFOUND:
			ret.type = CONTAINER_GET_NOTFOUND;
			return ret;
		case TABLE_GET_DATA:
			if (t_res.key == key) {
				ret.value = t_res.value.data;
				ret.type  = CONTAINER_GET_FOUND;
			} else {
				ret.type = CONTAINER_GET_NOTFOUND;
			}
			return ret;
		case TABLE_GET_POINTER:
			tab = t_res.value.pointer;
		}
	}
}

static void __create_until_nonequal(
	struct table * tab, uint32_t layer, struct table_get_res * current_res,
	uint64_t key, uint64_t value
) {
	layer++;

	LAYER_KEY_TYPE lke;
	LAYER_KEY_TYPE lkn;
	struct table * n_tab;

	while ((lke = __layer_key(current_res->key, layer)) !=
	       (lkn = __layer_key(key, layer))) {
		n_tab = __alloc_table();
		table_unsafe_insert_pointer(tab, lke, n_tab);
		tab = n_tab;
	};

	table_unsafe_insert_data(tab, lke, current_res->key, current_res->value.data);
	table_unsafe_insert_data(tab, lkn, key, value);

	// KEY_TYPE       replacement_key	 = current_ret->data.format.key;
	// VALUE_TYPE     replacement_value = current_ret->data.format.value.data;
	// struct table * new_table	 = NULL;
	// size_t	       lk2;
	// // layer key for the inserted node == layer key for the current node.
	// do {
	// 	layer += 1;
	// 	// new_table = init_table();
	// 	new_table = allocate(mltable->allocator);
	// 	set_node_pointer(current_ret, 0, new_table);
	// 	current_ret = get_table_node(
	// 		new_table, (lk1 = __layer_key(key, layer))
	// 	);

	// } while (lk1 == (lk2 = __layer_key(replacement_key, layer)));
	// set_node_data(current_ret, key, value);

	// struct node * fl_current = get_table_node(new_table, lk2);
	// set_node_data(fl_current, replacement_key, replacement_value);
}

void container_upsert(struct container * cont, KEY_TYPE key, VALUE_TYPE value) {
	struct table * tab   = &cont->root;
	int	       layer = 0;
	while (true) {
		size_t		     lk	   = __layer_key(key, layer++);
		struct table_get_res t_res = table_get(tab, lk);

		switch (t_res.type) {
		case TABLE_GET_NOTFOUND:
			table_unsafe_insert_data(tab, lk, key, value);
			return;
		case TABLE_GET_DATA:
			if (t_res.key == key) {
				tab->values[lk].data = value;
			} else {
				__create_until_nonequal(
					tab, layer, &t_res, key, value
				);
			}
			return;
		case TABLE_GET_POINTER:
			tab = t_res.value.pointer;
		}
	}
}