#include "parser/include/database/database.hpp"
#include "regex/regex.hpp"
#define DBFILE "../test/tpch_lineitem_comment.db"

// void re2_regex_match_count(Database& db, int thread_id, uint64_t start, uint64_t end,
//                         const re2::RE2& pattern, vector<uint64_t>& partial_counts) {
//     uint64_t total = 0;
//     // auto start_time = chrono::high_resolution_clock::now();
//     for (auto i = start; i < end; i++) {
//         char row[64] = {0};
//         db[i].copy(row, db[i].size());
//         // total += RegexMatcher::Matches(row, pattern);
//         partial_counts[thread_id] += RegexMatcher::Matches(row, pattern);
//     }
//     // partial_counts[thread_id * 8] = total;
//     // auto end_time = chrono::high_resolution_clock::now();
//     // chrono::duration<double> elapsed = end_time - start_time;
// }

// void duckdb_regex_match_count(Database& db, int thread_id, uint64_t start, uint64_t end,
//                         const char* pattern, vector<uint64_t>& partial_counts) {
//     uint64_t total = 0;
//     // auto start_time = chrono::high_resolution_clock::now();
//     for (uint64_t i = start; i < end; i++) {
//         char row[64] = {0};
//         db[i].copy(row, db[i].size());
//         total += RegexMatcher::Matches(row, strlen(row), pattern, strlen(pattern));
//     }
//     partial_counts[thread_id] = total;
//     // auto end_time = chrono::high_resolution_clock::now();
//     // chrono::duration<double> elapsed = end_time - start_time;
//     // cout << "[duckdb_regex_match_count] elapsed: " << elapsed.count() << " sec\n";
// }

int main() {
    Database db(DBFILE);
    db.LoadExistingDatabase();
    Table* table = db.GetTable(0);
    db.ScanTable(table);
    re2::RE2 pattern(".*regular.*");
    uint64_t total = 0;
    for (auto i = 0; i < table->GetRowCount(); i++) {
        if (re2::RE2::FullMatch(table->GetString(i), pattern)) {
            total++;
        }
    }
    std::cout << "total=" << total << '\n';

    // TODO: regex benchmarking
    // const char* plain_pattern = "%regular%";
    // re2::RE2 re2_pattern(".*regular.*");
    // uint64_t match_count = 0;

    // auto nthreads = NTHREADS;
    // auto row_count = db.GetRowCount();
    // auto batch_size = (row_count + nthreads - 1) / nthreads;

    // //
    // // benchmarking duckdb's regex operator
    // //
    // //
    // vector<thread> duck_threads;
    // vector<uint64_t> duck_results(nthreads);
    // auto start_time = chrono::high_resolution_clock::now();
    // for (unsigned i = 0; i < nthreads; i++) {
    //     uint64_t start = i * batch_size;
    //     uint64_t end = (i == nthreads-1) ? row_count : start + batch_size;
    //     duck_threads.emplace_back(
    //         duckdb_regex_match_count, ref(db), i, start, end, plain_pattern, ref(duck_results)
    //     );
    // }
    // for (auto& t : duck_threads) {
    //     t.join();
    // }
    // match_count = accumulate(duck_results.begin(), duck_results.end(), 0);
    // auto end_time = chrono::high_resolution_clock::now();
    // std::chrono::duration<double> elapsed = end_time - start_time;
    // std::cout << "[DuckDB regex parallel] # matches = " << match_count <<  ", elapsed time: " << elapsed.count() << " sec\n";

    // //
    // // benchmarking re2's FullMatch
    // //
    // //
    // vector<thread> re2_threads;
    // vector<uint64_t> re2_results(nthreads);
    // start_time = chrono::high_resolution_clock::now();
    // for (unsigned i = 0; i < nthreads; i++) {
    //     uint64_t start = i * batch_size;
    //     uint64_t end = (i == nthreads-1) ? row_count : start + batch_size;
    //     re2_threads.emplace_back(
    //         re2_regex_match_count, ref(db), i, start, end, ref(re2_pattern), ref(re2_results)
    //     );
    // }
    // for (auto& t : re2_threads) {
    //     t.join();
    // }
    // match_count = accumulate(re2_results.begin(), re2_results.end(), 0);
    // end_time = chrono::high_resolution_clock::now();
    // elapsed = end_time - start_time;
    // std::cout << "[RE2 regex parallel] # matches = " << match_count << ", elapsed time: " << elapsed.count() << " sec\n";

    // //
    // // benchmarking re2's serial matching
    // //
    // //
    // vector<uint64_t> re2_serial_results(1);
    // start_time = chrono::high_resolution_clock::now();
    // match_count = 0;
    // for (auto& row : db) {
    //     match_count += re2::RE2::FullMatch(row, re2_pattern);
    // }
    // end_time = chrono::high_resolution_clock::now();
    // elapsed = end_time - start_time;
    // std::cout << "[RE2 regex serial] # matches = " << match_count << ", elapsed time: " << elapsed.count() << " sec\n";

    return 0;
}