import pytest
import subprocess
import threading
from pathlib import Path
import shutil
import python_c_sockets.tx as pcs

R = Path(__file__).resolve().parents[1]

exe = shutil.which("unicast_tx", path=str(R / "build"))


@pytest.fixture
def unicast_sender():
    if exe is None:
        pytest.skip("could not find unicast_tx")
    proc = subprocess.Popen([exe])
    yield proc
    proc.terminate()


@pytest.fixture
def ipv4bcast_sender():
    def _dummy(port: int, N: int):
        thread = threading.Thread(
            target=pcs.ipv4bcast_tx, kwargs={"port": port, "N": N, "twait": 0.02, "verbose": True}
        )
        thread.start()

    return _dummy
