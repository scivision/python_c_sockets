#!/usr/bin/env python3
"""
Send UDP broadcast packets
Only possible on IPv4, not recommended for actual use.
1024 bytes max.
"""

import socket
import time


def ipv4bcast_tx(port: int, N: int = 5, twait: float = 0.1, verbose: bool = True):

    L = 16

    with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as s:
        s.bind(("", 0))
        s.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)

        for i in range(N):
            payload = i.to_bytes(L, "big")
            s.sendto(payload, ("<broadcast>", port))
            if verbose:
                print("broadcast", payload, "\r", end="")

            time.sleep(twait)


if __name__ == "__main__":
    ipv4bcast_tx(port=2000)
