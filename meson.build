project('RegDuck', 'c', 'cpp',
  default_options: ['warning_level=2', 'optimization=0']
)

regex_dep = dependency('re2', required: false)
if not regex_dep.found()
  regex_sp = subproject('src/regex/re2_regex')
  regex_dep = regex_sp.get_variable('regex_dep')
endif

which_main = 'src/main.cpp'
is_bench = get_option('bench')
if is_bench
  which_main = 'benchmarks/regex_filter_re2.cpp'
endif

executable('run',
  cpp_args: ['-std=c++17'],
  dependencies: [regex_dep],
  sources: ['src/regex/duckdb_regex/regex.cpp',
            'src/parser/include/decoder/decoder.cpp',
            'src/parser/block/metadata_block.cpp',
            'src/parser/block/storage_block.cpp',
            'src/parser/statistics/base_statistics.cpp',
            'src/parser/statistics/distinct_statistics.cpp',
            'src/parser/row_group/row_group.cpp',
            'src/parser/schema/schema.cpp',
            'src/parser/table/table.cpp',
            'src/parser/type_info/type_info.cpp',
            'src/parser/header/main_header.cpp',
            'src/parser/header/database_header.cpp',
            'src/parser/database/database.cpp',
            'src/regduck_api.cpp', which_main])
