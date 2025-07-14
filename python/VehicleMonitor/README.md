# OpenDDS examples

Examples of opendds integrations with other languages such as python, etc

## Python examples

### VehicleMonitor.py

This example uses the pub-sub pattern with OpenDDS for communications between several
vehicles (generally speaking between objects, but for demo purposes named to vehicles).

The python script uses a shared library: `libVehicle.so`.

This library `libVehicle.so` contains the interface to send and receive messages for communications via OpenDDS.
