#!/usr/bin/env bash
##########################################################################
#                                                                        #
#                              CloudComPy                                #
#                                                                        #
#  This program is free software; you can redistribute it and/or modify  #
#  it under the terms of the GNU General Public License as published by  #
#  the Free Software Foundation; either version 3 of the License, or     #
#  any later version.                                                    #
#                                                                        #
#  This program is distributed in the hope that it will be useful,       #
#  but WITHOUT ANY WARRANTY; without even the implied warranty of        #
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the          #
#  GNU General Public License for more details.                          #
#                                                                        #
#  You should have received a copy of the GNU General Public License     #
#  along with this program. If not, see <https://www.gnu.org/licenses/>. #
#                                                                        #
#          Copyright 2020-2025 Paul RASCLE www.openfields.fr             #
#                                                                        #
##########################################################################

# Sets or disables the CloudComPy environment.
# Should be sourced to activate or deactivate environment.

activate()
{
    if [ x${CLOUDCOMPY_ENV_ACTIVATED} != "x1" ]; then
        echo "Activating CloudComPy environment"
        export LD_LIBRARY_PATH_SAVED=${LD_LIBRARY_PATH}
        export PYTHONPATH_SAVED=${PYTHONPATH}
        export PATH_SAVED=${PATH}
        export PATH=${CLOUDCOMPY_ROOT}/bin:${CLOUDCOMPY_ROOT}/cloudComPy:${PATH}
        export PYTHONPATH=${CLOUDCOMPY_ROOT}:${CLOUDCOMPY_ROOT}/cloudComPy:${PYTHONPATH}
        export PYTHONPATH=${CLOUDCOMPY_ROOT}/cloudComPy/doc/PythonAPI_test:${PYTHONPATH}
        export LD_LIBRARY_PATH=${CLOUDCOMPY_ROOT}/cloudComPy:${CLOUDCOMPY_ROOT}/cloudComPy/plugins/CC:${LD_LIBRARY_PATH}
        export QT_PLUGIN_PATH=${CLOUDCOMPY_ROOT}/cloudComPy/plugins
        export QT_QPA_PLATFORM_PLUGIN_PATH=${CLOUDCOMPY_ROOT}/cloudComPy/plugins/platforms
        export CLOUDCOMPY_ENV_ACTIVATED=1
        export LC_NUMERIC=C
    fi
}

deactivate()
{
    if [ x${CLOUDCOMPY_ENV_ACTIVATED} == "x1" ]; then
        echo "Deactivating CloudComPy environment"
        export LD_LIBRARY_PATH=${LD_LIBRARY_PATH_SAVED}
        export LD_LIBRARY_PATH_SAVED=
        export PYTHONPATH=${PYTHONPATH_SAVED}
        export PYTHONPATH_SAVED=
        export PATH=${PATH_SAVED}
        export PATH_SAVED=
    fi
    export CLOUDCOMPY_ENV_ACTIVATED=0
}

usage()
{
    echo "Sets or disables the CloudComPy environment."
    echo "this script must be sourced (with source or '.')"
    echo "Usage:"
    echo "source ${SCRIPT_NAME} activate    ==> set CloudComPy paths"
    echo "source ${SCRIPT_NAME} deactivate  ==> reset CloudComPy paths"
    echo "source ${SCRIPT_NAME}             ==> show usage, status and paths"
    echo
    echo "CLOUDCOMPY_ROOT: ${CLOUDCOMPY_ROOT}"
    echo "CLOUDCOMPY_ENV_ACTIVATED: ${CLOUDCOMPY_ENV_ACTIVATED}"
    echo "LD_LIBRARY_PATH: ${LD_LIBRARY_PATH}"
    echo "PYTHONPATH: ${PYTHONPATH}"
    echo "LD_LIBRARY_PATH_SAVED: ${LD_LIBRARY_PATH_SAVED}"
    echo "PYTHONPATH_SAVED: ${PYTHONPATH_SAVED}"
    echo "PATH: ${PATH}"
    echo
}


SCRIPT_PATH=$(readlink -f "${BASH_SOURCE[0]}")
SCRIPT_NAME=$(basename "${SCRIPT_PATH}")
SCRIPT_DIR=$(dirname "${SCRIPT_PATH}")
CLOUDCOMPY_ROOT=$(realpath "${SCRIPT_DIR}/..")
PROG=$(basename $0)
# $PROG is the name of the shell (i.e. bash) when sourced, or the script name when executed

if [ "x$PROG" == "x$SCRIPT_NAME" ]; then
    echo "this script must be sourced (with source or '.')"
elif [ "x$1" == "xdeactivate" ]; then
    deactivate
elif [ "x$1" == "xactivate" ]; then
    activate
else
    usage
fi

export LD_LIBRARY_PATH
export LD_LIBRARY_PATH_SAVED
export PYTHONPATH
export PYTHONPATH_SAVED
export PATH
export PATH_SAVED
export CLOUDCOMPY_ENV_ACTIVATED

