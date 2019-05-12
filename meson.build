project('Sockets and HDF5 with C and Python', 'c',
  meson_version : '>=0.50',
  default_options : ['default_library=static', 'c_std=c11'])

cc = meson.get_compiler('c')
ccid = cc.get_id()
os = build_machine.system()

if os == 'windows'
  error('Try Cygwin or WSL, native Windows requires different libraries')
elif ccid == 'gcc'
  add_global_arguments('-Wextra', '-Wpedantic', language : 'c')
endif

executable('multicast_rx', 'multicast_rx.c')
executable('multicast_tx', 'multicast_tx.c')
executable('unicast_rx', 'unicast_rx.c')
executable('unicast_tx', 'unicast_tx.c')
#----- linux only ----------
#get_ifc = executable('get_interfaces', 'linux/get_interfaces.c')
#test('InterfaceNames', get_ifc)
#------------ HDF5  ----------
hdf5 = dependency('hdf5', required: false)
if hdf5.found()
	executable('unicast_rx_h5', 'hdf5/unicast_rx_h5.c', dependencies : hdf5)
endif
