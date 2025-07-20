import pytest
import os
import time

from unicast_rx import udpunicast

CI = bool(os.environ.get("CI"))


@pytest.mark.usefixtures("unicast_sender")
@pytest.mark.skipif(CI, reason="Many CI's don't have IPv6")
def test_listener():

    time.sleep(0.1)
    # need delay to allow the sender to start

    udpunicast(host="::1", port=2001, N=5)
