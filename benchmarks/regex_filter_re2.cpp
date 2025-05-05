#include "../src/parser/common.hpp"
#include "../src/parser/include/database/database.hpp"
#include "../src/regex/regex.hpp"

#define PATTERN ".*regular.*"
#define NFILES 7

void re2_regex_match_count(char** partial_strings, uint64_t partial_count, const char* pattern_ptr, uint64_t* count, vector<char *>& result) {
    re2::RE2 pattern(pattern_ptr);
    uint64_t total = 0;
    for (uint64_t i = 0; i < partial_count; i++) {
        if (re2::RE2::FullMatch(partial_strings[i], pattern)) {
            total++;
            result.push_back(partial_strings[i]);
        }
    }
    *count = total;
}

int main() {
    const char* db_files[] = {
        "/Users/kaiwenzheng/Documents/RegDuck/db-files/tpch_lineitem_comment_sf1.db",
        "/Users/kaiwenzheng/Documents/RegDuck/db-files/tpch_lineitem_comment_sf2.db",
        "/Users/kaiwenzheng/Documents/RegDuck/db-files/tpch_lineitem_comment_sf3.db",
        "/Users/kaiwenzheng/Documents/RegDuck/db-files/tpch_lineitem_comment_sf4.db",
        "/Users/kaiwenzheng/Documents/RegDuck/db-files/tpch_lineitem_comment_sf10.db",
        "/Users/kaiwenzheng/Documents/RegDuck/db-files/tpch_lineitem_comment_sf20.db",
        "/Users/kaiwenzheng/Documents/RegDuck/db-files/tpch_lineitem_comment_sf30.db"
    };
    
    // TODO: uncomment these lines if you want to dump matching strings
    // const char* regex_matches_dumps[] = {
    //     "/Users/kaiwenzheng/Documents/RegDuck/test/tpch_lineitem_comment_sf1_matches.txt",
    //     "/Users/kaiwenzheng/Documents/RegDuck/test/tpch_lineitem_comment_sf2_matches.txt",
    //     "/Users/kaiwenzheng/Documents/RegDuck/test/tpch_lineitem_comment_sf3_matches.txt",
    //     "/Users/kaiwenzheng/Documents/RegDuck/test/tpch_lineitem_comment_sf4_matches.txt",
    //     "/Users/kaiwenzheng/Documents/RegDuck/test/tpch_lineitem_comment_sf10_matches.txt",
    //     "/Users/kaiwenzheng/Documents/RegDuck/test/tpch_lineitem_comment_sf20_matches.txt",
    //     "/Users/kaiwenzheng/Documents/RegDuck/test/tpch_lineitem_comment_sf30_matches.txt"
    // };

    for (auto i = 0; i < NFILES; i++) {
        const char* db_file = db_files[i];
        if (!filesystem::exists(db_file)) {
            // TODO: add missing db files
            continue;
        }

        Database db(db_file);
        db.LoadExistingDatabase();
        Table* table = db.GetTable(0);

        auto start = chrono::high_resolution_clock::now();
        db.ScanTable(table);
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> elapsed = end - start;
        std::cout << "[SCAN] elapsed: " << elapsed.count() << " sec\n";

        vector<thread> re2_threads;
        vector<vector<char *>> partial_results(NTHREADS);
        start = chrono::high_resolution_clock::now();
        vector<uint64_t> counts(NTHREADS * 8);
        uint64_t match_count = 0;
        for (unsigned i = 0; i < NTHREADS; i++) {
            re2_threads.emplace_back(
                re2_regex_match_count, table->GetStringsPerThread(i), table->GetCountPerThread(i), PATTERN, &counts[i * 8], ref(partial_results[i])
            );
        }
        for (auto& t : re2_threads) {
            t.join();
        }
        match_count = accumulate(counts.begin(), counts.end(), 0);
        end = chrono::high_resolution_clock::now();
        elapsed = end - start;
        
        std::cout << db_file << '\n';
        std::cout << "[FILTER] elapsed: " << elapsed.count() << " sec\n";
        std::cout << "match count: " << match_count << "\n\n";

        // TODO: uncomment these lines if you want to dump matching strings
        // const char* dump_file = regex_matches_dumps[i];
        // ofstream dump;
        // assert((dump = ofstream(dump_file)));
        // for (int i = 0; i < NTHREADS; i++) {
        //     for (const char* str : partial_results[i]) {
        //         dump << str << '\n';
        //     }
        // }
        // dump.close();
        table->Clear();
    }
    
    return 0;
}