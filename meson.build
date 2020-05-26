project('Sockets and HDF5 with C and Python', 'c',
  meson_version : '>=0.50',
  default_options : ['default_library=static', 'buildtype=release', 'warning_level=3'])

os = build_machine.system()

if os == 'windows'
  error('Try Cygwin or WSL, native Windows requires different libraries')
endif

subdir('src')
