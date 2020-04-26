project('Pocket Knife', 'c')

includes = include_directories('vendor')
test_includes = include_directories('src')

src = ['src/main.c', 'src/draw.c', 'src/impl.c', 'src/font.c']
executable('pk', src, include_directories: includes, gui_app: true)

subdir('tests')
