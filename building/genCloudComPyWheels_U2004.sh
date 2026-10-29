#!/bin/bash

action=$1
ifirst=$2
ilast=$3
pyversions=("3.9"    "3.10"    "3.11"    "3.12"    "3.13"    "3.14"  )
pyvers=(    "39"     "310"     "311"     "312"     "313"     "314"   )
pyminvers=( "9"      "10"      "11"      "12"      "13"      "14"    )
pyfullvers=("3.9.25" "3.10.20" "3.11.15" "3.12.13" "3.13.14" "3.14.6")
export genbase="${HOME}/projets/CloudComPy"
export CONDA_ROOT=${HOME}/miniconda3                                    # root directory of conda installation
export REPO_DIR=${genbase}/CloudComPy
export CLOUDCOMPY_VERSION="2.13.90"

genWheel() {

    #conda activate ${CONDA_ENV}
    export pyindex=$1
    export PYPATH="${genbase}/install/${pyfullvers[$pyindex]}"
    export PYTHON_VERSION=${pyversions[$pyindex]}
    export PYTVER=${pyvers[$pyindex]}
    export PYMINOR=${pyminvers[$pyindex]}
    export VENV_PYTHON="${genbase}/.venv${PYTVER}"

    export CONDA_ENV=CloudComPy3${PYMINOR}                              # conda environment name
    export CONDA_PATH=${CONDA_ROOT}/envs/${CONDA_ENV}                   # conda environment directory
    #export QT_PREFIX=${CONDA_PATH}/lib/qt6                              # prefix for qt (if qt plugins are needed, otherwise set to empty or remove from cmake options)
    export Qt6_DIR=${CONDA_PATH}/lib/cmake/Qt6                          # path to Qt6Config.cmake (if qt plugins are needed, otherwise set to empty or remove from cmake options)

    # creation environnement virtuel Python

    # -------------------------------------

    cd "${genbase}"
    rm -rf ${VENV_PYTHON}
    ${PYPATH}/bin/python${PYTHON_VERSION} -m venv ${VENV_PYTHON}

    # activation environnement virtuel Python
    # ---------------------------------------

    cd "${genbase}"
    source ${VENV_PYTHON}/bin/activate
    pip install --upgrade pip
    pip install numpy scipy requests psutil matplotlib numpy-quaternion pybind11 sphinx-rtd-theme patchelf
    
    export Boost_DIR=${CONDA_PATH}/lib/cmake/Boost-1.88.0
    export CGAL_DIR=${CONDA_PATH}/lib/cmake/CGAL
    export CMAKE_C_COMPILER=${CONDA_PATH}/bin/gcc
    export CMAKE_CXX_COMPILER=${CONDA_PATH}/bin/g++
    export CMAKE_PREFIX_PATH=${CONDA_PATH}
    export cork_rep=${genbase}/cork
    export CORK_INCLUDE_DIR=${cork_rep}/src
    export CORK_RELEASE_LIBRARY_FILE=${cork_rep}/lib/libcork.a
    export draco_rep=${CONDA_PATH}
    #export DRACO_INCLUDE_DIR=${draco_rep}/include
    export DRACO_LIBRARIES=${draco_rep}/lib/libdraco.a
    #export DRACO_LIB_DIR=${draco_rep}/lib
    #export EIGEN_INCLUDE_DIR=${CONDA_PATH}/include/eigen3
    export EIGEN_ROOT_DIR=${CONDA_PATH}/include/eigen3
    export fbxsdk_rep=${genbase}/fbxSdk
    export FBX_SDK_INCLUDE_DIR="${fbxsdk_rep}/include"
    export FBX_SDK_LIBRARY_FILE="${fbxsdk_rep}/lib/release/libfbxsdk.a"
    export FBX_XML2_LIBRARY_FILE=""
    export GDAL_INCLUDE_DIR="${CONDA_PATH}/include"
    export GDAL_LIBRARY="${CONDA_PATH}/lib/libgdal.so"
    export GMP_INCLUDE_DIR="${CONDA_PATH}/include"
    export GMP_LIBRARIES="${CONDA_PATH}/lib/libgmp.so"
    #export GMP_LIBRARIES_DIR="${CONDA_PATH}"
    export libigl_rep=${genbase}/libigl
    export LIBIGL_INCLUDE_DIR=${libigl_rep}/libigl/include
    export LIBIGL_RELEASE_LIBRARY_FILE=${libigl_rep}/install/lib/libigl.a
    export MPFR_INCLUDE_DIR="${CONDA_PATH}/include"
    export MPFR_LIBRARIES="${CONDA_PATH}/lib/libmpfr.so"
    #export MPFR_LIBRARIES_DIR="${CONDA_PATH}"
    export MPIR_INCLUDE_DIR="${CONDA_PATH}/include"
    export MPIR_RELEASE_LIBRARY_FILE="${CONDA_PATH}/lib/libgmp.so"
    export opencascade_rep=${CONDA_PATH}
    #export OPENCASCADE_DLL_DIR="${opencascade_rep}/lib"
    export OPENCASCADE_INC_DIR="${opencascade_rep}/include/opencascade"
    export OPENCASCADE_LIB_DIR="${opencascade_rep}/lib"
    export OPENCASCADE_TBB_DLL_DIR="${opencascade_rep}/lib"
    #export OpenMP_omp_LIBRARY="${CONDA_PATH}/lib/libomp.so"
    #export PYTHONAPI_TEST_DIRECTORY="CloudComPy/Data"
    #export PYTHONAPI_EXTDATA_DIRECTORY="CloudComPy/ExternalData"
    export PYMINOR=${PYMINOR}
    export PYTHON_PREFERED_VERSION=${PYTHON_VERSION}
    export PYTHONVENV_DIR=${VENV_PYTHON}
    export pybind11_DIR=${VENV_PYTHON}/lib/python${PYTHON_VERSION}/site-packages/pybind11/share/cmake/pybind11
    export TBB_DIR="${CONDA_PATH}/lib/cmake/TBB"
    export XercesC_INCLUDE_DIR=${CONDA_PATH}/include
    export XercesC_LIBRARY_DEBUG=${CONDA_PATH}/lib/libxerces-c.so
    export XercesC_LIBRARY_RELEASE=${CONDA_PATH}/lib/libxerces-c-3.3.so
    export XercesC_VERSION=3.3
    export ZLIB_INCLUDE_DIR="${CONDA_PATH}/include"
    export ZLIB_LIBRARY_RELEASE="${CONDA_PATH}/lib/libz.so"

    # construction SDist et wheel (dans l'environnement virtuel)
    # ----------------------------------------------------------

    cd "${HOME}/projets/CloudComPy/CloudComPy"
    cp -f Linux/pyproject.toml pyproject.toml
    pip install build scikit_build_core
    export SKBUILD_WHEEL_TAG=$(python -m scikit_build_core.builder.wheel_tag)
    python -m build
    #conda deactivate
}

