#!/usr/bin/env python3
"""
UDP Unicast send/receive to companion UDP C-code server on localhost or other device
prints rolling mean of cycle time (typical <80 microseconds on localhost)

It does 2 UDP reads per cycle:
1) get number of float32 via int
2) get float32 array

Example:
1) python unicast_tx.py
2) python unicast_rx.py
3) iftop -i lo   (watch traffic, approaching 1 Gb/sec)
"""
from argparse import ArgumentParser
import python_c_sockets.rx as pcs


def main():
    p = ArgumentParser(description="UDP unicast RX demo")
    p.add_argument("-i", "--host", help="IPv6 address of receiver. ::1 is loopback", default="::1")
    p.add_argument("-p", "--port", help="port", type=int, default=2000)
    p = p.parse_args()

    print("unicast client", p.host, "port", p.port)

    pcs.udp_simple_rx(p.host, p.port)


if __name__ == "__main__":
    main()
