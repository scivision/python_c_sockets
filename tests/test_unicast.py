#!/usr/bin/env python
import pytest
from pathlib import Path
import subprocess
import python_c_sockets as pcs

R = Path(__file__).resolve().parents[1]


@pytest.fixture()
def sender():
    proc = subprocess.Popen(['./unicast_tx'], cwd=R / 'bin')
    yield proc
    proc.terminate()


@pytest.mark.usefixtures("sender")
def test_listener():
    port = 2000
    addr = '::1'

    pcs.udpunicast(addr, port, N=5000)


if __name__ == '__main__':
    pytest.main()
