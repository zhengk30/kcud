#include "regduck_api.hpp"

#define DBFILE "/Users/kaiwenzheng/Documents/RegDuck/db-files/tpch_lineitem_comment_sf1.db"

int main() {
    char buf[MAX_BATCH_SIZE + 1];
    while (load_strings_in_batch(buf, MAX_BATCH_SIZE)) {
        buf[MAX_BATCH_SIZE] = 0;
        // printf("%s", buf);
        memset(buf, 0, MAX_BATCH_SIZE + 1);
    }
    return 0;
}