import pytest
import subprocess
import threading
from pathlib import Path
import shutil
from datetime import datetime

from broadcast_tx import ipv4bcast_tx

R = Path(__file__).resolve().parent

exe = shutil.which("udp_tx", path=R / "build")


@pytest.fixture
def unicast_sender():
    if exe is None:
        pytest.skip("could not find unicast_tx")
    proc = subprocess.Popen([exe])
    print(
        f"started unicast_tx with PID {proc.pid}  {datetime.now().strftime('%Y-%m-%d %H:%M:%S.%f')}"
    )
    yield proc
    print(
        f"terminating unicast_tx with PID {proc.pid}  {datetime.now().strftime('%Y-%m-%d %H:%M:%S.%f')}"
    )
    proc.terminate()


@pytest.fixture
def ipv4bcast_sender():
    def _dummy(port: int, N: int):
        thread = threading.Thread(
            target=ipv4bcast_tx, kwargs={"port": port, "N": N, "twait": 0.02, "verbose": True}
        )
        thread.start()

    return _dummy
