#!/usr/bin/env python
"""
Send UDP broadcast packets
Only possible on IPv4, not recommended for actual use.
1024 bytes max.
"""
import python_c_sockets as pcs


def main():
    pcs.ipv4bcast(port=2000)


if __name__ == '__main__':
    main()
