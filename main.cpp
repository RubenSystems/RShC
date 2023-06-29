//
//  main.c
//  RShC
//
//  Created by Ruben Ticehurst-James on 24/06/2023.
//


#include <chrono>
#include <iostream>
#include "src/include/container.h"
#include "testing_dataset.h"

#define EXE_TIMER_UNIT milliseconds

int main(int argc, const char * argv[]) {
	// insert code here...
//	struct container c;
//	container_init(&c);
//
//
//
	struct table t;
	table_init(&t);
	table_insert(&t, 10, 200, 200);
	table_insert(&t, 10, 100, 100);
	auto startTime = std::chrono::high_resolution_clock::now();
//	for (int i = 0; i < sizeof(random_keys) / sizeof(random_keys[0]); i ++)
//		container_insert(&c, random_keys[i], i);
////	dbg("%i\n", table_insert(&t, 5, 100, 200));
////	dbg("%i\n", table_insert(&t, 5, 200, 300));
////	dbg("%i\n", table_insert(&t, 5, 300, 400));
///
///
	struct table_get_result res = table_get(&t, 10, 100);
	auto endTime = std::chrono::high_resolution_clock::now();
////	printf("%llu\n", random_keys[1000]);
	

	
	
	
	printf("%i %i\n", res.value, res.response_type);
	
	auto duration = std::chrono::duration_cast<std::chrono::EXE_TIMER_UNIT>(endTime - startTime).count();
	std::cout << "Execution time: " << duration << " EXE_TIMER_UNITs" << std::endl;

	
	printf("Hello, World! %i\n", (int)sizeof(struct table));
	return 0;
}
