# How to

## Pre-requisites

### Create a virtual environment to run the code with python version 3.12:

- Open a terminal and go to your `${HOME}`

> cd

- Create a python virtual environment

> `python3 -m venv venv_3.12`
> `source venv_3.12/bin/activate`

If python v3.12 is not installed, then install this first (ask chatgpt or google etc...)

### Download/Build the (c++) library

You will need the c++ API library for communication between vehicles

- Download `libVehicle.so` shared library, or
- Build the library that will be used from the repo: <TODO>

## Run the example

- go to the location where you read this info...

> `cd opendds/python/VehicleMonitor`
> `python VehicleMonitor.py`
