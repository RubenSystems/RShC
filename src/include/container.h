//
//  container.h
//  RShC
//
//  Created by Ruben Ticehurst-James on 05/07/2023.
//

#ifndef container_h
#define container_h

#include "table.h"

#ifdef __cplusplus
extern "C" {
#endif

struct container {
	struct table root;
};

enum container_get_types {
	CONTAINER_GET_FOUND,
	CONTAINER_GET_NOTFOUND,
};

struct container_get_res {
	enum container_get_types type;
	KEY_TYPE		 key;
	VALUE_TYPE		 value;
};

void container_init(struct container *);

struct container_get_res container_get(struct container *, KEY_TYPE);

#ifdef __cplusplus
}
#endif

#endif /* container_h */