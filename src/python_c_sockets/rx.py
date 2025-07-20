import socket
import time

from . import NPKT


def ipv4bcast_rx(port: int, N: int = NPKT, timeout: float = 15.0):

    L = 16

    with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as s:
        s.settimeout(timeout)
        s.bind(("", port))

        for i in range(N):
            print("package", i)
            m = s.recv(L)
            print(m)
