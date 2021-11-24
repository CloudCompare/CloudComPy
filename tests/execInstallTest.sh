#!/bin/bash

usage()
{
    echo "usage: $0 test<xxx>.py"
}

if [ "$1" == "" ]; then
    usage
    exit 1
fi

SCRIPT_PATH=$(readlink -f "${BASH_SOURCE[0]}")
SCRIPT_DIR=$(dirname "${SCRIPT_PATH}")
CLOUDCOMPY_ROOT=$(realpath "${SCRIPT_DIR}/../..")
export PYTHONPATH=${CLOUDCOMPY_ROOT}/lib/cloudcompare:${PYTHONPATH}
export PYTHONPATH=${CLOUDCOMPY_ROOT}/doc/PythonAPI_test:${PYTHONPATH}
export LD_LIBRARY_PATH=${CLOUDCOMPY_ROOT}/lib/cloudcompare/plugins:${LD_LIBRARY_PATH}
python3 $1
