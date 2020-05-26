#!/usr/bin/env python
import pytest
import os
import python_c_sockets.rx as pcs

CI = bool(os.environ.get("CI"))


@pytest.mark.usefixtures("unicast_sender")
@pytest.mark.skipif(CI, reason="Many CI's don't have IPv6")
@pytest.mark.skipif(os.name == "nt", reason="C code not designed for Windows")
def test_listener():
    port = 2000
    addr = "::1"

    pcs.udpunicast(addr, port, N=1000)


if __name__ == "__main__":
    pytest.main([__file__])
