# RegDuck

## Integration
To integrate with the parser, include `src/regduck_api.hpp` and use `src/load_strings_in_batch`. Check `src/main.cpp` for sample usage. Modify the `DBFILE` macro as needed.

## Dependencies
* DuckDB's Python API (`pip install duckdb --upgrade`)
* Nlohmann json parser
* Google's RE2 library

## Setup
The following files are of particular interest:
- `benchmarks/regex_filter_duckdb/cpp` benchmarks the performance of running a RegEx query in DuckDB, including the end-to-end query execution latency, the wall-clock time of query execution, the wall-clock time of scanning the table, and the wall-clock time of filtering the table rows.
- `benchmarks/regex_filter_re2.cpp` accomplishes the similar task except that the end-to-end query execution latency are not measured.
- `src/main.cpp` serves as a sample usage program to demonstrate how `regduck_api.hpp` is used for integration.
To choose which one to run, change `run.sh` by passing the right option to `meson`:
  * Set `bench_duckdb` to `true` if `benchmarks/regex_filter_duckdb/cpp` is desired.
  * Set `bench_re2` to `true` if `benchmarks/regex_filter_re2.cpp` is desired.
By default, `src/main.cpp` is run if none of the options above are specified.
