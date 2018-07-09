#!/usr/bin/env python
"""
UDP Unicast send/receive to companion UDP C-code server on localhost or other device
prints rolling mean of cycle time (typical <80 microseconds on localhost)

It does 2 UDP reads per cycle:
1) get number of float32 via int
2) get float32 array

Example:
1) ./unicast_tx   (compiled from unicast_tx.c)
2) ./unicast_rx.py
3) iftop -i lo   (watch traffic, approaching 1 Gb/sec)
"""
from argparse import ArgumentParser
from pathlib import Path
import numpy as np
import struct
from time import time
import socket
try:
    import h5py
except ImportError:
    h5py = None

Nupdate = 1000  # update/write every N loops
BUFSIZE = 8192
Nelbyte = 4  # 4 bytes for float32
N = 1000000


def udpunicast(host: str, port: int, h5fn: Path=None):
    """
    maxshape parameters:
    limit ~ 100000*BUFSIZE to overfill hardrive with extremely fast demo
    normally you would take other more sensible measures, and have sane datarates.
    None means unlimited size (entire hard drive)
    """

    S = socket.socket(socket.AF_INET6, socket.SOCK_DGRAM)
    # Do NOT connect or bind
    try:
        first = True

        if h5py is not None and h5fn:
            h5 = h5py.File(h5fn, 'w')
            h5d = h5.create_dataset('/data',
                                    dtype=np.float32,
                                    chunks=True,
                                    shape=(Nupdate*BUFSIZE/Nelbyte,),
                                    maxshape=(100000*BUFSIZE/Nelbyte,))
        else:
            h5 = h5d = None

# %% main loop
        for i in range(N):
            tic = time()
# %% host-> device
            # host (other program) is programmed to send payload for any input--including simply '\n'
            S.sendto(b'\n', (host, port, 0, 0))
# %% device -> host
            Nel = struct.unpack('<1L', S.recv(4))[0]  # int len
            Nbyte_dg = Nel*Nelbyte

            A = S.recv(Nbyte_dg)

            if len(A) != Nbyte_dg:
                continue
# %% parse result
            dat = struct.unpack('<{:d}f'.format(Nel), A)
            if first:  # to avoid having to restart C code each time
                first = False
                last = dat[0]-1
                rtoc = time()-tic

            rtoc = np.mean((time()-tic, rtoc))
# %% optional write to disk
            if h5 is not None:
                h5d[i*Nel:(i+1)*Nel] = dat
# %% progress report / writing
            if not i % Nupdate:
                print(rtoc, i)
                h5 = updatemsg(i, h5, h5d)
# %% data validation due to a priori pattern

# for single precision float, large integer jumps are experienced at large values by IEEE754 definition
            # assert_allclose(diff(dat),1,err_msg=str(dat))

            np.testing.assert_allclose(last, dat[0]-1,
                                       err_msg=str(last)+' be mindful of min/max values of your datatype')

            last = dat[-1]
    except KeyboardInterrupt:
        pass

    S.close()
    if h5 is not None:
        h5.close()


def updatemsg(i: int, h5: h5py.Dataset, h5d) -> h5py.Dataset:
    if h5 is not None:
        h5.flush()  # NOTE: every N loops to improve performance
        try:
            h5d.resize((i+Nupdate)*BUFSIZE/Nelbyte, axis=0)
        except ValueError as e:
            print('stopping HDF5 writing for this fast demo. in real life you can set maxshape=(None,) for no limit.', e)
            h5.close()
            h5 = None

    return h5

# def getselfaddr():
    # return socket.getaddrinfo(None, PORT, socket.AF_INET6, socket.SOCK_DGRAM,socket.IPPROTO_IP)[0]


def main():
    p = ArgumentParser(
        description='UDP unicast RX demo with optional HDF5 writing')
    p.add_argument('-o', '--ofn', help='output hdf5 filename to write')
    p.add_argument(
        '-i', '--host', help='IPv6 address of receiver. ::1 is loopback', default='::1')
    p.add_argument('-p', '--port', help='port', type=int, default=2000)
    p = p.parse_args()

    print('unicast server', p.host, 'port', p.port)

    udpunicast(p.host, p.port, p.ofn)


if __name__ == "__main__":
    main()
