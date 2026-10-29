#!/usr/bin/env zsh
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
    echo "activate"
    if [[ x${CLOUDCOMPY_ENV_ACTIVATED} != "x1" ]]; then
        export PYTHONPATH_SAVED=${PYTHONPATH}
        export PYTHONPATH=${CLOUDCOMPY_ROOT}:${CLOUDCOMPY_ROOT}/cloudComPy:${CLOUDCOMPY_ROOT}/cloudComPy/CloudCompare/CloudCompare.app/Contents/Frameworks:${PYTHONPATH}
        export PYTHONPATH=${CLOUDCOMPY_ROOT}/cloudComPy/doc/PythonAPI_test:${PYTHONPATH}
        export CLOUDCOMPY_ENV_ACTIVATED=1
        export LC_NUMERIC=C
    fi
}

deactivate()
{
    echo "deactivate"
    if [[ x${CLOUDCOMPY_ENV_ACTIVATED} == "x1" ]]; then
        export PYTHONPATH=${PYTHONPATH_SAVED}
        export PYTHONPATH_SAVED=
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
    echo "PYTHONPATH: ${PYTHONPATH}"
    echo "PYTHONPATH_SAVED: ${PYTHONPATH_SAVED}"
    echo
}

#echo "argument: $1 "
val="$1"
echo "val: $val"
echo "ZSH_EVAL_CONTEXT: ${ZSH_EVAL_CONTEXT:-}"
CLOUDCOMPY_ROOT=$(realpath "${0:a}/../..")
echo "CLOUDCOMPY_ROOT: $CLOUDCOMPY_ROOT"
if [[ "${ZSH_EVAL_CONTEXT:-}" != *:file ]]; then
    echo "this script must be sourced (with source or '.')"
elif [[ "$val" == "deactivate" ]]; then
    deactivate
elif [[ "$val" == "activate" ]]; then
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

