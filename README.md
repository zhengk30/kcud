# kcud

Kcud is a simplified library that extracts string columns from tables in a database stored as a DuckDB database file. **(UNDER CONSTRUCTION FOR MORE FEATURES!)**

## Dependencies
* DuckDB's C++ API ([install it based on your OS](https://duckdb.org/docs/installation/?version=stable))
* DuckDB's CLI (refer to [this page](https://duckdb.org/docs/installation/?version=stable&environment=cli&platform=linux&download_method=direct&architecture=arm64))
  * If not working, then build from [DuckDB's source](https://github.com/duckdb/duckdb)
    * `cd` into `duckdb`.
    * Run `make`.
    * A `build` folder is created.
    * To run DuckDB as an in-memory database, run `build/release/duckdb` and you are done. To run DuckDB with a native database file (i.e., `.db` file), run    `build/release/duckdb <filename>.db`. Refer to the *How to Generate `.db` Files* section for details related to generating database files for testing.

## How to Generate `.db` Files
* Run DuckDB as an in-memory database.
* Ensure that the TPC-H extension is loaded in DuckDB. If not, run `INSTALL tpch;` and then `LOAD tpch;`.
* Run `CALL dbgen(sf=<sf>);`, where `<sf>` is the [scale factor](https://duckdb.org/docs/stable/extensions/tpch.html) the TPC-H workload is generated.
* Run `COPY (SELECT l_comment FROM lineitem) TO 'comment_sf<sf>.csv' (header, delimiter ',');` to dump the `lineitem.l_comment` column to a CSV file.
* Run `.exit` to quit DuckDB and then re-run DuckDB, specifying the database file (no need to create one in advance, as DuckDB will do it if the file is not found).
* Run `PRAGMA force_compression='uncompressed';` to disable compression on the string columns. This is for the purpose of benchmarking.
* Load the rows in the dumped CSV file with `CREATE TABLE comment AS SELECT * FROM 'comment_sf<sf>.csv';`. Now you have a table named `comment` that contains all strings from `lineitem.l_comment` column.
