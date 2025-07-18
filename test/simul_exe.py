#!/usr/bin/env python3

"""
run two multicast programs in parallel
"""

import subprocess
import argparse

p = argparse.ArgumentParser(description="Run multicast sender and receiver")
p.add_argument("producer_exe", help="Path to the multicast producer executable")
p.add_argument("consumer_exe", help="Path to the multicast consumer executable")
args = p.parse_args()

p1 = subprocess.Popen([args.producer_exe])

p2 = subprocess.Popen([args.consumer_exe], stdin=subprocess.PIPE, text=True)

try:
    if p1.poll() is not None:
        raise RuntimeError(
            f"Producer process {args.producer_exe} exited unexpectedly with code {p1.returncode}"
        )

    p1.wait(timeout=10)

    p2.wait(timeout=10)

    assert p1.returncode == 0, f"Producer exited with code {p1.returncode}"
    assert p2.returncode == 0, f"Consumer exited with code {p2.returncode}"
    print("Both processes completed successfully.")
except KeyboardInterrupt:
    print("Interrupted, terminating processes...")
    p1.terminate()
    p2.terminate()
    p1.wait()
    p2.wait()
except subprocess.CalledProcessError as e:
    print(f"Runtime error occurred: {e}")
    p1.terminate()
    p2.terminate()
    p1.wait()
    p2.wait()
