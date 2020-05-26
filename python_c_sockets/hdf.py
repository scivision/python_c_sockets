import socket
import time
import numpy as np
import struct
from pathlib import Path

try:
    import h5py
except ImportError:
    h5py = None

from . import NPKT


def udpunicast(
    host: str,
    port: int,
    h5fn: Path = None,
    Nupdate: int = 1000,
    bufsize: int = 8192,
    Nelbyte: int = 4,
    N: int = NPKT,
):
    """
    maxshape parameters:
    limit ~ 100000*BUFSIZE to overfill hardrive with extremely fast demo
    normally you would take other more sensible measures, and have sane datarates.
    None means unlimited size (entire hard drive)
    """

    S = socket.socket(socket.AF_INET6, socket.SOCK_DGRAM)
    # Do NOT connect or bind
    try:
        first = True

        if h5py is not None and h5fn:
            h5 = h5py.File(h5fn, "w")
            h5d = h5.create_dataset(
                "/data",
                dtype=np.float32,
                chunks=True,
                shape=(Nupdate * bufsize / Nelbyte,),
                maxshape=(100000 * bufsize / Nelbyte,),
            )
        else:
            h5 = h5d = None

        # %% main loop
        for i in range(N):
            tic = time.time()
            # %% host-> device
            # host (other program) is programmed to send payload for any input--including simply '\n'
            S.sendto(b"\n", (host, port, 0, 0))
            # %% device -> host
            print("recieving")
            Nel = struct.unpack("<1L", S.recv(4))[0]  # int len
            print(Nel)
            Nbyte_dg = Nel * Nelbyte

            A = S.recv(Nbyte_dg)

            if len(A) != Nbyte_dg:
                print("unicast_rx: could not determine length")
                continue
            print(len(A))
            # %% parse result
            dat = struct.unpack(f"<{Nel:d}f", A)
            if first:  # to avoid having to restart C code each time
                first = False
                last = dat[0] - 1
                rtoc = time.time() - tic

            rtoc = np.mean((time.time() - tic, rtoc))
            # %% optional write to disk
            if h5 is not None:
                h5d[i * Nel : (i + 1) * Nel] = dat
            # %% progress report / writing
            if not i % Nupdate:
                print(rtoc, i)
                h5 = _updatemsg(i, h5, h5d, Nupdate, bufsize, Nelbyte)
            # %% data validation due to a priori pattern

            # for single precision float, large integer jumps are experienced at large values by IEEE754 definition
            # assert_allclose(diff(dat),1,err_msg=str(dat))

            np.testing.assert_allclose(
                last, dat[0] - 1, err_msg=f"{last} be mindful of min/max values of your datatype"
            )

            last = dat[-1]
    except KeyboardInterrupt:
        pass

    S.close()
    if h5 is not None:
        h5.close()


def _updatemsg(i: int, h5, h5d, Nupdate: int, bufsize: int, Nelbyte: int):
    if h5 is not None:
        h5.flush()  # NOTE: every N loops to improve performance
        try:
            h5d.resize((i + Nupdate) * bufsize / Nelbyte, axis=0)
        except ValueError as e:
            print(
                "stopping HDF5 writing for this fast demo. in real life you can set maxshape=(None,) for no limit.",
                e,
            )
            h5.close()
            h5 = None

    return h5
