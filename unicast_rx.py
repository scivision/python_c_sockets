#!/usr/bin/env python3
"""
UDP Unicast send/receive to companion UDP C-code server on localhost or other device
prints rolling mean of cycle time (typical <80 microseconds on localhost)

It does 2 UDP reads per cycle:
1) get number of float32 via int
2) get float32 array

Example:
1) ./unicast_tx   (compiled from unicast_tx.c)
2) python unicast_rx.py
3) iftop -i lo   (watch traffic, approaching 1 Gb/sec)
"""
from argparse import ArgumentParser

import socket
import time
import struct
from pathlib import Path
import sys

import numpy as np

try:
    import h5py
except ImportError:
    pass


def udpunicast(
    host: str,
    port: int,
    h5fn: Path | None = None,
    *,
    Nupdate: int = 1000,
    bufsize: int = 8192,
    Nelbyte: int = 4,
    N: int = 5,
    timeout: float = 5.0,
):
    """
    maxshape parameters:
    limit ~ 100000*BUFSIZE to overfill hardrive with extremely fast demo
    normally you would take other more sensible measures, and have sane datarates.
    None means unlimited size (entire hard drive)
    """

    # Do NOT connect or bind
    with socket.socket(socket.AF_INET6, socket.SOCK_DGRAM) as S:
        S.settimeout(timeout)
        first = True

        if "h5py" in sys.modules and h5fn:
            h5 = h5py.File(h5fn, "w")
            h5d = h5.create_dataset(
                "/data",
                dtype=np.float32,
                chunks=True,
                shape=(Nupdate * bufsize / Nelbyte,),
                maxshape=(100000 * bufsize / Nelbyte,),
            )
        else:
            h5 = h5d = None

        # %% main loop
        for i in range(N):
            tic = time.time()
            # %% host-> device
            # host (other program) is programmed to send payload for any input--including simply '\n'
            S.sendto(b"\n", (host, port, 0, 0))
            # %% device -> host
            # print("receiving")
            Nel = struct.unpack("<1L", S.recv(4))[0]  # int len
            print(Nel)
            Nbyte_dg = Nel * Nelbyte

            A = S.recv(Nbyte_dg)

            if len(A) != Nbyte_dg:
                print("unicast_rx: could not determine length")
                continue
            print(len(A))
            # %% parse result
            dat = struct.unpack(f"<{Nel:d}f", A)
            if first:  # to avoid having to restart C code each time
                first = False
                # last = dat[0] - 1
                rtoc = time.time() - tic

            rtoc = (time.time() - tic + rtoc) / 2
            # %% optional write to disk
            if h5 is not None:
                h5d[i * Nel : (i + 1) * Nel] = dat
            # %% progress report / writing
            if not i % Nupdate:
                print(rtoc, i)
                h5 = _updatemsg(i, h5, h5d, Nupdate, bufsize, Nelbyte)
            # %% data validation due to a priori pattern

            # for single precision float, large integer jumps are experienced at large values by IEEE754 definition
            # assert_allclose(diff(dat),1,err_msg=str(dat))

            # assert np.allclose(
            #     last, dat[0] - 1
            # ), f"{last} != {dat[0] - 1}, be mindful of min/max values of the datatype"

            # last = dat[-1]

    if h5 is not None:
        h5.close()


def _updatemsg(i: int, h5, h5d, Nupdate: int, bufsize: int, Nelbyte: int):
    if h5 is not None:
        h5.flush()  # NOTE: every N loops to improve performance
        try:
            h5d.resize((i + Nupdate) * bufsize / Nelbyte, axis=0)
        except ValueError as e:
            print(
                "stopping HDF5 writing for this fast demo. One can set maxshape=(None,) for no limit.",
                e,
            )
            h5.close()
            h5 = None

    return h5


def main():
    p = ArgumentParser(description="UDP unicast RX demo with optional HDF5 writing")
    p.add_argument("-o", "--ofn", help="output hdf5 filename to write")
    p.add_argument("-i", "--host", help="IPv6 address of receiver. ::1 is loopback", default="::1")
    p.add_argument("-p", "--port", help="port", type=int, default=2001)
    p = p.parse_args()

    print("unicast server", p.host, "port", p.port)

    udpunicast(p.host, p.port, p.ofn)


if __name__ == "__main__":
    main()
