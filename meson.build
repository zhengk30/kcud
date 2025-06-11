project('parser', 'cpp',
  default_options: ['warning_level=1', 'optimization=0']
)

inc_dir = include_directories('/usr/local/include')
re2_dep = meson.get_compiler('cpp').find_library('re2', dirs: '/usr/local/lib', required: true)
absl_dep = meson.get_compiler('cpp').find_library('absl', dirs: '/usr/local/lib', required: true)

regduck_src = ['src/include/decoder/decoder.cpp',
            'src/block/metadata_block.cpp',
            'src/block/storage_block.cpp',
            'src/statistics/base_statistics.cpp',
            'src/statistics/distinct_statistics.cpp',
            'src/row_group/row_group.cpp',
            'src/schema/schema.cpp',
            'src/table/table.cpp',
            'src/type_info/type_info.cpp',
            'src/header/main_header.cpp',
            'src/header/database_header.cpp',
            'src/database/database.cpp',
            'src/interface.cpp',
            'src/main.cpp']

regduck_lib = static_library('run',
  cpp_args: ['-std=c++17'],
  sources: regduck_src,
  include_directories: inc_dir,
  dependencies: [re2_dep, absl_dep])

thread_dep = dependency('threads')

executable('run',
  cpp_args: ['-std=c++17'],
  sources: regduck_src,
  include_directories: inc_dir,
  dependencies: [re2_dep, absl_dep, thread_dep])

regduck_inc = include_directories('src')

regduck_dep = declare_dependency(
  include_directories: [regduck_inc, inc_dir],
  dependencies: [re2_dep, absl_dep, thread_dep],
  link_with: regduck_lib
)
