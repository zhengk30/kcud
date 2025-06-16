#include "interface.h"
#include <chrono>
#include <iostream>

int main() {
    db_init();
    // scan_table();
    //dump_strings_to("dump.txt");
    
	char buf[BATCH_SIZE];
    uint8_t lengths[STRLEN_ARR_SIZE];

    auto start = std::chrono::high_resolution_clock::now();
    while(load_strings_in_batch(BATCH_SIZE, buf, lengths));
    // size_t n_strs = load_strings_in_batch(BATCH_SIZE, buf, lengths);
    
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "batched load duration: " << duration.count() << " sec\n";
    return 0;
}
