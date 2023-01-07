project('libsam',
	['c', 'cpp'],
	default_options : [
		'warning_level=3',
		'werror=false',
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

tinyusb_headers = include_directories('tinyusb/src')
tinyusb_sources = [
	'tinyusb/src/common/tusb_fifo.c',
	'tinyusb/src/device/usbd.c',
	'tinyusb/src/device/usbd_control.c',
	'tinyusb/src/tusb.c',
	'tinyusb/src/portable/nuvoton/nuc505/dcd_nuc505.c',
	'tinyusb/src/portable/nuvoton/nuc121/dcd_nuc121.c',
	'tinyusb/src/portable/nuvoton/nuc120/dcd_nuc120.c',
	'tinyusb/src/portable/synopsys/dwc2/dcd_dwc2.c',
	'tinyusb/src/portable/ehci/ehci.c',
	'tinyusb/src/portable/nxp/khci/dcd_khci.c',
	'tinyusb/src/portable/nxp/khci/hcd_khci.c',
	'tinyusb/src/portable/nxp/lpc17_40/hcd_lpc17_40.c',
	'tinyusb/src/portable/nxp/lpc17_40/dcd_lpc17_40.c',
	'tinyusb/src/portable/nxp/lpc_ip3511/dcd_lpc_ip3511.c',
	'tinyusb/src/portable/nxp/transdimension/dcd_transdimension.c',
	'tinyusb/src/portable/nxp/transdimension/hcd_transdimension.c',
	'tinyusb/src/portable/sunxi/dcd_sunxi_musb.c',
	'tinyusb/src/portable/sony/cxd56/dcd_cxd56.c',
	'tinyusb/src/portable/renesas/usba/dcd_usba.c',
	'tinyusb/src/portable/renesas/usba/hcd_usba.c',
	'tinyusb/src/portable/valentyusb/eptri/dcd_eptri.c',
	'tinyusb/src/portable/nordic/nrf5x/dcd_nrf5x.c',
	'tinyusb/src/portable/template/dcd_template.c',
	'tinyusb/src/portable/mentor/musb/hcd_musb.c',
	'tinyusb/src/portable/mentor/musb/dcd_musb.c',
	'tinyusb/src/portable/bridgetek/ft9xx/dcd_ft9xx.c',
	'tinyusb/src/portable/espressif/esp32sx/dcd_esp32sx.c',
	'tinyusb/src/portable/raspberrypi/pio_usb/hcd_pio_usb.c',
	'tinyusb/src/portable/raspberrypi/pio_usb/dcd_pio_usb.c',
	'tinyusb/src/portable/raspberrypi/rp2040/hcd_rp2040.c',
	'tinyusb/src/portable/raspberrypi/rp2040/dcd_rp2040.c',
	'tinyusb/src/portable/raspberrypi/rp2040/rp2040_usb.c',
	'tinyusb/src/portable/st/stm32_fsdev/dcd_stm32_fsdev.c',
	'tinyusb/src/portable/st/synopsys/dcd_synopsys.c',
	'tinyusb/src/portable/ti/msp430x5xx/dcd_msp430x5xx.c',
	'tinyusb/src/portable/dialog/da146xx/dcd_da146xx.c',
	'tinyusb/src/portable/ohci/ohci.c',
	'tinyusb/src/portable/mindmotion/mm32/dcd_mm32f327x_otg.c',
	'tinyusb/src/portable/microchip/samx7x/dcd_samx7x.c',
	'tinyusb/src/portable/microchip/samg/dcd_samg.c',
	'tinyusb/src/portable/microchip/pic32mz/dcd_pic32mz.c',
	'tinyusb/src/portable/microchip/samd/dcd_samd.c',
	'tinyusb/src/portable/chipidea/ci_hs/hcd_ci_hs.c',
	'tinyusb/src/portable/chipidea/ci_hs/dcd_ci_hs.c',
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

samd_defines = ['-D__SAMD21E16BU__']
sam = static_library('sam',
	'src/newlib_syscalls.c',
	'src/bod.cpp',
	'src/clock.cpp',
	'src/gpio.cpp',
	'src/pinmux.cpp',
	'src/sercom_usart.cpp',
	'src/serial_number.cpp',
	'SipHash/siphash.c',
	'SipHash/halfsiphash.c',
	'samd21_pack/samd21b/gcc/gcc/startup_samd21.c',
	tinyusb_sources,

	c_args: samd_defines,
	cpp_args: samd_defines,
	include_directories: [samd_headers, cmsis_headers, our_headers, tinyusb_headers, siphash_headers]
)
