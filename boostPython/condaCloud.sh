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
#          Copyright 2020-2021 Paul RASCLE www.openfields.fr             #
#                                                                        #
##########################################################################

# Sets or disables the CloudComPy environment. Overrides conda activation/deactivation commands.
# Should be sourced to activate or deactivate environment.

activate()
{
    #echo "function activate $1"
    conda activate "$1"
    ret=$?
    if [ $ret == "0" ] && [ x${CONDACLOUD_ACTIVATED} != "x1" ]; then
        #echo "setenv"
        CONDA_ENV_ROOT=$(realpath "$(dirname $(which python))/..")
        export LD_LIBRARY_PATH_SAVED=${LD_LIBRARY_PATH}
        export PYTHONPATH_SAVED=${PYTHONPATH}
        export PATH_SAVED=${PATH}
        export LD_LIBRARY_PATH=${CONDA_ENV_ROOT}/lib:${LD_LIBRARY_PATH}
        export PATH=${CLOUDCOMPY_ROOT}/bin:${PATH}
        export PYTHONPATH=${CLOUDCOMPY_ROOT}/lib/cloudcompare:${PYTHONPATH}
        export PYTHONPATH=${CLOUDCOMPY_ROOT}/doc/PythonAPI_test:${PYTHONPATH}
        export LD_LIBRARY_PATH=${CLOUDCOMPY_ROOT}/lib/cloudcompare:${CLOUDCOMPY_ROOT}/lib/cloudcompare/plugins:${LD_LIBRARY_PATH}
        export CONDACLOUD_ACTIVATED=1
        export LC_NUMERIC=C
    fi
}

deactivate()
{
    #echo "function deactivate"
    CONDA_ENV_ROOT=$(realpath "$(dirname $(which python))/..")
    conda deactivate
    if [ x${CONDACLOUD_ACTIVATED} == "x1" ]; then
        export LD_LIBRARY_PATH=${LD_LIBRARY_PATH_SAVED}
        export LD_LIBRARY_PATH_SAVED=
        export PYTHONPATH=${PYTHONPATH_SAVED}
        export PYTHONPATH_SAVED=
        export PATH=${PATH_SAVED}
        export PATH_SAVED=
    fi
    export CONDACLOUD_ACTIVATED=0
}

usage()
{
    echo
    echo "Sets or disables the CloudComPy environment. Overrides conda activation/deactivation commands."
    echo "this script must be sourced (with source or '.')"
    echo "Usage:"
    echo "source ${SCRIPT_NAME} activate my_conda_env ==> conda activate my_conda_env and set CloudComPy paths"
    echo "source ${SCRIPT_NAME} deactivate            ==> deactivate conda, reset CloudComPy paths"
    echo "source ${SCRIPT_NAME}                       ==> show usage, status and paths"
    echo
    echo "CLOUDCOMPY_ROOT: ${CLOUDCOMPY_ROOT}"
    echo "CONDACLOUD_ACTIVATED: ${CONDACLOUD_ACTIVATED}"
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
CONDA_ROOT=$(realpath "$(dirname $(which conda))/..")
. ${CONDA_ROOT}/etc/profile.d/conda.sh                                       # required to have access to conda commands in a shell script

if [ $PROG == $SCRIPT_NAME ]; then
    echo "this script must be sourced (with source or '.')"
else
    case $# in
        1 ) if [ "$1" = "deactivate" ]; then
                deactivate
            else
                usage
            fi
            ;;
        2 ) if [ "$1" = "activate" ]; then
                activate $2
            else
                usage
            fi
            ;;
        * ) usage
    esac
fi

export LD_LIBRARY_PATH
export LD_LIBRARY_PATH_SAVED
export PYTHONPATH
export PYTHONPATH_SAVED
export PATH
export PATH_SAVED
export CONDACLOUD_ACTIVATED
