#!/usr/bin/env python3
"""
Receive UDP broadcast packets
Only possible on IPv4, not recommended for actual use.
1024 bytes max.
"""

import socket


def ipv4bcast_rx(port: int, N: int, timeout: float = 15.0):

    L = 16

    with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as s:
        s.settimeout(timeout)
        s.bind(("", port))

        for i in range(N):
            print("package", i)
            m = s.recv(L)
            print(m)


if __name__ == "__main__":
    ipv4bcast_rx(port=2000, N=5, timeout=30.0)
