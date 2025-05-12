#include "../third-party/duckdb/duckdb.hpp"
#include "../third-party/json/single_include/nlohmann/json.hpp"
#include <fstream>
#include <iostream>

// NOTE: use full path
#define INPUT_PATH "/home/ubuntu/RegDuck/db-files/comment_sf1.db"
#define QUERY "select count(l_comment) from comment where l_comment like '%regular%'"

using namespace duckdb;
using namespace std;
using json = nlohmann::json;

void run(Connection& conn, const char* query) {
    conn.Query("PRAGMA enable_profiling='json'");
    conn.Query("PRAGMA profiling_mode='detailed'");
    conn.Query("PRAGMA profiling_output='out.json'");
    conn.Query(query);
}

void json_stat(const char* filepath) {
    ifstream file(filepath);
    json data = json::parse(file);
    auto latency = data["latency"];
    auto cpu_time = data["cpu_time"];
    auto filter_cpu_time = data["children"][0]["cpu_time"];
    auto scan_cpu_time = data["children"][0]["children"][0]["cpu_time"];
    std::cout << "execution latency (e2e): " << latency << '\n';
    std::cout << "execution latency (cpu): " << cpu_time << '\n';
    std::cout << "filter cpu time: " << filter_cpu_time << '\n';
    std::cout << "scan cpu time: " << scan_cpu_time << '\n';
}


int main(int argc, char** argv) {
    DuckDB db(INPUT_PATH);
    Connection conn(db);
    run(conn, QUERY);
    json_stat("out.json");
    remove("out.json");
    return 0;
}
