=================
python_c_sockets
=================

A demo of IPv6 UDP datagram on C and Python limited by interface speed (approaching 1Gbps on my laptop) i.e. relatively high performance code.

The server (C-code) goes on the device, the Python code runs on the laptop. Of course, one can equally well use Python for both by creating a ``socksource.py`` program equivalent to ``socksource.c``.

The data flow rate on localhost ::1 with gigabit network card is 1Gbps.



Install
=======
::

  make socksource.c
  
Usage
=====

Loopback
--------
open a terminal::
  
  ./socksource 0
  
open second terminal::

  nc -u ::1 2000
  
and you will see your text you type echoed back to you when you press Return key

Streaming float array
---------------------
This demo sends 8192 byte float32 arrays, by first sending an int telling the length of the upcoming float32 array

Open a terminal::
  
  ./socksource 1
  
Open another terminal::

  ./socksink.py
  
I see ~70 microseconds cycle time on my laptop --> 8192 / 70e-6 * 8 ~ 936 Mbps
