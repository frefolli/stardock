project('stardock', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3', 'cpp_std=c++20'])

includes = include_directories('include')

stardock = library(
  'stardock', [
    'src/stardock/index.cc',
    'src/stardock/point.cc',
    'src/stardock/diff.cc',
    'src/stardock/transfer.cc'
  ], include_directories: includes
)

executable(
  'main.exe', [
    'src/main.cc'
  ], link_with: [stardock],
  include_directories: includes)
