project('pgl_demo', 'c',
        version : '1.0.0',
        default_options: 'default_library=static')

sdl2_dep = dependency('sdl2', version: '>=2.0.0')

sources = ['demo.c']

executable('pgl_demo', sources,
           include_directories: '.', dependencies: sdl2_dep)
