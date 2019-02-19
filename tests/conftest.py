import pytest
import subprocess
import threading
from pathlib import Path
import python_c_sockets as pcs

R = Path(__file__).resolve().parents[1]


@pytest.fixture
def unicast_sender():
    proc = subprocess.Popen(['./unicast_tx'], cwd=R/'build')
    yield proc
    proc.terminate()


@pytest.fixture
def ipv4bcast_sender():

    def _dummy(port: int, N: int):
        thread = threading.Thread(target=pcs.ipv4bcast_tx,
                                  kwargs={'port': port, 'N': N,
                                          'twait': 0.02, 'verbose': True})
        thread.start()

    return _dummy
