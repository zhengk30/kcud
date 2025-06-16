#include "interface.h"
#include "include/database/database.hpp"
#include <fstream>
#include <re2/re2.h>
#include <re2/stringpiece.h>

Database* db = nullptr;
Table* table = nullptr;
size_t str_count = 0;

extern "C" void db_init() {
    auto start = chrono::high_resolution_clock::now();
    db = new Database(DBFILE);
    db->LoadExistingDatabase();
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    std::cout << "\033[31mdb loaded (" << duration.count() << " sec)\033[0m\n";
}

extern "C" void scan_table() {
    // assert(db);
    // table = db->GetTable(0);
    // auto start = chrono::high_resolution_clock::now();
    // db->ScanTable(table);
    // auto end = chrono::high_resolution_clock::now();
    // chrono::duration<double> duration = end - start;
    // std::cout << "\033[31mtable scanned into memory (" << duration.count() << " sec)\033[0m\n";
    auto start = chrono::high_resolution_clock::now();
    char buf[BATCH_SIZE];
    uint8_t lengths[STRLEN_ARR_SIZE];
    while(load_strings_in_batch(BATCH_SIZE, buf, lengths));
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    std::cout << "\033[31mtable scanned into memory (" << duration.count() << " sec)\033[0m\n";
}

void regex_filter_worker(const char* pattern, char* partial_strings, vector<uint8_t>& partial_lengths, size_t* count) {
    re2::RE2 pattern_obj(pattern);
    size_t n_strs = partial_lengths.size();
    // printf("n_strs=%u\n", n_strs);
    size_t offset = 0;
    size_t total = 0;
    for (size_t i = 0; i < n_strs; i++) {
        // printf("[%u] offset=%u, partial_lengths[%u]=%u, partial_strings+offset=%s\n", i, offset, i, partial_lengths[i], partial_strings+offset);
        re2::StringPiece sp(partial_strings + offset, partial_lengths[i]);
        total += re2::RE2::FullMatch(sp, pattern_obj);
        offset += partial_lengths[i];
        // printf("[%u] sp created, offset=%u\n", i, offset);
    }
    *count = total;
    assert(offset == strlen(partial_strings));
}

extern "C" size_t count_matches(const char* pattern) {
    vector<thread> threads;
    vector<size_t> counts(NTHREADS);
    auto start = std::chrono::high_resolution_clock::now();
    for (unsigned i = 0; i < NTHREADS; i++) {
       threads.emplace_back(
           regex_filter_worker,
           pattern,
           table->GetPartialStrings(i),
           ref(table->GetPartialLengths(i)),
           &counts[i]
       );
    }
    for (auto& t : threads) {
       t.join();
    }
    auto end = std::chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    std::cout << "\033[31mfilter duration=" << duration.count() << " sec\033[0m\n";
    
    start = std::chrono::high_resolution_clock::now();
    size_t total = accumulate(counts.begin(), counts.end(), 0);
    end = std::chrono::high_resolution_clock::now();
    duration = end - start;
    std::cout << "\033[31maggregate duration=" << duration.count() << " sec\033[0m\n";

    return total;    
}

extern "C" void dump_strings_to(const char* dest) {
    ofstream dump(dest);
    
    for (unsigned i = 0; i < NTHREADS; i++) {
        size_t offset = 0;
        vector<uint8_t>& partial_lengths = table->GetPartialLengths(i);
        char* partial_strings = table->GetPartialStrings(i);
        size_t n_strs = partial_lengths.size();
        for (size_t j = 0; j < n_strs; j++) {
            string str(partial_strings + offset, partial_lengths[j]);
            offset += partial_lengths[j];
            dump << str.c_str() << '\n';
        }
    }
    dump.close();
}

extern "C" size_t load_strings_in_batch(size_t nbytes, char* buf, uint8_t* lengths) {
    assert(db);
    table = db->GetTable(0);
    assert(table);
    // printf("[load_strings_in_batch] size=%u\n", nbytes);
    memset(buf, 0, nbytes);
    lengths = new uint8_t[STRLEN_ARR_SIZE];
	size_t n_strs = table->LoadData(DBFILE, nbytes, buf, lengths);
    // printf("n_strs=%lu\n", n_strs);
    // str_count += n_strs;
    return n_strs;
}
