meson setup build -Dbench_re2=true
meson compile -C build
build/run
rm -rf build
