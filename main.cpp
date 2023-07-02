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
	struct container c;
	container_init(&c);
//
//
//
	struct table t;
	table_init(&t);
	table_insert(&t, 2, 200, 20);
	table_insert(&t, 2, 100, 10);
	
	printf("%llu %llu\n", table_get(&t, 2, 100).response_type, table_get(&t, 2, 200).response_type);
//	return 0; 
	auto startTime = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < sizeof(random_keys) / sizeof(random_keys[0]); i ++) {
		container_insert(&c, random_keys[i], i);
	}
	auto endTime = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::EXE_TIMER_UNIT>(endTime - startTime).count();
	std::cout << "Execution time: " << duration << " EXE_TIMER_UNITs" << std::endl;

	
	startTime = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < sizeof(random_keys) / sizeof(random_keys[0]); i ++) {
		container_get(&c, random_keys[i]);
//		assert(m.value == i);

	}

	endTime = std::chrono::high_resolution_clock::now();

	duration = std::chrono::duration_cast<std::chrono::EXE_TIMER_UNIT>(endTime - startTime).count();
	std::cout << "Execution time: " << duration << " EXE_TIMER_UNITs" << std::endl;

	
	printf("SIZEOF_TABLE: %i\n", (int)sizeof(struct table));
	return 0;
}
