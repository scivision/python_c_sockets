=================
python_c_sockets
=================

A demo of IPv6 UDP datagram on C and Python limited by interface speed (approaching 1Gbps on my laptop) i.e. relatively high performance code.

The server (C-code) goes on the device, the Python code runs on the laptop. 

The data flow rate on localhost ::1 with gigabit network card is 1Gbps.

.. contents::

Prereqs
=======
::

    sudo apt-get install gcc cmake libhdf5-dev

Compile
=======
::

  cd bin
  cmake ..
  make

  
Unicast
=======

Loopback
--------
open a terminal::
  
  ./unicast_tx.out 0
  
open second terminal::

  nc -u ::1 2000
  
and you will see your text you type echoed back to you when you press Return key

Streaming float array
---------------------
This demo sends 8192 byte float32 arrays, by first sending an int telling the length of the upcoming float32 array

Open a terminal::
  
  ./unicast_tx.out 1
  
Open another terminal::

  ./unicast_rx.py
  
I see ~70 microseconds cycle time on my laptop --> 8192 / 70e-6 * 8 ~ 936 Mbps

Write streaming data to HDF5
----------------------------
HDF5 files can be as large as your hard drive, and these tests run extremely fast,
so I disabled the unlimited writing so that you didn't write a huge file.

Python
~~~~~~

Run the ``unicast_tx.out 1`` and in the second terminal window run::

    ./unicast_rx.py -o test.h5


C
~

Run the ``unicast_tx.out 1`` and in the second terminal window run::

    ./unicast_rx_h5.out


Multicast
=========
open a terminal::

    ./multicast_tx.out

open a second terminal::

    ./multicast_rx.out
