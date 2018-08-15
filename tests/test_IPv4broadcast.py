#!/usr/bin/env python
import pytest
import threading
import python_c_sockets as pcs

PORT = 2000
N = 100


@pytest.fixture()
def ipv4bcast_sender():
    thread = threading.Thread(target=pcs.ipv4bcast_tx,
                              kwargs={'port': PORT, 'N': N,
                                      'twait': 0.02, 'verbose': True})
    thread.start()


@pytest.mark.usefixtures("ipv4bcast_sender")
def test_listener():
    pcs.ipv4bcast_rx(PORT, N=N)


if __name__ == '__main__':
    pytest.main(['-x', __file__])
