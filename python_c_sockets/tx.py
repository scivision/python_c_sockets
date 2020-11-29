import socket
import time

from . import NPKT


def ipv4bcast_tx(port: int, N: int = NPKT, twait: float = 0.1, verbose: bool = True):

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


def unicast_tx(
    host: str,
    port: int,
    twait: float = 0.1,
):

    L = 1024

    i = 0
    with socket.socket(socket.AF_INET6, socket.SOCK_DGRAM) as s:
        while True:
            payload = i.to_bytes(L, "big")
            s.sendto(payload, (host, port, 0, 0))
            i += 1
