import socket
import time

from . import NPKT


def ipv4bcast_rx(port: int, N: int = NPKT, timeout: float = 10.0):

    L = 16

    with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as s:
        s.settimeout(timeout)
        s.bind(("", port))

        for i in range(N):
            print("package", i)
            m = s.recv(L)
            print(m)


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
