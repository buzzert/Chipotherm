project('chipotherm', 'cpp',
    default_options: ['cpp_std=c++14'],
)

libbubbles_proj = subproject('bubbles')
libbubbles_dep  = libbubbles_proj.get_variable('bubbles_dep')

compiler = meson.get_compiler('cpp')

chipotherm_deps = [
    libbubbles_dep,
    dependency('pangocairo'),
    dependency('sigc++-2.0'),
    dependency('threads'),
    dependency('hidapi-hidraw'),
    compiler.find_library('tempered'),
]

chipotherm_sources = [
    'src/button_actor.cpp',
    'src/clock_actor.cpp',
    'src/graph_actor.cpp',
    'src/io_control.cpp',
    'src/main.cpp',
    'src/monitor.cpp',
    'src/palette.cpp',
    'src/primary_scene.cpp',
    'src/qube_actor.cpp',
    'src/rounded_title_actor.cpp',
    'src/runloop.cpp',
    'src/utilities.cpp',
]

chipotherm_bin = executable('chipotherm',
    chipotherm_sources,
    dependencies: chipotherm_deps,
    install: true
)
