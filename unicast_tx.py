#!/usr/bin/env python
"""
UDP Unicast send/receive to companion UDP C-code server on localhost or other device
prints rolling mean of cycle time (typical <80 microseconds on localhost)

Example:
1) python unicast_tx.py
2) python unicast_rx.py
3) iftop -i lo   (watch traffic, approaching 1 Gb/sec)
"""
from argparse import ArgumentParser
import python_c_sockets.tx as pcs


def main():
    p = ArgumentParser(description="UDP unicast TX demo")
    p.add_argument("-i", "--host", help="IPv6 address of receiver. ::1 is loopback", default="::1")
    p.add_argument("-p", "--port", help="port", type=int, default=2000)
    p = p.parse_args()

    print("unicast: ", p.host, "port", p.port)

    pcs.unicast_tx(p.host, p.port)


if __name__ == "__main__":
    main()
