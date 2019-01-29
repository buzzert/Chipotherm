project('chipotherm', 'cpp',
    default_options: ['cpp_std=c++14'],
)

libbubbles_proj = subproject('bubbles')
libbubbles_dep  = libbubbles_proj.get_variable('bubbles_dep')

chipotherm_deps = [
    libbubbles_dep,
    dependency('pangocairo'),
    dependency('sigc++-2.0'),
]

chipotherm_sources = [
    'src/button_actor.h',
    'src/button_actor.cpp',
    'src/clock_actor.h',
    'src/clock_actor.cpp',
    'src/graph_actor.h',
    'src/graph_actor.cpp',
    'src/main.cpp',
    'src/palette.h',
    'src/palette.cpp',
    'src/qube_actor.h',
    'src/qube_actor.cpp',
    'src/rounded_title_actor.h',
    'src/rounded_title_actor.cpp',
]

chipotherm_bin = executable('chipotherm',
    chipotherm_sources,
    dependencies: chipotherm_deps,
    install: true
)
