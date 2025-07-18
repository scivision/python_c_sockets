"""
this test requires network connection (not just localhost)
"""

import python_c_sockets.rx as pcs

PORT = 2000
N = 100


def test_listener(ipv4bcast_sender):
    ipv4bcast_sender(PORT, N)
    pcs.ipv4bcast_rx(PORT, N=N)
