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
import python_c_sockets as pcs


def main():
    p = ArgumentParser(description='UDP unicast RX demo with optional HDF5 writing')
    p.add_argument('-o', '--ofn', help='output hdf5 filename to write')
    p.add_argument('-i', '--host', help='IPv6 address of receiver. ::1 is loopback', default='::1')
    p.add_argument('-p', '--port', help='port', type=int, default=2000)
    p = p.parse_args()

    print('unicast server', p.host, 'port', p.port)

    pcs.udpunicast(p.host, p.port, p.ofn)


if __name__ == "__main__":
    main()
