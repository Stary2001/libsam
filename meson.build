project('libsam',
	['c', 'cpp'],
	default_options : [
		'warning_level=3',
		'werror=false',
		# This project defaults to a release build
		'debug=false',
		'optimization=2',
		# `build.*` options affect `native: true targets`
		# plain options affect `native: false` targets.
		'c_std=c11', 'build.c_std=c11',
		'cpp_std=c++11', 'build.cpp_std=c++11',
		'default_library=static'
	],
	license: 'MIT',
	meson_version: '>=0.54.0',
	version: '1.0'
)

samd_variant = get_option('samd_variant')
samd_device = get_option('samd_device')
assert(samd_variant != '', 'Please specify samd_variant')
assert(samd_device != '', 'Please specify samd_device')

samd_headers = include_directories('samd21_pack/'+samd_variant+'/include')
cmsis_headers = include_directories('CMSIS_5/CMSIS/Core/Include/')
our_headers = include_directories('include')

samd_defines = ['-D__SAMD21E16BU__']

sam = static_library('sam',
	'newlib_syscalls.c',
	'bod.cpp',
	'clock.cpp',
	'gpio.cpp',
	'pinmux.cpp',
	'sercom_usart.cpp',
	'samd21_pack/samd21b/gcc/gcc/startup_samd21.c',

	c_args: samd_defines,
	cpp_args: samd_defines,
	include_directories: [samd_headers, cmsis_headers, our_headers]
)
