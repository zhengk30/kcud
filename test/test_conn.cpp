#include "../third-party/duckdb/duckdb.hpp"
#include <iostream>

using namespace duckdb;
using namespace std;

int main() {
    DuckDB db(nullptr);
    Connection conn(db);
    cout << "Connection established\n";
    return 0;
}