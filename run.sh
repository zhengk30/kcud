meson setup build -Dbench_duckdb=true
meson compile -C build
build/run
rm -rf build
