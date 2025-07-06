#!/bin/bash

# --------------------------------------------------------------------------------------
# This script will generate all the necessary sources for an OpenDDS implementation
# of the defined IDL's
# --------------------------------------------------------------------------------------
GENERATEDDIR=generated
IDLFILES=( Vehicle )

UNKNOWN_ANNOTATIONS='--unknown-annotations ignore -as'
IDL_VERSION='--idl-version 4'

function Generate
{
    IDLFILE=$1
    cd ${IDLFILE}

    echo "Remove ${GENERATEDDIR} ..."
    rm -rf ${GENERATEDDIR}

    echo "Processing IDL ${IDLFILE} ..."

    opendds_idl -o ${GENERATEDDIR} -I${DDS_ROOT} ${IDLFILE}.idl

    tao_idl ${IDL_VERSION} ${UNKNOWN_ANNOTATIONS} -o ${GENERATEDDIR} -I${DDS_ROOT} ${IDLFILE}.idl
    tao_idl ${IDL_VERSION} ${UNKNOWN_ANNOTATIONS} -o ${GENERATEDDIR} -I${DDS_ROOT} ${GENERATEDDIR}/${IDLFILE}TypeSupport.idl

    cd ..
}


# --------------------------------------------------------------------------------------
# Main
# --------------------------------------------------------------------------------------
if (( $# == 0 ));
then
    for IDLFILE in ${IDLFILES[@]}; do
        Generate ${IDLFILE}
    done
elif (( $# == 1 ));
then
    for IDLFILE in ${IDLFILES[@]}; do
        if  [ "$IDLFILE" = "$1" ];
        then
            echo "Generate single IDL: $1"
            Generate ${IDLFILE}
        fi
    done
fi
