#include "regduck_api.hpp"

#define DBFILE "/Users/kaiwenzheng/Documents/RegDuck/test/tpch_lineitem_comment_sf1.db"

Table* table = nullptr;

void table_init() {
    Database db(DBFILE);
    db.LoadExistingDatabase();
    table = db.GetTable(0);  // the database file only has one table with one column
    db.ScanTable(table);
}

size_t load_batched_strings(char* batch_buf, size_t batch_size) {
    if (table == nullptr) {
        table_init();
    }
    printf("thread_cursor=%d, per_thread_cursor=%llu\n", table->GetCurrentThreadCursor(), table->GetCurrentPerThreadCursor());
    size_t current_size = 0;
    size_t actual_size = 0;
    char* str = nullptr;
    while ((str = table->GetNextString())) {
        current_size += strlen(str);
        if (current_size > batch_size) {
            actual_size = current_size - strlen(str);
            break;
        }
        table->AdvanceCursors();
    }
    printf("actual_size=%llu, batch_size=%llu\n\n", actual_size, batch_size);
    return current_size;
}