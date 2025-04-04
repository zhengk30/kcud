import duckdb
import time
import pandas


db_files = {
    'sf_1': '../test/tpch_lineitem_comment.db',
    'sf_2': '../test/tpch_lineitem_comment_2.db',
    'sf_3': '../test/tpch_lineitem_comment_3.db',
    'sf_4': '../test/tpch_lineitem_comment_4.db'
}


def run(conn, pattern):
    query = f"select l_comment as lc from comment where lc like '{pattern}'"
    rc = conn.execute('explain analyze ' + query).fetchall()
    rc = '\n'.join(row[1] for row in rc)
    print(rc)


if __name__ == '__main__':
    for file in db_files:
        conn = duckdb.connect(db_files[file])
        run(conn, '%regular%')
        conn.close()
