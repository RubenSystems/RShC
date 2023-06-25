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
		struct table * left;
		struct table * right;
		LOOKUP_TABLE_TYPE lookup;
		KEY_TYPE keys [TABLE_SIZE];
		VALUE_TYPE values [TABLE_SIZE];
		
	};
	
	enum table_insert_res {
		INSERT_LEFT,
		INSERT_RIGHT,
		INSERT_COMPLETED,
		UPSERT_COMPLETED
	};
	
	enum table_insert_res table_insert(
									   struct table * tab,
									   KEY_TYPE layer_key,
									   KEY_TYPE key,
									   VALUE_TYPE value
									);
	
#ifdef __cplusplus
}
#endif


#endif /* table_h */
