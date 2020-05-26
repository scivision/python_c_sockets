import socket
import time

NPKT = 10000


def ipv4bcast_tx(port: int, N: int = NPKT, twait: float = 0.1, verbose: bool = True):
    with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as s:
        s.bind(("", 0))
        s.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)

        for i in range(N):
            payload = i.to_bytes(14, "big")
            s.sendto(payload, ("<broadcast>", port))
            if verbose:
                print("broadcast", payload, "\r", end="")

            time.sleep(twait)


def unicast_tx(
    host: str, port: int, twait: float = 0.1,
):

    L = 1024

    i = 0
    with socket.socket(socket.AF_INET6, socket.SOCK_DGRAM) as s:
        while True:
            payload = i.to_bytes(L, "big")
            s.sendto(payload, (host, port, 0, 0))

            # print("unicast payload:", payload, "\r", end="")
            # time.sleep(twait)
            i += 1
