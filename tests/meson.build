sources = ['../src/impl.c', '../src/font.c', '../src/log.c']
test_font = executable('test_font', ['test_font.c'] + sources, include_directories: includes, dependencies: libm)
test('fonts', test_font)
