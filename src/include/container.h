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

void container_insert(struct container *, KEY_TYPE, VALUE_TYPE);

#ifdef __cplusplus
}
#endif

#endif /* container_h */
