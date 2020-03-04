project('Sockets and HDF5 with C and Python', 'c',
  meson_version : '>=0.50',
  default_options : ['default_library=static', 'buildtype=release', 'warning_level=3'])

cc = meson.get_compiler('c')
os = build_machine.system()

if os == 'windows'
  error('Try Cygwin or WSL, native Windows requires different libraries')
endif

executable('multicast_rx', 'multicast_rx.c')
executable('multicast_tx', 'multicast_tx.c')
executable('unicast_rx', 'unicast_rx.c')
executable('unicast_tx', 'unicast_tx.c')

if os == 'linux'
  get_ifc = executable('get_interfaces', 'linux/get_interfaces.c')
  test('InterfaceNames', get_ifc, timeout: 10)
endif
#------------ HDF5  ----------
hdf5 = dependency('hdf5', required: false)
if hdf5.found()
	executable('unicast_rx_h5', 'hdf5/unicast_rx_h5.c', dependencies : hdf5)
endif
