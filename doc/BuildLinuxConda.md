
## Build on Linux64, with Anaconda3 or miniconda3

The script below executes the following:

 - create or update the conda environment
 - activate the environment
 - build and install CloudComPy (Sources must be cloned from Github and up to date)
 - create the tarfile
 - execute ctest

```
export CLOUDCOMPY_SRC=${HOME}/projets/CloudComPy/CloudComPy                       # CloudComPy source directory
export CLOUDCOMPY_BUILD=${HOME}/projets/CloudComPy/buildConda39                   # CloudComPy build directory
export CLOUDCOMPY_INSTDIR=${HOME}/projets/CloudComPy/installConda                 # directory for CloudComPy installs
export CLOUDCOMPY_INSTNAME=CloudComPy39                                           # CloudComPy install directory name
export CLOUDCOMPY_INSTALL=${CLOUDCOMPY_INSTDIR}/${CLOUDCOMPY_INSTNAME}            # CloudComPy install directory
export CLOUDCOMPY_TARFILE=CloudComPy_Conda39_Linux64_"$(date +"%Y%m%d-%H%M")".tgz # CloudComPy Binary tarfile (will be in ${CLOUDCOMPY_INSTDIR}
export CONDA_ROOT=${HOME}/anaconda3                                               # root directory of conda installation
export CONDA_ENV=CloudComPy39                                                     # conda environment name
export CONDA_PATH=${CONDA_ROOT}/envs/${CONDA_ENV}                                 # conda environment directory
export FBXSDK_REP=${HOME}/projets/CloudComPy/fbxSdk                               # directory of fbx sdk (remove the plugin in cmake options if not needed)
export NBTHREADS="$(grep -c processor /proc/cpuinfo)"                             # number of threads for parallel make

source ${CONDA_ROOT}/etc/profile.d/conda.sh                                       # required to have access to conda commands in a shell script

error_exit()
{
  echo "Error $1" 1>&2
  exit 1
}

# --- conda environment

conda_buildenv()
{
    echo "# --- build conda environment ---"
    conda activate ${CONDA_ENV}
    ret=$?
    if [ $ret != "0" ]; then
        conda activate && \
        conda create -y --name ${CONDA_ENV} python=3.9 && \
        conda activate ${CONDA_ENV} || error_exit "conda environment ${CONDA_ENV} cannot be built"
    fi
    conda config --add channels conda-forge && \
    conda config --set channel_priority strict && \
    conda install -y boost cgal cmake eigen ffmpeg gdal jupyterlab matplotlib mysql numpy opencv openmp pcl pdal psutil "qhull=2019.1" qt scipy sphinx_rtd_theme spyder tbb tbb-devel xerces-c || error_exit "conda environment ${CONDA_ENV} cannot be completed"
}

# --- CloudComPy build

cloudcompy_setenv()
{
    echo "# --- set CloudComPy build environment ---"
    conda activate ${CONDA_ENV} || error_exit "${CONDA_ENV} is not a conda environment"
    echo ${CLOUDCOMPY_BUILD}
    echo ${CLOUDCOMPY_INSTALL}
    rm -rf ${CLOUDCOMPY_BUILD}
    rm -rf ${CLOUDCOMPY_INSTALL}
    mkdir -p ${CLOUDCOMPY_BUILD} && cd ${CLOUDCOMPY_BUILD} || error_exit "access problem to ${CLOUDCOMPY_BUILD}"
}

cloudcompy_configure()
{
    echo "# --- configure CloudComPy ---"
    cmake \
    -S"${CLOUDCOMPY_SRC}" \
    -B"${CLOUDCOMPY_BUILD}" \
    -G"Unix Makefiles" \
    -DCMAKE_C_COMPILER:FILEPATH=/usr/bin/clang \
    -DCMAKE_CXX_COMPILER:FILEPATH=/usr/bin/clang++ \
    -DCMAKE_Fortran_COMPILER:FILEPATH=/usr/bin/gfortran \
    -DBUILD_PY_TESTING:BOOL="1" \
    -DBUILD_REFERENCE_DOC:BOOL="1" \
    -DBUILD_TESTING:BOOL="1" \
    -DCCCORELIB_SHARED:BOOL="1" \
    -DCCCORELIB_USE_CGAL:BOOL="1" \
    -DCCCORELIB_USE_TBB:BOOL="0" \
    -DCMAKE_BUILD_TYPE:STRING="RelWithDebInfo" \
    -DCMAKE_INSTALL_PREFIX:PATH="${CLOUDCOMPY_INSTALL}" \
    -DFBX_SDK_INCLUDE_DIR:PATH="${FBXSDK_REP}/include" \
    -DFBX_SDK_LIBRARY_FILE:FILEPATH="${FBXSDK_REP}/lib/gcc/x64/release/libfbxsdk.a" \
    -DFBX_XML2_LIBRARY_FILE:FILEPATH="" \
    -DGDAL_INCLUDE_DIR:PATH="${CONDA_PATH}/include" \
    -DGDAL_LIBRARY:FILEPATH="${CONDA_PATH}/lib/libgdal.so" \
    -DGMP_INCLUDE_DIR:PATH="${CONDA_PATH}/include" \
    -DGMP_LIBRARIES:FILEPATH="${CONDA_PATH}/lib/libgmp.so" \
    -DGMP_LIBRARIES_DIR:FILEPATH="${CONDA_PATH}" \
    -DMPFR_INCLUDE_DIR:PATH="${CONDA_PATH}/include" \
    -DMPFR_LIBRARIES:FILEPATH="${CONDA_PATH}/lib/libmpfr.so" \
    -DMPFR_LIBRARIES_DIR:FILEPATH="${CONDA_PATH}" \
    -DOPTION_USE_GDAL:BOOL="1" \
    -DOpenMP_omp_LIBRARY:FILEPATH="${CONDA_PATH}/lib/libomp.so" \
    -DPLUGIN_EXAMPLE_GL:BOOL="1" \
    -DPLUGIN_EXAMPLE_IO:BOOL="1" \
    -DPLUGIN_EXAMPLE_STANDARD:BOOL="1" \
    -DPLUGIN_GL_QEDL:BOOL="1" \
    -DPLUGIN_GL_QSSAO:BOOL="1" \
    -DPLUGIN_IO_QADDITIONAL:BOOL="1" \
    -DPLUGIN_IO_QCSV_MATRIX:BOOL="1" \
    -DPLUGIN_IO_QDRACO:BOOL="0" \
    -DPLUGIN_IO_QE57:BOOL="1" \
    -DPLUGIN_IO_QFBX:BOOL="1" \
    -DPLUGIN_IO_QPDAL:BOOL="1" \
    -DPLUGIN_IO_QPHOTOSCAN:BOOL="1" \
    -DPLUGIN_STANDARD_QBROOM:BOOL="1" \
    -DPLUGIN_STANDARD_QCOMPASS:BOOL="1" \
    -DPLUGIN_STANDARD_QHPR:BOOL="1" \
    -DPLUGIN_STANDARD_QM3C2:BOOL="1" \
    -DPLUGIN_STANDARD_QPCL:BOOL="1" \
    -DPLUGIN_STANDARD_QPOISSON_RECON:BOOL="1" \
    -DPLUGIN_STANDARD_QRANSAC_SD:BOOL="1" \
    -DPYTHONAPI_TEST_DIRECTORY:STRING="CloudComPy/Data" \
    -DPYTHONAPI_TRACES:BOOL="1" \
    -DPYTHON_PREFERED_VERSION:STRING="3.9" \
    -DTBB_DIR:PATH="${CONDA_PATH}/lib/cmake/TBB" \
    -DUSE_CONDA_PACKAGES:BOOL="1" \
    -DXercesC_INCLUDE_DIR:PATH="${CONDA_PATH}/include" \
    -DXercesC_LIBRARY_DEBUG:FILEPATH="XercesC_LIBRARY_DEBUG-NOTFOUND" \
    -DXercesC_LIBRARY_RELEASE:FILEPATH="${CONDA_PATH}/lib/libxerces-c.so" \
    -DZLIB_INCLUDE_DIR:PATH="${CONDA_PATH}/include" \
    -DZLIB_LIBRARY_RELEASE:FILEPATH="${CONDA_PATH}/lib/libz.so"
}

cloudcompy_build()
{
    echo "# --- build and install CloudComPy ---"
    cd ${CLOUDCOMPY_BUILD} && make -j${NBTHREADS} && make install
}

cloudcompy_tarfile()
{
    echo "# --- generate CloudComPy binaries tarfile ---"
    cd ${CLOUDCOMPY_INSTDIR} && rm -f ${CLOUDCOMPY_TARFILE} &&\
    tar cvzf ${CLOUDCOMPY_TARFILE} ${CLOUDCOMPY_INSTNAME}
}

cloudcompy_test()
{
    echo "# --- test CloudComPy ---"
    cd ${CLOUDCOMPY_INSTALL} && \
    . bin/condaCloud.sh activate ${CLOUDCOMPY_INSTNAME} && \
    rm -rf ~/CloudComPy/Data &&
    cd doc/PythonAPI_test && ctest
}

conda_buildenv  && \
cloudcompy_setenv && \
cloudcompy_configure && \
cloudcompy_build && \
cloudcompy_tarfile && \
cloudcompy_test
```
