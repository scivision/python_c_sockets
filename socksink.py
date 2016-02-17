#!/usr/bin/env python3
"""
Simple send/receive to companion UDP C-code server on localhost or other device
prints rolling mean of cycle time (typical <80 microseconds on localhost)

It does 2 UDP reads per cycle:
1) get number of float32 via int
2) get float32 array
"""
from numpy import diff,mean
from numpy.testing import assert_allclose
from struct import unpack
from time import time
import socket

HOST='::1' # '::1' is to ipv6 what 'localhost' is to ipv4
PORT=2000
BUFSIZE=8192
Nelbyte = 4 #4 bytes for float32

def socksink():

    S = socket.socket(socket.AF_INET6,socket.SOCK_DGRAM)
    # Do NOT connect or bind
    try:

      first=True

      for i in range(100000000):
        tic=time()
#%% host-> device
        S.sendto(b'\n',(HOST,PORT,0,0))
#%% device -> host
        Nel = unpack('<1i',S.recv(4))[0] #int len
        Nbyte_dg = Nel*Nelbyte

        A = S.recv(Nbyte_dg)

        if not A:
            continue
#%% parse result
        dat = unpack('<{:d}f'.format(Nel),A)
        if first: #to avoid having to restart C code each time
            first=False
            last=dat[0]-1
            rtoc=time()-tic

        rtoc = mean((time()-tic,rtoc))

        if not i % 1000: print(rtoc)

        #assert_allclose(diff(dat),1,err_msg=str(dat))
        assert_allclose(last,dat[0]-1,err_msg=str(last))
        last = dat[-1]

    except KeyboardInterrupt:
        pass
    finally:
        S.close()

#def getselfaddr():
    #return socket.getaddrinfo(None, PORT, socket.AF_INET6, socket.SOCK_DGRAM,socket.IPPROTO_IP)[0]

if __name__ == "__main__":
    socksink()
