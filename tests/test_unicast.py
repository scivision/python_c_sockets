#!/usr/bin/env python
import pytest
from pathlib import Path
import subprocess
import os
import python_c_sockets as pcs

R = Path(__file__).resolve().parents[1]
CI = bool(os.environ['CI']) if 'CI' in os.environ else False


@pytest.fixture()
def sender():
    proc = subprocess.Popen(['./unicast_tx'], cwd=R / 'bin')
    yield proc
    proc.terminate()


@pytest.mark.usefixtures("sender")
@pytest.mark.skipif(CI, reason="Many CI's don't have audio hardware")
def test_listener():
    port = 2000
    addr = '::1'

    pcs.udpunicast(addr, port, N=5000)


if __name__ == '__main__':
    pytest.main(['-x', __file__])
