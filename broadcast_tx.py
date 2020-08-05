#!/usr/bin/env python3
"""
Send UDP broadcast packets
Only possible on IPv4, not recommended for actual use.
1024 bytes max.
"""
import python_c_sockets.tx as pcs

pcs.ipv4bcast_tx(port=2000)
