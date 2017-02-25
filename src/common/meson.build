common_src = [
    'array.c',
    'hash.c',
    'mem_op.c',
    'print_util.c',
    'sort.c',
    'time_util.c',
    'matrix.c',
]

common_lib = static_library(
    'common',
    common_src,
    include_directories : inc,
    dependencies : [m_dep, thread_dep],
    install : true)
