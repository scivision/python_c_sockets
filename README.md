[![image](https://travis-ci.org/scivision/python_c_sockets.svg?branch=master)](https://travis-ci.org/scivision/python_c_sockets)
[![Build status](https://ci.appveyor.com/api/projects/status/n53e7oll4d1sj9m2?svg=true)](https://ci.appveyor.com/project/scivision/python-c-sockets)

# Python C sockets

Multi-platform (BSD, Linux, Mac, Windows Subsystem for Linux) demo of IPv6 UDP datagram on C and Python.
Maximum speed is only limited by interface speed--approaching 1Gbps on my laptop.
This is simple yet performant code.

The server (C-code) goes on the device, the Python code runs on the laptop.

The data flow rate on localhost `::1` with gigabit network card is 1 Gbps.

## Build

1. prereqs:
   ```sh
   apt install gcc cmake libhdf5-dev
   ```
2. Compile
   ```sh
   cd bin
   cmake ..
   cmake --build .
   ```

## Unicast examples

### Loopback

1. open terminal:
   ```sh
   ./unicast_tx
   ```
2. open another terminal:
   ```sh
   nc -u ::1 2000
   ```

You will see text you type echoed back to you when you press Return key

### Streaming float array

This demo sends 8192 byte float32 arrays, by first sending an int
telling the length of the upcoming float32 array

1. Open terminal:
   ```sh
   ./unicast_tx
   ```
2. Open another terminal:
   ```sh
   ./unicast_rx.py
   ```
I see ~70 microseconds cycle time on my laptop --> 8192 / 70e-6 * 8
~ 936 Mbps

### Write streaming data to HDF5

HDF5 files can be as large as your hard drive, and these tests run
extremely fast, so I disabled the unlimited writing so that you didn\'t
write a huge file.

#### Python

Run the `unicast_tx` and in the second terminal window run:
```sh
./unicast_rx.py -o test.h5
```

#### C

Run the `unicast_tx` and in the second terminal window run:
```sh
./unicast_rx
```

## IPv6 Multicast examples

1. open a terminal:
   ```sh
   ./multicast_tx
   ```
2. open a second terminal:
   ```sh
   ./multicast_rx
   ```

### IPv4 multicast
In general IPv6 should be used instead.

1. open a terminal:
   ```sh
   ./broadcast_ipv4.py
   ```


## Notes

* Windows native would require some rewriting for Winsock library and `#ifdef`. These days, it may be better to use WSL or Cygwin instead of native Windows.
