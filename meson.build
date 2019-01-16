project('Sockets and HDF5 with C and Python', 'c', default_options : ['default_library=static', 'c_std=c11'])

cc = meson.get_compiler('c')
if cc.get_id() == 'gcc'
  add_global_arguments('-Wall', '-Wextra', '-Wpedantic', language : 'c')
endif

executable('multicast_rx', 'multicast_rx.c')
executable('multicast_tx', 'multicast_tx.c')
executable('unicast_rx', 'unicast_rx.c')
executable('unicast_tx', 'unicast_tx.c')
#----- linux only ----------
#get_ifc = executable('get_interfaces', 'linux/get_interfaces.c')
#test('InterfaceNames', get_ifc)
#------------ HDF5  ----------
hdf5 = dependency('hdf5')
executable('unicast_rx_h5', 'hdf5/unicast_rx_h5.c', dependencies : hdf5)

