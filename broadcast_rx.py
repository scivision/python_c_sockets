#!/usr/bin/env python3
"""
Receive UDP broadcast packets
Only possible on IPv4, not recommended for actual use.
1024 bytes max.
"""
import python_c_sockets.rx as pcs

pcs.ipv4bcast_rx(port=2000)
