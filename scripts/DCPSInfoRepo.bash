#!/bin/bash

# -------------------------------------------------------------------------------------------------
# Only Listen to localhost: For test purposes
# DCPSINFOSERVER=127.0.0.1
# Listen to all interfaces: Production
DCPSINFOSERVER=127.0.0.1
DCPSINFOPORT=12345

if [[ "$1" != "" ]]
then
  DCPSINFOPORT=$1
fi

echo "Server:${DCPSINFOSERVER}"
echo "Port  :${DCPSINFOPORT}"

# -------------------------------------------------------------------------------------------------
# Start DCPS Info repository (Discovery system)
# -------------------------------------------------------------------------------------------------
# Any additional parameters can be used:
#
# -ORBListenEndpoints : This is necessary for discovery by publisher and subscriber
# -ORBDebugLevel      : <optional>
# -ORBLogFile         : <optional>
# $1                  : DCPSINFOPORT
# -------------------------------------------------------------------------------------------------
APP=`basename $0 .bash`

${DDS_ROOT}/bin/${APP} \
   -ORBListenEndpoints iiop://${DCPSINFOSERVER}:${DCPSINFOPORT}   \
    -ORBDebugLevel 10                                             \
    -ORBLogFile ${APP}.log
