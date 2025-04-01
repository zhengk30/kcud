project('duckreg', 'c', 'cpp',
  default_options: ['warning_level=2', 'optimization=0']
)

executable('main',
  cpp_args: ['-std=c++17'],
  sources: ['main.cpp',
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

