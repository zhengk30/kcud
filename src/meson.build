project('RegDuck', 'c', 'cpp',
  default_options: ['warning_level=2', 'optimization=0']
)

regex_dep = dependency('re2', required: false)
if not regex_dep.found()
  regex_sp = subproject('regex/re2_regex')
  regex_dep = regex_sp.get_variable('regex_dep')
endif


executable('main',
  cpp_args: ['-std=c++17'],
  dependencies: [regex_dep],
  sources: ['main.cpp',
            'regex/duckdb_regex/regex.cpp',
            'parser/include/decoder/decoder.cpp',
            'parser/block/metadata_block.cpp',
            'parser/block/storage_block.cpp',
            'parser/statistics/base_statistics.cpp',
            'parser/statistics/distinct_statistics.cpp',
            'parser/row_group/row_group.cpp',
            'parser/schema/schema.cpp',
            'parser/table/table.cpp',
            'parser/type_info/type_info.cpp',
            'parser/header/main_header.cpp',
            'parser/header/database_header.cpp',
            'parser/database/database.cpp'])

