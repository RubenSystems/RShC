//
//  table.h
//  RShC
//
//  Created by Ruben Ticehurst-James on 24/06/2023.
//

#ifndef table_h
#define table_h

#include <stdio.h>
#include "config.h"
#include "node.h"

#ifdef __cplusplus
extern "C" {
#endif

struct table {
	struct table *	  left;
	struct table *	  right;
	LOOKUP_TABLE_TYPE lookup;
	KEY_TYPE	  keys[TABLE_SIZE];
	VALUE_TYPE	  values[TABLE_SIZE];
};

enum table_insert_res {
	INSERT_LEFT	 = 3,
	INSERT_RIGHT	 = 2,
	INSERT_COMPLETED = 0,
	UPSERT_COMPLETED = 1
};
	
	enum table_get_res {
		GET_NOTFOUND = 3,
		GET_LEFT = 2,
		GET_RIGHT = 1,
		GET_COMPLETED = 0
	};
	
	struct table_get_result {
		enum table_get_res response_type;
		VALUE_TYPE value;
	};

void table_init(struct table *);

struct table * talloc(void);

enum table_insert_res table_insert(
	struct table *, LOCAL_KEY_TYPE, KEY_TYPE,
	VALUE_TYPE value
);
	
	struct table_get_result table_get(struct table *, LOCAL_KEY_TYPE, KEY_TYPE);

#ifdef __cplusplus
}
#endif

#endif /* table_h */
