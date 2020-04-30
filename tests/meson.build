# Sources from the main project
sources = ['../src/impl.c', '../src/font.c', '../src/log.c', '../src/buffer.c']

test_font = executable('test_font', ['test_font.c'] + sources, include_directories: includes, dependencies: libm)
test('fonts', test_font)

test_buffer = executable('test_buffer', ['test_buffer.c'] + sources, include_directories: includes, dependencies: libm)
test('buffer', test_buffer)
