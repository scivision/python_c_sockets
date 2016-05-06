#!/usr/bin/env python3
"""
 Send UDP broadcast packets
 Only possible on IPv4, not recommended for actual use.
 1024 bytes max.
"""

MYPORT = 50000
import time
from socket import *

s = socket(AF_INET, SOCK_DGRAM)
s.bind(('', 0))
s.setsockopt(SOL_SOCKET, SO_BROADCAST, 1)

for i in range(1000000):
    s.sendto(bytes(i), ('<broadcast>', MYPORT))
    time.sleep(1)
