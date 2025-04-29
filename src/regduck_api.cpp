#include "regduck_api.hpp"

#define DBFILE "/Users/kaiwenzheng/Documents/RegDuck/test/tpch_lineitem_comment_sf1.db"
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

Table* table = nullptr;

void _table_init() {
    Database db(DBFILE);
    db.LoadExistingDatabase();
    table = db.GetTable(0);  // the database file only has one table with one column
    db.ScanTable(table);
}

size_t load_strings_in_batch(char* batch_buf, size_t batch_size) {
    if (table == nullptr) {
        _table_init();
    }
    // batch_size = MIN(batch_size, MAX_BATCH_SIZE);
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