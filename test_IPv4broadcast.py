"""
this test requires network connection (not just localhost)
"""

from broadcast_rx import ipv4bcast_rx

PORT = 2000
N = 100


def test_listener(ipv4bcast_sender):
    ipv4bcast_sender(PORT, N)
    ipv4bcast_rx(PORT, N=N, timeout=30.0)
