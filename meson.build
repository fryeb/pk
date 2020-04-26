project('Pocket Knife', 'c')

cc = meson.get_compiler('c')
libm = cc.find_library('m', required: false)
includes = include_directories('vendor')

src = ['src/win32_main.c', 'src/linux_main.c', 'src/draw.c', 'src/impl.c', 'src/font.c']
executable('pk', src, include_directories: includes, gui_app: true, dependencies: libm)

subdir('tests')
