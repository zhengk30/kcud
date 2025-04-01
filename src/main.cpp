#include "parser/include/database/database.hpp"
#include "parser/include/decoder/decoder.hpp"

int main() {
    // Database db("../test/tpch_lineitem_comment.db");

    Database db("/Users/kaiwenzheng/comment.db");

    db.LoadExistingDatabase();
    return 0;
}