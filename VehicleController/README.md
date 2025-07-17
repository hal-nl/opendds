# OpenDDS examples

Examples of OpenDDS integrations with other languages such as python.

They are not intended as release versions, but show you how you can start setting up
a distributed system of interacting objects (be it vehicles or otherwise).

Also showing how to use the notify system, that handles incoming commands in a multithreaded way,
and sends control commands to these objects. The real control is not implemented (yet),
but you can do this by setting up a new model with .idl files and implement
the control parts in OpenDDS.

The examples are tested on Ubuntu 24.04.

## Install OpenDDS

1. Download the latest version (3.32.0) and set `OPENDDS_VERSION=3.32.0`
2. I usually download and unzip it in `${HOME}/OpenDDS-${OPENDDS_VERSION}`, so I can keep several OpenDDS versions and switch by setting the ${OPENDDS_ROOT}
   to `${HOME}/OpenDDS-${VERSION}`
3. Run `./configure  --rapidjson --std=c++17` for
   1. using  rapidjson and at least c++17 because I use the fmt library on Linux
   2. you can also try `--dry-run` to see if everything will work as expected
4. If you already have an OpenDDS environment, make sure to disable them before running `./configure`
   1. Reset `ACE_ROOT`, `TAO_ROOT`, `MPC_ROOT`, `CIAO_ROOT` and `DANCE_ROOT` that OpenDDS has generated via `setenv.sh` in the current environment
5. Run `./configure --rapidjson --std=c++17`
6. It will generate a `setenv.sh` with the proper environment vars for OpenDDS
7. Run `setenv.sh`
8. Run `make`, this might take take some time depending on your PC/laptop
9. Adjust your `.bashrc` to contain the environment variables in the `setenv.sh`
10. Add the following to your `.bashrc`, so that each time you open a terminal the OpenDDS settings will point to the version you are using

```bash
export DDS_VERSION=3.32.0
export DDS_ROOT=/${HOME}/OpenDDS-${DDS_VERSION}
export ACE_ROOT=${DDS_ROOT}/ACE_wrappers
export TAO_ROOT=${ACE_ROOT}/TAO
export MPC_ROOT=${ACE_ROOT}/MPC
export CIAO_ROOT=unused
export DANCE_ROOT=unused

export PATH=${PATH}:${DDS_ROOT}/bin:${ACE_ROOT}/bin
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${ACE_ROOT}/lib:${DDS_ROOT}/lib:./

export RAPIDJSON_ROOT=${DDS_ROOT}/tools/rapidjson
```

## Install/clone the examples

1. Download this repo: extract from zip or clone this repo.
2. In a terminal go to `opendds/`
3. Create a directory `build` and go into it: `mkdir build && cd build`
4. Run `cmake -DCMAKE_BUILD_TYPE=debug ..`
5. Go to `opendds/_idls`
6. Run `./create_idl.bash` to generate the vehicle type support sources.
7. Go to `opendds/build` and run `make`. This will build the examples.

## Scripts and binaries

1. Go to `opendds/build/bin`
2. `libDDSCommon.so`    : a shared object with helper functionality
3. `libDDSQoS.so`       : a shared object with helper functionality to use a configuration file for QoS
4. `libVehicle.so`      : is the OpenDDS wrapper used in `VehicleMonitor.py`
5. `TestVehicle`        : start a single vehicle (is used in `start_all.bash`)
6. `VehicleMonitor.py`  : a simple python (v3.12) demo application showing 3 vehicles and using OpenDDS
7. `VehicleController`  : a multithreaded service in c++ to control the vehicles by command files or pub/sub messaging via OpenDDS
8. There are two test scripts and :

### Running the Monitor (python)

1. `start_all.bash` to start 3 vehicles and monitor this via a python application.
2. `stop_all.bash` to stop them all and stop monitoring.

### Running the Controller (c++)

1. Go to `opendds/build/bin`
2. Start controller: `./VehicleController`
3. Drag and drop a control file (`VehicleController.pause.json`) in the `opendds/build/bin/data/input` directory
   This will show up in the output of the (the real control is not implemented).

### Using from pascal (Delphi)

TODO:
