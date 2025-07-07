#!/bin/bash

rm .pid

./DCPSInfoRepo.bash  &
echo $! >> .pid

./TestVehicle.bash --name=A  &
echo $! >> .pid

./TestVehicle.bash --name=B  &
echo $! >> .pid

./TestVehicle.bash --name=C  &
echo $! >> .pid


./VehicleMonitor.bash &
echo $! >> .pid
