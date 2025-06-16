#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define DBFILE "/home/ubuntu/dbfiles/sf50.db"
#define BATCH_SIZE ((1 << 14) + 1)
#define STRLEN_ARR_SIZE (1 << 10)

#ifdef __cplusplus
extern "C" {
#endif

// initializes metadata of a database and its entries
void db_init();

// scans rows of an entire table into memory (assuming 1st table)
void scan_table();

// persists table rows into a file
void dump_strings_to(const char* dest);

// populates *buf* with complete strings that are stored contiguously
// and returns the number of complete strings copied into *buf*
size_t load_strings_in_batch(size_t nbytes, char* buf, uint8_t* lengths);

size_t count_matches(const char* pattern);

#ifdef __cplusplus
}
#endif

#endif  // INTERFACE_H