repairWheel() {
    # construction du wheel manylinux (toujours dans l'environnement virtuel)
    # -----------------------------------------------------------------------
    #conda activate ${CONDA_ENV}
    export pyindex=$1
    export PYTVER=${pyvers[$pyindex]}
    export VENV_PYTHON="${genbase}/.venv${PYTVER}"

    cd ${genbase}
    source ${VENV_PYTHON}/bin/activate
    cd "${HOME}/projets/CloudComPy"
    pip install auditwheel
    auditwheel repair CloudComPy/dist/cloudcompy-${CLOUDCOMPY_VERSION}-cp${PYTVER}-cp${PYTVER}-linux_x86_64.whl \
    --disable-isa-ext-check \
    --exclude 'libX*' \
    --exclude 'libxcb*' \
    --exclude 'libwayland*' \
    --exclude 'libEGL*' \
    --exclude 'libGL.so*' \
    --exclude 'libGLdispatch.so*' \
    --exclude 'libGLU.so*' \
    --exclude 'libGLX.so*' \
    --exclude 'libGLES.so*'
    #conda deactivate
}

#. ${CONDA_ROOT}/etc/profile.d/conda.sh                                                 # required to have access to conda commands in a shell script
i=$ifirst
while [ $i -le $ilast ]; do
  echo "********************************************************************************"
  echo "*** $i ${pyvers[$i]} python${pyversions[$i]} "
  echo "********************************************************************************"
  if [ "$action" = "gen" ] ; then
    genWheel $i
  fi
  if [ "$action" = "repair" ] ; then
    repairWheel $i
  fi
  ((i++))
done
