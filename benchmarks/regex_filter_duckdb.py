import duckdb
import json


QUERY = "SELECT COUNT(l_comment) FROM comment WHERE l_comment LIKE '%regular%'"
DATABASE_FILE = '/Users/kaiwenzheng/Documents/RegDuck/db-files/tpch_lineitem_comment_sf1.db'
JSON_FILE = '/Users/kaiwenzheng/Documents/RegDuck/out.json'


def run(conn, query):
    conn.execute("PRAGMA enable_profiling='json'")
    conn.execute("PRAGMA profiling_mode='detailed'")
    conn.execute(f"PRAGMA profiling_output='{JSON_FILE}'")
    conn.execute(query)


def parse_json(path):
    with open(path, 'r') as json_file:
        data = json.load(json_file)
    json_file.close()
    latency = data["latency"]
    cpu_time = data["cpu_time"]
    filter_cpu_time = data["children"][0]["operator_timing"]
    scan_cpu_time = data["children"][0]["children"][0]["operator_timing"]
    print(f'execution latency (e2e): {latency} sec')
    print(f'execution latency (cpu): {cpu_time} sec')
    print(f'filter cpu time: {filter_cpu_time} sec')
    print(f'scan cpu time: {scan_cpu_time} sec')


if __name__ == '__main__':
    conn = duckdb.connect(database=DATABASE_FILE)
    run(conn, QUERY)
    parse_json(JSON_FILE)