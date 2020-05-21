project('Pocket Knife', 'c')

cc = meson.get_compiler('c')

# TODO: See if we actually need libm
libm = cc.find_library('m', required: false)

includes = include_directories('vendor')

src = [
	'src/draw.c',
	'src/impl.c',
	'src/font.c',
	'src/log.c',
	'src/buffer.c'
]

exe_src = src + ['src/linux_main.c', 'src/win32_main.c']
exe = executable('pk', exe_src, include_directories: includes, gui_app: true, dependencies: libm)

tests = [
	'font',
	'buffer'
]

foreach test_name : tests
	test(
		test_name,
		executable(
			'test_' + test_name,
			src + ['tests/test_' + test_name + '.c'],
			include_directories: include_directories(['vendor', 'src']),
			dependencies: libm))
endforeach
