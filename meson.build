project('libsam',
	['c', 'cpp'],
	default_options : [
		'warning_level=3',
		'werror=false',
		# `build.*` options affect `native: true targets`
		# plain options affect `native: false` targets.
		'c_std=c11', 'build.c_std=c11',
		'cpp_std=c++17', 'build.cpp_std=c++17',
		'default_library=static'
	],
	license: 'MIT',
	meson_version: '>=0.54.0',
	version: '1.0'
)

samd_device = get_option('samd_device')
assert(samd_device != '', 'Please specify samd_device')

samd_variant = get_option('samd_variant')
assert(samd_variant != '', 'Please specify samd_variant')

samd_defines = []
if samd_device.startswith('SAMD21')
	samd_dir = 'samd21_pack/' + samd_variant
	samd_variant_startup_c = 'samd21_pack/' + samd_variant + '/gcc/gcc/startup_samd21.c'
	samd_defines += ['-DSAMD21']
elif samd_device.startswith('SAMD11')
	samd_dir = 'samd11_pack'
	samd_variant_startup_c = 'samd11_pack/gcc/gcc/startup_samd11.c'
	samd_defines += ['-DSAMD11']
else
	// fail
endif

samd_headers = include_directories(samd_dir+'/include')
cmsis_headers = include_directories('CMSIS_5/CMSIS/Core/Include/')
our_headers = include_directories('include')

tinyusb_headers = include_directories('tinyusb/src')
tinyusb_sources = [
	'tinyusb/src/common/tusb_fifo.c',
	'tinyusb/src/device/usbd.c',
	'tinyusb/src/device/usbd_control.c',
	'tinyusb/src/tusb.c',
	
	'tinyusb/src/portable/microchip/samd/dcd_samd.c',

	'tinyusb/src/class/audio/audio_device.c',
	'tinyusb/src/class/dfu/dfu_device.c',
	'tinyusb/src/class/dfu/dfu_rt_device.c',
	'tinyusb/src/class/vendor/vendor_host.c',
	'tinyusb/src/class/vendor/vendor_device.c',
	'tinyusb/src/class/msc/msc_device.c',
	'tinyusb/src/class/msc/msc_host.c',
	'tinyusb/src/class/usbtmc/usbtmc_device.c',
	'tinyusb/src/class/hid/hid_host.c',
	'tinyusb/src/class/hid/hid_device.c',
	'tinyusb/src/class/bth/bth_device.c',
	'tinyusb/src/class/net/ecm_rndis_device.c',
	'tinyusb/src/class/net/ncm_device.c',
	'tinyusb/src/class/video/video_device.c',
	'tinyusb/src/class/cdc/cdc_rndis_host.c',
	'tinyusb/src/class/cdc/cdc_host.c',
	'tinyusb/src/class/cdc/cdc_device.c',
	'tinyusb/src/class/midi/midi_device.c',
	'tinyusb/src/host/hub.c',
	'tinyusb/src/host/usbh.c',
]
siphash_headers = include_directories('SipHash')

samd_defines += ['-D__'+samd_device.to_upper()+'__']
sam = static_library('sam',
	'src/newlib_syscalls.cpp',
	'src/adc.cpp',
	'src/bod.cpp',
	'src/clock.cpp',
	'src/gpio.cpp',
	'src/pinmux.cpp',
	'src/tc.cpp',
	'src/tcc.cpp',
	'src/sercom_usart.cpp',
	'src/sercom_spi.cpp',
	'src/serial_number.cpp',
	'SipHash/siphash.c',
	'SipHash/halfsiphash.c',
	samd_variant_startup_c,
	tinyusb_sources,

	c_args: samd_defines,
	cpp_args: samd_defines,
	include_directories: [samd_headers, cmsis_headers, our_headers, tinyusb_headers, siphash_headers]
)
