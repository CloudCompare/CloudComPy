#!/bin/zsh
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
    echo "function activate $1"
    conda activate "$1"
    ret=$?
    echo $ret
    if [[ $ret == "0" ]] && [[ x${CONDACLOUD_ACTIVATED} != "x1" ]]; then
        echo "setenv"
        CONDA_ENV_ROOT=$(realpath "$(dirname $(which python))/..")
        export PYTHONPATH_SAVED=${PYTHONPATH}
        export PATH_SAVED=${PATH}
        export PATH=${CLOUDCOMPY_ROOT}/bin:${PATH}
        export PYTHONPATH=${CLOUDCOMPY_ROOT}:${PYTHONPATH}
        export PYTHONPATH=${CLOUDCOMPY_ROOT}/CloudCompare/CloudCompare.app/Contents/Frameworks:${PYTHONPATH}
        export PYTHONPATH=${CLOUDCOMPY_ROOT}/doc/PythonAPI_test:${PYTHONPATH}
        export CONDACLOUD_ACTIVATED=1
        export LC_NUMERIC=C
    fi
}

deactivate()
{
    echo "function deactivate"
    CONDA_ENV_ROOT=$(realpath "$(dirname $(which python))/..")
    conda deactivate
    if [[ x${CONDACLOUD_ACTIVATED} == "x1" ]]; then
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

sourced=0
case $ZSH_EVAL_CONTEXT in *:file) sourced=1;; esac
#[[ $ZSH_EVAL_CONTEXT =~ :file$) ]] && sourced=1 || sourced=0
echo "sourced="${sourced}
if [[ ${sourced} == 0 ]]; then
    echo "this script must be sourced (with source or '.')"
else
    SCRIPT_PATH=${0:a}
    #echo ${SCRIPT_PATH}
    SCRIPT_NAME=$(basename "${SCRIPT_PATH}")
    #echo ${SCRIPT_NAME}
    CLOUDCOMPY_ROOT=${0:a:h:h}
    #echo ${CLOUDCOMPY_ROOT}
    PROG=$(basename $0)
    #echo ${PROG}
    CONDA_ROOT=$(conda info | grep "envs directories"|awk '{print $NF}')
    . ${CONDA_ROOT}/../etc/profile.d/conda.sh                                       # required to have access to conda commands in a shell script
    
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

export PYTHONPATH
export PYTHONPATH_SAVED
export PATH
export PATH_SAVED
export CONDACLOUD_ACTIVATED
