#ifndef REGDUCK_API_H
#define REGDUCK_API_H

#include <stdlib.h>

#define DBFILE "/proj/fardatalab-PG0/dbfiles/sf100.db"
#define MAX_BATCH_SIZE (1 << 14)
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

#ifdef __cplusplus
extern "C" {
#endif

size_t load_strings_in_batch(char* batch_buf, size_t batch_size);
void dump_strings_to(const char* dest);
size_t count_matches(const char* pattern);  // for comparing re2's direct filter vs dpkernel

#ifdef __cplusplus
}
#endif

#endif  // REGDUCK_API_H
