project('X', 'c')

inc = include_directories(['src/'])

# Declare external dependencies here
cc = meson.get_compiler('c')
m_dep = cc.find_library('m', required : false)
thread_dep = dependency('threads')

subdir('src/common')
subdir('src/rpi')
subdir('src/examples/print_test')
subdir('src/examples/matrix_test')
