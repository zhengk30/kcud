#include "regduck_api.hpp"

#define DBFILE "/Users/kaiwenzheng/Documents/RegDuck/test/tpch_lineitem_comment_sf1.db"

int main() {
    char buf[MAX_BATCH_SIZE + 1];
    while (load_strings_in_batch(buf, MAX_BATCH_SIZE)) {
        buf[MAX_BATCH_SIZE] = 0;
        printf("%s", buf);
        memset(buf, 0, MAX_BATCH_SIZE + 1);
    }
    return 0;
}

// #include "parser/include/database/database.hpp"
// #include "regex/regex.hpp"
// #define DBFILE "/Users/kaiwenzheng/Documents/RegDuck/test/tpch_lineitem_comment_sf10.db"

// //
// // NOTE: don't reuse re2::RE2 object across threads, create a new one for each thread!
// //
// //
// void re2_regex_match_count(char** partial_strings, uint64_t partial_count, const char* pattern_ptr, uint64_t* count) {
//     re2::RE2 pattern(pattern_ptr);
//     uint64_t total = 0;
//     for (uint64_t i = 0; i < partial_count; i++) {
//         total += re2::RE2::FullMatch(partial_strings[i], pattern);
//     }
//     *count = total;
// }

// // void duckdb_regex_match_count(Database& db, int thread_id, uint64_t start, uint64_t end,
// //                         const char* pattern, vector<uint64_t>& partial_counts) {
// //     uint64_t total = 0;
// //     // auto start_time = chrono::high_resolution_clock::now();
// //     for (uint64_t i = start; i < end; i++) {
// //         char row[64] = {0};
// //         db[i].copy(row, db[i].size());
// //         total += RegexMatcher::Matches(row, strlen(row), pattern, strlen(pattern));
// //     }
// //     partial_counts[thread_id] = total;
// //     // auto end_time = chrono::high_resolution_clock::now();
// //     // chrono::duration<double> elapsed = end_time - start_time;
// //     // cout << "[duckdb_regex_match_count] elapsed: " << elapsed.count() << " sec\n";
// // }

// int main() {
//     Database db(DBFILE);
//     db.LoadExistingDatabase();
//     Table* table = db.GetTable(0);
//     auto start = chrono::high_resolution_clock::now();
//     db.ScanTable(table);
//     auto end = chrono::high_resolution_clock::now();
//     chrono::duration<double> elapsed = end - start;
//     std::cout << "[ScanTable] elapsed: " << elapsed.count() << " sec\n";

//     const char* re2_pattern = ".*regular.*";
//     // //
//     // // benchmarking duckdb's regex operator
//     // //
//     // //
//     // vector<thread> duck_threads;
//     // vector<uint64_t> duck_results(nthreads);
//     // auto start_time = chrono::high_resolution_clock::now();
//     // for (unsigned i = 0; i < nthreads; i++) {
//     //     uint64_t start = i * batch_size;
//     //     uint64_t end = (i == nthreads-1) ? row_count : start + batch_size;
//     //     duck_threads.emplace_back(
//     //         duckdb_regex_match_count, ref(db), i, start, end, plain_pattern, ref(duck_results)
//     //     );
//     // }
//     // for (auto& t : duck_threads) {
//     //     t.join();
//     // }
//     // match_count = accumulate(duck_results.begin(), duck_results.end(), 0);
//     // auto end_time = chrono::high_resolution_clock::now();
//     // std::chrono::duration<double> elapsed = end_time - start_time;
//     // std::cout << "[DuckDB regex parallel] # matches = " << match_count <<  ", elapsed time: " << elapsed.count() << " sec\n";

//     // //
//     // // benchmarking re2's FullMatch
//     // //
//     // //
//     vector<thread> re2_threads;
//     start = chrono::high_resolution_clock::now();
//     vector<uint64_t> counts(NTHREADS * 8);
//     uint64_t match_count = 0;
//     for (unsigned i = 0; i < NTHREADS; i++) {   
//         re2_threads.emplace_back(
//             re2_regex_match_count, table->GetStringsPerThread(i), table->GetCountPerThread(i), re2_pattern, &counts[i * 8]
//         );
//     }
//     for (auto& t : re2_threads) {
//         t.join();
//     }
//     match_count = accumulate(counts.begin(), counts.end(), 0);
//     end = chrono::high_resolution_clock::now();
//     elapsed = end - start;
//     std::cout << "[RE2 regex parallel]" << " elapsed time: " << elapsed.count() << " sec\n";
//     std::cout << "match_count = " << match_count << '\n';

//     table->Clear();
//     return 0;
// }