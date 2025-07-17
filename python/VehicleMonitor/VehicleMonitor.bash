#!/bin/bash

# ---------------------------------------------------------------------------
# Make sure that python3 is installed, because this script will first create
# a virtual environment for it and then run the python example.
# ---------------------------------------------------------------------------

python3 -m venv _venv
. _venv/bin/activate
python VehicleMonitor.py

