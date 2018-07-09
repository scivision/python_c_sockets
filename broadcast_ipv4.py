#!/usr/bin/env python
"""
 Send UDP broadcast packets
 Only possible on IPv4, not recommended for actual use.
 1024 bytes max.
"""
import time
import socket
#
MYPORT = 50000


def main():

    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.bind(('', 0))
    s.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)

    for i in range(10000):
        payload = i.to_bytes(14, 'big')
        s.sendto(payload, ('<broadcast>', MYPORT))
        print('broadcast', payload, '\r', end="")
        time.sleep(1)


if __name__ == '__main__':
    main()
