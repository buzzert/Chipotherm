project('chipotherm', 'cpp')

libbubbles_proj = subproject('bubbles')
libbubbles_dep  = libbubbles_proj.get_variable('bubbles_dep')

chipotherm_deps = [
    libbubbles_dep,
]

chipotherm_sources = [
    'src/clock_actor.cpp',
    'src/main.cpp',
    'src/palette.h',
    'src/qube_actor.h',
    'src/qube_actor.cpp',
    'src/clock_actor.h',
]

chipotherm_bin = executable('chipotherm',
    chipotherm_sources,
    dependencies: chipotherm_deps,
    install: true
)


