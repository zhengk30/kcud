#include "regduck_api.h"
#include "include/database/database.hpp"
#include <fstream>
#include <re2/re2.h>

Table* table = nullptr;

void _table_init() {
    Database db(DBFILE);
    db.LoadExistingDatabase();
    table = db.GetTable(0);  // the database file only has one table with one column

    auto start = chrono::high_resolution_clock::now();
    db.ScanTable(table);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    std::cout << "\033[31mtable scan duration=" << duration.count() << " sec\033[0m\n";
}

void re2_worker(char** partial_strings, uint64_t partial_count, const char* pattern_ptr, uint64_t* count) {
    re2::RE2 pattern(pattern_ptr);
    uint64_t total = 0;
    for (uint64_t i = 0; i < partial_count; i++) {
        total += re2::RE2::FullMatch(partial_strings[i], pattern);
    }
    *count = total;
}

extern "C" size_t load_strings_in_batch(char* batch_buf, size_t batch_size) {
    if (table == nullptr) {
        _table_init();
    }
    size_t actual_size = 0;
    size_t i = 0;
    char* str = nullptr;
    while ((str = table->GetNextString())) {
        size_t len = strlen(str);
        if (actual_size + len + 1 > batch_size) {
            break;
        }
        memcpy(batch_buf + i, str, len);
        batch_buf[i + len] = '\n';
        i += (len + 1);
        actual_size += (len + 1);
        table->AdvanceCursors();
    }
    return actual_size;
}

extern "C" void dump_strings_to(const char* dest) {
    _table_init();
    ofstream dump(dest);
    for (unsigned i = 0; i < NTHREADS; i++) {
        uint64_t num_strings = table->GetCountPerThread(i);
        char** strings = table->GetStringsPerThread(i);
        for (uint64_t j = 0; j < num_strings; j++) {
            dump << strings[j] << '\n';
        }
    }
    dump.close();
}

extern "C" size_t count_matches(const char* pattern) {
    _table_init();

    //re2::RE2 re2_pattern(pattern);
    //char* str = nullptr;
    //size_t total = 0;
    //auto start = std::chrono::high_resolution_clock::now();
    //while ((str = table->GetNextString())) {
    //    total += re2::RE2::FullMatch(str, re2_pattern);
    //    table->AdvanceCursors();
    //}
    //auto end = std::chrono::high_resolution_clock::now();
    //chrono::duration<double> duration = end - start;
    //std::cout << "\033[31mfilter duration=" << duration.count() << " sec\033[0m\n";
    //return total;
    vector<thread> threads;
    vector<uint64_t> counts(NTHREADS);
    auto start = std::chrono::high_resolution_clock::now();
    for (unsigned i = 0; i < NTHREADS; i++) {
        threads.emplace_back(
            re2_worker,
            table->GetStringsPerThread(i),
            table->GetCountPerThread(i),
            pattern,
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

