project(
  'httpServer',
  'cpp',
  version : '0.1',
  default_options : [
    'warning_level=3',
    'cpp_std=c++20'
  ]
)

incdir = include_directories('include')

sources = [
  'src/main.cpp',
  'src/helloController.cpp',
  'src/dataRecordController.cpp',
]

deps = [
  dependency('threads', required: true),
  dependency('dl', required: false),
]

cmake = import('cmake')

date_opts = cmake.subproject_options()
date_opts.add_cmake_defines({
    'USE_SYSTEM_TZ_DB': true,
    'BUILD_TZ_LIB': true,
    'BUILD_SHARED_LIBS': get_option('default_library') == 'shared',
})
date_opts.set_override_option('cpp_std', 'c++20')
date_proj = cmake.subproject('date', options: date_opts)

drogon_opts = cmake.subproject_options()
drogon_opts.add_cmake_defines({
   'BUILD_DOC': false,
   'BUILD_PROGRAMS': false,
   'BUILD_EXAMPLES': false,
   'BUILD_CTL': false,
   'BUILD_SHARED_LIBS': get_option('default_library') == 'shared',
   'USE_SUBMODULE': true,
 })
drogon_opts.set_override_option('cpp_std', 'c++17')
drogon_opts.set_override_option('c_std', 'c11')
drogon_opts.set_override_option('cpp_std', 'c++17', target: 'drogon')
drogon_opts.set_override_option('cpp_std', 'c++14', target: 'trantor')

drogon_proj = cmake.subproject('drogon', options: drogon_opts)

# Fetch the dependency object
message('drogon CMake targets:\n - ' + '\n - '.join(drogon_proj.target_list()))
message('date CMake targets:\n - ' + '\n - '.join(date_proj.target_list()))
deps += date_proj.dependency('date')
deps += date_proj.dependency('date-tz')
deps += drogon_proj.dependency('drogon')
deps += drogon_proj.dependency('trantor')

exe = executable(
  'httpserver',
  sources,
  install : true,
  dependencies: deps,
  include_directories: incdir,
)
