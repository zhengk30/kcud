#pragma once
#include "src/parser/include/database/database.hpp"

extern Table* table;
/*
    Returns multiple strings as contiguous bytes. The total size of returned strings
    is limited by *batch_size*, and all returned strings are complete, so it's possible
    that the actual number of bytes returned is less than *batch_size*
*/
size_t load_batched_strings(char* batch_buf, size_t batch_size);