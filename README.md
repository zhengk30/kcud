# RegDuck

## Dependencies
* DuckDB's C++ API for Linux
  * `cd` into `RegDuck/third-party/duckdb`. There's a zip file containing the necessary files for building DuckDB's C++ API.
  * Unzip that file, remove the original zip file, and you are done.
* DuckDB's CLI (refer to [this page](https://duckdb.org/docs/installation/?version=stable&environment=cli&platform=linux&download_method=direct&architecture=arm64))
  * If not working, then build from [DuckDB's source](https://github.com/duckdb/duckdb)
    * `cd` into `duckdb`.
    * Run `make`.
    * A `build` folder is created.
    * To run DuckDB as an in-memory database, run `build/release/duckdb` and you are done. To run DuckDB with a native database file (i.e., `.db` file), run    `build/release/duckdb <filename>.db`. Refer to the *How to Generate `.db` Files* section for details related to generating database files for testing.
    
* Nlohmann json parser
* Google's RE2 library (TODO: integration not working as of now, investigating...)

## Setup
The following files are of particular interest:
- `benchmarks/regex_filter_duckdb/cpp` benchmarks the performance of running a RegEx query in DuckDB, including the end-to-end query execution latency, the wall-clock time of query execution, the wall-clock time of scanning the table, and the wall-clock time of filtering the table rows.
- `benchmarks/regex_filter_re2.cpp` accomplishes the similar task except that the end-to-end query execution latency are not measured.
- `src/main.cpp` serves as a sample usage program to demonstrate how `regduck_api.hpp` is used for integration.
To choose which one to run, change `run.sh` by passing the right option to `meson`:
  * Set `bench_duckdb` to `true` if `benchmarks/regex_filter_duckdb/cpp` is desired.
  * Set `bench_re2` to `true` if `benchmarks/regex_filter_re2.cpp` is desired.
By default, `src/main.cpp` is run if none of the options above are specified.

## Integration
To integrate with the parser, include `src/regduck_api.hpp` and use `src/load_strings_in_batch`. Check `src/main.cpp` for sample usage. Modify the `DBFILE` macro as needed.

## How to Generate `.db` Files
* Run DuckDB as an in-memory database.
* Ensure that the TPC-H extension is loaded in DuckDB. If not, run `INSTALL tpch;` and then `LOAD tpch;`.
* Run `CALL dbgen(sf=<sf>);`, where `<sf>` is the [scale factor](https://duckdb.org/docs/stable/extensions/tpch.html) the TPC-H workload is generated.
* Run `COPY (SELECT l_comment FROM lineitem) TO 'comment_sf<sf>.csv' (header, delimiter ',');` to dump the `lineitem.l_comment` column to a CSV file.
* Run `.exit` to quit DuckDB and re-run DuckDB but with database file specified (there's no need to create one in advance, as DuckDB will do the creation if the file is not found).
* Run `PRAGMA force_compression='uncompressed';` to disable compression on the string columns. This is for the purpose of benchmarking.
* Load the rows in the dumped CSV file with `CREATE TABLE comment AS SELECT * FROM 'comment_sf<sf>.csv';`. Now you have a table named `comment` that contains all strings from `lineitem.l_comment` column.
