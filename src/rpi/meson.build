rpi_src = [
    'gpio.c',
    'pwm.c',
    'rpi.c',
    'systimer.c',]

rpi_lib = static_library(
    'rpi',
    rpi_src,
    include_directories : inc,
    dependencies : [m_dep, thread_dep],
    link_with: [common_lib],
    install : true)
