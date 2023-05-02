project(
  'httpServer',
  'cpp',
  version : '0.1',
  default_options : [
    'warning_level=3',
    'cpp_std=c++17'
  ]
)

incdir = include_directories('include')

sources = [
  'src/main.cpp'
]

deps = [
  dependency('threads', required: true),
  dependency('dl', required: false),
]

cmake = import('cmake')

# Configure the CMake project
drogon_opts = cmake.subproject_options()
drogon_opts.add_cmake_defines({
  'BUILD_DOC': false,
  'BUILD_PROGRAMS': false,
  'BUILD_EXAMPLES': false,
  'BUILD_CTL': false,
})
drogon_opts.set_override_option('cpp_std', 'c++17')
drogon_opts.set_override_option('c_std', 'c11')
drogon_opts.set_override_option('cpp_std', 'c++17', target: 'drogon')
drogon_opts.set_override_option('cpp_std', 'c++14', target: 'trantor')

drogon_proj = cmake.subproject('drogon', options: drogon_opts)

# Fetch the dependency object
message('drogon CMake targets:\n - ' + '\n - '.join(drogon_proj.target_list()))
deps += drogon_proj.dependency('drogon')

exe = executable(
  'httpserver',
  sources,
  install : true,
  dependencies: deps,
  include_directories: incdir,
)
