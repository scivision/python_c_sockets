#!/usr/bin/env python3
"""
UDP Unicast send/receive to companion UDP C-code server on localhost or other device
prints rolling mean of cycle time (typical <80 microseconds on localhost)

It does 2 UDP reads per cycle:
1) get number of float32 via int
2) get float32 array
"""
from numpy import diff,mean,float32
from numpy.testing import assert_allclose
from struct import unpack
from time import time
import socket
try:
    import h5py
except ImportError:
    h5py=None

Nupdate = 1000 #update/write every N loops
BUFSIZE=8192
Nelbyte = 4 #4 bytes for float32

def udpunicast(host,port,h5fn=None):

    S = socket.socket(socket.AF_INET6,socket.SOCK_DGRAM)
    # Do NOT connect or bind
    try:
      first=True

      if h5py is not None and h5fn:
          h5 = h5py.File(h5fn,'w',libver='latest')
          h5d = h5.create_dataset('/data',
                            dtype=float32,
                            chunks=True,
                            shape=(Nupdate*BUFSIZE/Nelbyte,),
                            # I set limit to 100000*BUFSIZE so as not to overfill your hardrive with this extremely fast demo
                            # normally you would take other more sensible measures, and have sane datarates.
                            maxshape=(100000*BUFSIZE/Nelbyte,)) #None means unlimited size (entire hard drive)
      else:
          h5 = None

      i=0
      while True:
        tic=time()
#%% host-> device
        S.sendto(b'\n',(host,port,0,0))
#%% device -> host
        Nel = unpack('<1L',S.recv(4))[0] #int len
        Nbyte_dg = Nel*Nelbyte

        A = S.recv(Nbyte_dg)

        if len(A) != Nbyte_dg:
            continue
#%% parse result
        dat = unpack('<{:d}f'.format(Nel),A)
        if first: #to avoid having to restart C code each time
            first=False
            last=dat[0]-1
            rtoc=time()-tic

        rtoc = mean((time()-tic,rtoc))
#%% optional write to disk
        if h5 is not None:
            h5d[i*Nel:(i+1)*Nel] = dat
#%% progress report / writing
        if not i % Nupdate:
            print(rtoc,i)
            if h5 is not None:
                h5.flush() # NOTE: every N loops to improve performance
                try:
                    h5d.resize((i+Nupdate)*BUFSIZE/Nelbyte,axis=0)
                except ValueError as e:
                    print('stopping HDF5 writing for this fast demo, in real life you can set maxshape=(None,) for no limit.  {}'.format(e))
                    h5.close()
                    h5=None
#%% data validation due to a priori pattern

        #for single precision float, large integer jumps are experienced at large values by IEEE754 definition
        #assert_allclose(diff(dat),1,err_msg=str(dat))

        assert_allclose(last,dat[0]-1,err_msg=str(last)+' be mindful of min/max values of your datatype')
        last = dat[-1]

        i+=1

    except KeyboardInterrupt:
        pass
    finally:
        S.close()
        if h5 is not None: h5.close()

#def getselfaddr():
    #return socket.getaddrinfo(None, PORT, socket.AF_INET6, socket.SOCK_DGRAM,socket.IPPROTO_IP)[0]

if __name__ == "__main__":
    from argparse import ArgumentParser
    p = ArgumentParser(description='UDP unicast RX demo with optional HDF5 writing')
    p.add_argument('-o','--ofn',help='output hdf5 filename to write')
    p.add_argument('-i','--host',help='IPv6 address of server ::1 is loopback',default='::1')
    p.add_argument('-p','--port',help='port',type=int,default=2000)
    p = p.parse_args()


    udpunicast(p.host,p.port,p.ofn)
