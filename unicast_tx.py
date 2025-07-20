#!/usr/bin/env python3
"""
UDP Unicast send/receive to companion UDP C-code server on localhost or other device
prints rolling mean of cycle time (typical <80 microseconds on localhost)

Example:
1) python unicast_tx.py
2) python unicast_rx.py
3) iftop -i lo   (watch traffic, approaching 1 Gb/sec)
"""

import socket

from argparse import ArgumentParser


def unicast_tx(host: str, port: int, N: int):

    L = 1024

    i = 0
    with socket.socket(socket.AF_INET6, socket.SOCK_DGRAM) as s:
        for _ in range(N):
            payload = i.to_bytes(L, "big")
            s.sendto(payload, (host, port, 0, 0))
            i += 1


if __name__ == "__main__":
    p = ArgumentParser(description="UDP unicast TX demo")
    p.add_argument("-i", "--host", help="IPv6 address of receiver. ::1 is loopback", default="::1")
    p.add_argument("-p", "--port", help="port", type=int, default=2001)
    p.add_argument("-n", "--N", help="number of packets to send", type=int, default=1000)
    args = p.parse_args()

    print("unicast: ", args.host, "port", args.port)

    unicast_tx(args.host, args.port, args.N)
