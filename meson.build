project('Pocket Knife', 'c')

cc = meson.get_compiler('c')

# TODO: See if we actually need libm
libm = cc.find_library('m', required: false)

includes = include_directories('vendor')

src = [
	'src/win32_main.c',
	'src/linux_main.c',
	'src/draw.c',
	'src/impl.c',
	'src/font.c',
	'src/log.c',
	'src/buffer.c'
	]

executable('pk', src, include_directories: includes, gui_app: true, dependencies: libm)

tests = [
'font',
'buffer'
]

foreach test_name : tests
	test(
		test_name,
		executable(
			'test' + test_name,
			src + ['tests/test_' + test_name + '.c'],
			include_directories: include_directories(['vendor', 'src']),
			dependencies: libm))
endforeach
