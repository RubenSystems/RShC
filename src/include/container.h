//
//  container.h
//  RShC
//
//  Created by Ruben Ticehurst-James on 28/06/2023.
//

#ifndef container_h
#define container_h

#include <stdio.h>
#include "table.h"
#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif

struct container {
	struct table root;
};

void container_init(struct container *);

enum table_get_responses {
	CONTAINER_GET_NOTFOUND	= 1,
	CONTAINER_GET_COMPLETED = 0
};

struct container_get_result {
	VALUE_TYPE		 value;
	enum table_get_responses response;
};

void container_insert(struct container *, KEY_TYPE, VALUE_TYPE);

struct container_get_result
container_get(struct container * cont, KEY_TYPE key);

#ifdef __cplusplus
}
#endif

#endif /* container_h */
