#!/bin/bash

# -------------------------------------------------------------------------------------------------
# For discovery purposes of DCPS:
#
# --DCPSInfoRepo       : This is necessary for discovery
# --ORBDebugLevel      : <optional>
# --ORBLogFile         : <optional>
#
# The following parameters that start with -- are extra application parameters:
#
# `DDSCOMMON::PARAMETERS::DDSCONFIG`  : The Configuration parameters file
# `DDSCOMMON::PARAMETERS::MONITOR`    : Do monitoring of statuses: true or false
# `DDSCOMMON::PARAMETERS::NAME`       : Name of the Vehicle component, can be given as param $1 in bash
# $1 : --name=NAME
# $2 : --domain=<NUMBER>              : Number between 100 and 200
# $3 : --frequency=<VALUE>            : A positive non zero float value
# -------------------------------------------------------------------------------------------------
./TestVehicle \
  --DCPSInfoRepo=127.0.0.1:12345 \
  --ORBDebugLevel=10 \
  --ORBLogFile=TestVehicle.log \
  --ddsconfig=./VehicleDDS.xml \
  --monitor=false \
  $1 $2 $3
