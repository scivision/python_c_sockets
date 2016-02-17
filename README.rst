=================
python_c_sockets
=================

A demo of IPv6 UDP datagram on C and Python limited by interface speed (e.g. 1Gbps) i.e. relatively high performance code.

The server (C-code) goes on the device, the Python code runs on the laptop. Of course, one can equally well use Python for both.

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
  
I see <80 microseconds on my laptop.
