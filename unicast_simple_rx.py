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

import socket
import time

from argparse import ArgumentParser


def udp_simple_rx(
    host: str,
    port: int,
    timeout: float = 10.0,
):

    M = 1000000
    L = 1024

    with socket.socket(socket.AF_INET6, socket.SOCK_DGRAM) as s:
        s.settimeout(timeout)
        s.bind((host, port))
        tic = time.monotonic()
        i = 1
        while True:
            m = s.recv(L)  # noqa: F841
            # print(m, "\r", end="")

            i += 1
            if i % M == 0:
                bw = M * L / (time.monotonic() - tic) / 1e6
                print(f"{bw:.3f} Mbytes/sec\r", end="")
                tic = time.monotonic()


if __name__ == "__main__":
    p = ArgumentParser(description="UDP unicast RX demo")
    p.add_argument("-i", "--host", help="IPv6 address of receiver. ::1 is loopback", default="::1")
    p.add_argument("-p", "--port", help="port", type=int, default=2000)
    p = p.parse_args()

    print("unicast client", p.host, "port", p.port)

    udp_simple_rx(p.host, p.port)
