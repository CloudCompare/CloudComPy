#!/bin/zsh
pyindex=$1
export PYMINVERS=("10" "11" "12" "13" "14")                                                    # Python minor versions to build
export PYFULLVERS=("3.10.20" "3.11.15" "3.12.13" "3.13.13" "3.14.4")                           # Python full versions to build
export PYMINOR=${PYMINVERS[$pyindex+1]}                                                          # Python minor version
export PYBASE=${HOME}/projets/CloudComPy/install/${PYFULLVERS[$pyindex+1]}                       # Python used to build venv for doc and tests
export CLOUDCOMPY_SRC=${HOME}/projets/CloudComPy/CloudComPy                                    # CloudComPy source directory
export CLOUDCOMPY_BUILD=${HOME}/projets/CloudComPy/buildConda3${PYMINOR}                       # CloudComPy build directory
export CLOUDCOMPY_INSTDIR=${HOME}/projets/CloudComPy/installConda                              # directory for CloudComPy installs
export CLOUDCOMPY_INSTNAME=CloudComPy_PyPI                                               # CloudComPy install directory name
export CLOUDCOMPY_INSTALL=${CLOUDCOMPY_INSTDIR}/${CLOUDCOMPY_INSTNAME}                         # CloudComPy install directory
export CLOUDCOMPY_TARFILE=CloudComPy_Conda3${PYMINOR}_MacOS_"$(date +"%Y%m%d-%H%M")".tar.xz    # CloudComPy Binary tarfile (will be in ${CLOUDCOMPY_INSTDIR}

export CONDA_ROOT=${HOME}/miniconda3                                                   # root directory of conda installation
export CONDA_ENV=CloudComPy3${PYMINOR}                                                 # conda environment name
export CONDA_PATH=${CONDA_ROOT}/envs/${CONDA_ENV}                                      # conda environment directory
export QT_PREFIX=${CONDA_PATH}/lib/qt6                                                 # prefix for qt (if qt plugins are needed, otherwise set to empty or remove from cmake options)

export PYTHONVENV=${HOME}/projets/CloudComPy/venv3${PYMINOR}doc                        # Python venv for documentation and building wheel
export PYTHONTESTENV=${HOME}/projets/CloudComPy/venv3${PYMINOR}test                    # Python venv for tests
export CORK_REP=${HOME}/projets/CloudComPy/Cork/cork                                   # directory of cork (remove the plugin in cmake options if not needed)
export FBXSDK_REP="/Applications/Autodesk/FBX SDK/2020.2.1"                            # directory of fbx sdk (remove the plugin in cmake options if not needed)
export LIBIGL_REP=${HOME}/projets/CloudComPy/libigl                                    # directory of libigl (remove the plugin in cmake options if not needed)
export OPENCASCADE_REP=${CONDA_PATH}                                                   # directory of OpenCascade (remove the plugin in cmake options if not needed)
# export PCLLIB_REP=${HOME}/projets/CloudComPy/pcl/install                               # patch on pcl lib (issue #100): libpcl_common.so
export NBTHREADS=10                                                                    # number of threads for parallel make
export CLOUDCOMPARE_VERSION="2.14.beta"                                                # CloudCompare version for documentation sed for doc)
export CLOUDCOMPY_VERSION="2.13.90"                                                    # CloudComPy version for documentation sed for doc)
export WHEELHOUSE_REPO=${HOME}/projets/CloudComPy/Wheelhouse

. ${CONDA_ROOT}/etc/profile.d/conda.sh                                                 # required to have access to conda commands in a shell script

error_exit()
{
  echo "Error $1" 1>&2
  exit 1
}

# --- conda environment
#     bug lldb with latest version openssl (>=3.1) https://stackoverflow.com/questions/74059978/why-is-lldb-generating-exc-bad-instruction-with-user-compiled-library-on-macos/76032052#76032052

conda_buildenv()
{
    echo "# --- build conda environment for Python 3.${PYMINOR} ---"
    conda install -y -n base mamba -c conda-forge
    conda update -y -n base -c defaults conda
    conda activate ${CONDA_ENV}
    ret=$?
    #ret=1
    if [ $ret != "0" ]; then
        conda activate && \
        mamba env create -y -n CloudComPy3${PYMINOR} -f CloudComPy3${PYMINOR}Qt6_MacOS.yml && \
        conda activate ${CONDA_ENV} || error_exit "conda environment ${CONDA_ENV} cannot be built"
    fi
    conda deactivate
}

# --- python venv for documentation and tests

python_buildenv()
{
    echo "# --- build Python venv for Python 3.${PYMINOR} ---"
    rm -rf ${PYTHONVENV}
    ${PYBASE}/bin/python3.${PYMINOR} -m venv ${PYTHONVENV}
    source ${PYTHONVENV}/bin/activate
    python3 -m pip install --upgrade pip
    pip install numpy scipy requests psutil matplotlib numpy-quaternion pybind11 sphinx-rtd-theme cmake
    deactivate
}


# --- CloudComPy build

cloudcompy_setenv()
{
    echo "# --- set CloudComPy build environment for Python 3.${PYMINOR} ---"
    conda activate ${CONDA_ENV} || error_exit "${CONDA_ENV} is not a conda environment"
    conda list > ${CLOUDCOMPY_SRC}/building/conda-list_macOS_3${PYMINOR} || error_exit "access problem to ${CLOUDCOMPY_SRC}"
    echo ${CLOUDCOMPY_BUILD}
    echo ${CLOUDCOMPY_INSTALL}
    rm -rf ${CLOUDCOMPY_BUILD}
    rm -rf ${CLOUDCOMPY_INSTALL}
    mkdir -p ${CLOUDCOMPY_BUILD} && cd ${CLOUDCOMPY_BUILD} || error_exit "access problem to ${CLOUDCOMPY_BUILD}"
}

cloudcompy_configure()
{
    echo "# --- configure CloudComPy for Python 3.${PYMINOR} ---"
    cmake \
    -S"${CLOUDCOMPY_SRC}" \
    -B"${CLOUDCOMPY_BUILD}" \
    -G"Unix Makefiles" \
    -DBUILD_PYPI="1" \
    -DBUILD_PY_TESTING:BOOL="1" \
    -DBUILD_REFERENCE_DOC:BOOL="1" \
    -DBUILD_TESTING:BOOL="1" \
    -DCCCORELIB_SHARED:BOOL="1" \
    -DCCCORELIB_USE_CGAL:BOOL="1" \
    -DCCCORELIB_USE_QT_CONCURRENT:BOOL="1" \
    -DCCCORELIB_USE_TBB:BOOL="0" \
    -DCLOUDCOMPARE_VERSION:STRING="${CLOUDCOMPARE_VERSION}" \
    -DCLOUDCOMPY_VERSION:STRING="${CLOUDCOMPY_VERSION}" \
    -DCGAL_DIR:PATH="${CONDA_PATH}/lib/cmake/CGAL" \
    -DCMAKE_BUILD_TYPE:STRING="Release" \
    -DCMAKE_OSX_DEPLOYMENT_TARGET="13.0" \
    -DCMAKE_INSTALL_PREFIX:PATH="${CLOUDCOMPY_INSTALL}" \
    -DCMAKE_INSTALL_RPATH="${CLOUDCOMPY_INSTALL}/cloudComPy/CloudCompare/CloudCompare.app/Contents/Frameworks" \
    -DCMAKE_MACOSX_RPATH=ON \
    -DCONDA_LIBS:PATH="${CONDA_PATH}/lib" \
    -DCONDA_PATH:PATH="${CONDA_PATH}" \
    -DCORK_INCLUDE_DIR:PATH="${CORK_REP}/src" \
    -DCORK_RELEASE_LIBRARY_FILE:FILEPATH="${CORK_REP}/lib/libcork.a" \
    -DDRACO_INCLUDE_DIR:PATH="${CONDA_PATH}/include" \
    -DDRACO_LIB_DIR:PATH="${CONDA_PATH}/lib" \
    -DDRACO_LIBRARIES:PATH="${CONDA_PATH}/lib/libdraco.a" \
    -DEIGEN_INCLUDE_DIR:PATH="${CONDA_PATH}/include/eigen3" \
    -DEIGEN_ROOT_DIR:PATH="${CONDA_PATH}/include/eigen3" \
    -DFFMPEG_INCLUDE_DIR:PATH="${CONDA_PATH}/include" \
    -DFFMPEG_LIBRARY_DIR:FILEPATH="${CONDA_PATH}/lib" \
    -DFFMPEG_X264_LIBRARY_DIR:FILEPATH="${CONDA_PATH}/lib" \
    -DFBX_SDK_INCLUDE_DIR:PATH="${FBXSDK_REP}/include" \
    -DFBX_SDK_LIBRARY_DIR:PATH="${FBXSDK_REP}/lib/clang/release" \
    -DFBX_SDK_LIBRARY_FILE:FILEPATH="${FBXSDK_REP}/lib/clang/release/libfbxsdk.dylib" \
    -DFBX_XML2_LIBRARY_FILE:FILEPATH="" \
    -DGDAL_INCLUDE_DIR:PATH="${CONDA_PATH}/include" \
    -DGDAL_LIBRARY:FILEPATH="${CONDA_PATH}/lib/libgdal.dylib" \
    -DGMPXX_INCLUDE_DIR:PATH="${CONDA_PATH}/include" \
    -DGMPXX_LIBRARIES:FILEPATH="${CONDA_PATH}/lib/libgmp.dylib" \
    -DGMP_INCLUDE_DIR:PATH="${CONDA_PATH}/include" \
    -DGMP_LIBRARY_DEBUG:FILEPATH="${CONDA_PATH}/lib/libgmp.dylib" \
    -DGMP_LIBRARY_RELEASE:FILEPATH="${CONDA_PATH}/lib/libgmp.dylib" \
    -DLASZIP_INCLUDE_DIR:PATH="${CONDA_PATH}/include" \
    -DLASZIP_LIBRARY:FILEPATH="${CONDA_PATH}/lib/liblaszip.dylib" \
    -DLIBIGL_INCLUDE_DIR:PATH="${LIBIGL_REP}/libigl/include" \
    -DLIBIGL_RELEASE_LIBRARY_FILE:FILEPATH="${LIBIGL_REP}/install/lib/libigl.a" \
    -DMPFR_INCLUDE_DIR:PATH="${CONDA_PATH}/include" \
    -DMPFR_LIBRARIES:FILEPATH="${CONDA_PATH}/lib/libmpfr.dylib" \
    -DMPIR_INCLUDE_DIR:PATH="${CONDA_PATH}/include" \
    -DMPIR_RELEASE_LIBRARY_FILE:FILEPATH="${CONDA_PATH}/lib/libgmp.dylib" \
    -DOPENCASCADE_78_OR_NEWER:BOOL="1" \
    -DOPENCASCADE_DLL_DIR:PATH="${OPENCASCADE_REP}/lib" \
    -DOPENCASCADE_INC_DIR:PATH="${OPENCASCADE_REP}/include/opencascade" \
    -DOPENCASCADE_LIB_DIR:PATH="${OPENCASCADE_REP}/lib" \
    -DOPENCASCADE_TBB_DLL_DIR:PATH="${OPENCASCADE_REP}/lib" \
    -DOPTION_BUILD_CCVIEWER:BOOL="0" \
    -DOPTION_SUPPORT_3DMOUSE_WITH_3DxWARE="0" \
    -DOPTION_SUPPORT_3DMOUSE_WITH_HIDAPI="1" \
    -DOPTION_HID_DEBUG="0" \
    -DOPTION_USE_GDAL:BOOL="1" \
    -DOpenCV_DIR:PATH="${CONDA_PATH}/lib/cmake/opencv4" \
    -DPCL_DIR:PATH="${CONDA_PATH}/share/pcl-1.15" \
    -DPNG_INCLUDE_DIR:PATH="${CONDA_PATH}/include/libpng16" \
    -DPNG_LIBRARIES:FILEPATH="${CONDA_PATH}/lib/libpng16.dylib" \
    -DPLUGIN_EXAMPLE_GL:BOOL="1" \
    -DPLUGIN_EXAMPLE_IO:BOOL="1" \
    -DPLUGIN_EXAMPLE_STANDARD:BOOL="1" \
    -DPLUGIN_GL_QEDL:BOOL="1" \
    -DPLUGIN_GL_QSSAO:BOOL="1" \
    -DPLUGIN_IO_QADDITIONAL:BOOL="1" \
    -DPLUGIN_IO_QCORE:BOOL="1" \
    -DPLUGIN_IO_QCSV_MATRIX:BOOL="1" \
    -DPLUGIN_IO_QDRACO:BOOL="1" \
    -DPLUGIN_IO_QE57:BOOL="1" \
    -DPLUGIN_IO_QFBX:BOOL="1" \
    -DPLUGIN_IO_QLAS:BOOL="1" \
    -DPLUGIN_IO_QLAS_FWF:BOOL="0" \
    -DPLUGIN_IO_QPDAL:BOOL="0" \
    -DPLUGIN_IO_QPHOTOSCAN:BOOL="1" \
    -DPLUGIN_IO_QRDB:BOOL="0" \
    -DPLUGIN_IO_QRDB_FETCH_DEPENDENCY:BOOL="1" \
    -DPLUGIN_IO_QRDB_INSTALL_DEPENDENCY:BOOL="1" \
    -DPLUGIN_IO_QSTEP:BOOL="1" \
    -DPLUGIN_PYTHON="OFF" \
    -DPLUGIN_STANDARD_3DMASC:BOOL="1" \
    -DPLUGIN_STANDARD_MASONRY_QAUTO_SEG:BOOL="1" \
    -DPLUGIN_STANDARD_MASONRY_QMANUAL_SEG:BOOL="1" \
    -DPLUGIN_STANDARD_QANIMATION:BOOL="1" \
    -DPLUGIN_STANDARD_QBROOM:BOOL="1" \
    -DPLUGIN_STANDARD_QCANUPO:BOOL="1" \
    -DPLUGIN_STANDARD_QCLOUDLAYERS:BOOL="1" \
    -DPLUGIN_STANDARD_QCOLORIMETRIC_SEGMENTER:BOOL="1" \
    -DPLUGIN_STANDARD_QCOMPASS:BOOL="1" \
    -DPLUGIN_STANDARD_QCORK:BOOL="1" \
    -DPLUGIN_STANDARD_QCSF:BOOL="1" \
    -DPLUGIN_STANDARD_QFACETS:BOOL="1" \
    -DPLUGIN_STANDARD_QHOUGH_NORMALS:BOOL="1" \
    -DPLUGIN_STANDARD_QHPR:BOOL="1" \
    -DPLUGIN_STANDARD_QJSONRPC:BOOL="1" \
    -DPLUGIN_STANDARD_QM3C2:BOOL="1" \
    -DPLUGIN_STANDARD_QMESH_BOOLEAN:BOOL="1" \
    -DPLUGIN_STANDARD_QMPLANE:BOOL="1" \
    -DPLUGIN_STANDARD_QPCL:BOOL="1" \
    -DPLUGIN_STANDARD_QPCV:BOOL="1" \
    -DPLUGIN_STANDARD_QPOISSON_RECON:BOOL="1" \
    -DPLUGIN_STANDARD_QRANSAC_SD:BOOL="1" \
    -DPLUGIN_STANDARD_QSRA:BOOL="1" \
    -DPLUGIN_STANDARD_QTREEISO:BOOL="1" \
    -Dpybind11_DIR:PATH="${CONDA_PATH}/share/cmake/pybind11" \
    -DPYTHONAPI_TEST_DIRECTORY:STRING="CloudComPy/Data" \
    -DPYTHONAPI_EXTDATA_DIRECTORY:STRING="CloudComPy/ExternalData" \
    -DPYTHONAPI_TRACES:BOOL="1" \
    -DPYMINOR:STRING="${PYMINOR}" \
    -DPYTHON_PREFERED_VERSION:STRING="3.${PYMINOR}" \
    -DPYTHONVENV_DIR:PATH="${PYTHONVENV}" \
    -DQANIMATION_WITH_FFMPEG_SUPPORT:BOOL="1" \
    -DQHULL_LIBRARY_DEBUG:FILEPATH="${CONDA_PATH}/lib/libqhullcpp.a" \
    -DQHULL_LIBRARY_DEBUG_STATIC:FILEPATH="${CONDA_PATH}/lib/libqhullstatic_r.a" \
    -DQHULL_LIBRARY_SHARED:FILEPATH="${CONDA_PATH}/lib/libqhull_r.dylib" \
    -DQHULL_LIBRARY_STATIC:FILEPATH="${CONDA_PATH}/lib/libqhullstatic_r.a" \
    -DQhull_DIR:PATH="${CONDA_PATH}/lib/cmake/Qhull" \
    -DREPO_DIR:PATH="${CLOUDCOMPY_SRC}" \
    -DTBB_DIR:PATH="${CONDA_PATH}/lib/cmake/TBB" \
    -DUSE_CONDA_PACKAGES:BOOL="1" \
    -DUSE_EXTERNAL_QHULL_FOR_QHPR:BOOL="0" \
    -DXercesC_INCLUDE_DIR:PATH="${CONDA_PATH}/include" \
    -DXercesC_LIBRARY_DEBUG:FILEPATH="${CONDA_PATH}/lib/libxerces-c.dylib" \
    -DXercesC_LIBRARY_RELEASE:FILEPATH="${CONDA_PATH}/lib/libxerces-c.dylib" \
    -DZLIB_INCLUDE_DIR:PATH="${CONDA_PATH}/include" \
    -DZLIB_LIBRARY_RELEASE:FILEPATH="${CONDA_PATH}/lib/libz.1.dylib"
}

cloudcompy_build()
{
    echo "# --- build and install CloudComPy ---"
    cd ${CLOUDCOMPY_BUILD} && make -j${NBTHREADS} && make install
}

cloudcompy_tarfile()
{
    echo "# --- generate CloudComPy binaries tarfile ---"
    cd ${CLOUDCOMPY_INSTNAME} && find . -type d -name __pycache__ -exec rm -rf {} \;
    cd ${CLOUDCOMPY_INSTDIR} && rm -f ${CLOUDCOMPY_TARFILE} &&\
    tar -cvJf ${CLOUDCOMPY_TARFILE} ${CLOUDCOMPY_INSTNAME}
}

cloudcompy_gen_wheel()
{
    echo "# --- generate CloudComPy wheel with ${PYTHONVENV} ---"
    deactivate
    conda deactivate
    source ${PYTHONVENV}/bin/activate
    echo "Python version: $(python --version)"
    pip install build delocate wheel pkginfo
    cd ${CLOUDCOMPY_SRC}
    cp -f MacOS/pyproject.toml pyproject.toml
    cp -f MacOS/setup.py setup.py
    rm -rf build dist
    python -m build --wheel
    delocate-wheel --ignore-missing-dependencies --no-sanitize-rpaths -v dist/*.whl
    twine check dist/*.whl 
    deactivate
}

cloudcompy_sign_wheel()
{
    echo "# --- sign CloudComPy wheel ---"
    source ${PYTHONVENV}/bin/activate
    cd ${CLOUDCOMPY_SRC}/dist
    rm -rf work/wheel_unpacked
    mkdir -p work/wheel_unpacked
    python3 -m wheel unpack cloudcompy-${CLOUDCOMPY_VERSION}-py3-none-any.whl -d work/wheel_unpacked
    # sign all the .dylib, .so and executable files in the wheel
    python3 ${CLOUDCOMPY_SRC}/CloudCompare/Scripts/mac/bundle/signatureCloudCompare.py work/wheel_unpacked/cloudcompy-${CLOUDCOMPY_VERSION}/cloudComPy

    APP_PATH="${CLOUDCOMPY_SRC}/dist/work/wheel_unpacked/cloudcompy-${CLOUDCOMPY_VERSION}"
    NOWIS=`date "+%Y%m%d_%H%M%S"`
    ZIP_PATH="${CLOUDCOMPY_SRC}/dist/work/cloudcompy-${CLOUDCOMPY_VERSION}_$NOWIS.zip"
    # --- notarize and staple cloudCompare.app
    #     - Create a ZIP archive suitable for notarization.
    #     - submit for notarization
    #     - staple the app: message "The staple and validate action worked!"
    /usr/bin/ditto -c -k -v --keepParent "$APP_PATH" "$ZIP_PATH"
    xcrun notarytool submit "$ZIP_PATH" --keychain-profile "AC_PASSWORD" --wait
    xcrun stapler staple ${APP_PATH}/cloudComPy/CloudCompare/CloudCompare.app
    xcrun stapler validate ${APP_PATH}/cloudComPy/CloudCompare/CloudCompare.app

    # repack the wheel
    cd work/wheel_unpacked/cloudcompy-${CLOUDCOMPY_VERSION}
    ditto -c -k --sequesterRsrc . ${WHEELHOUSE_REPO}/cloudcompy-${CLOUDCOMPY_VERSION}-cp3${PYMINOR}-cp3${PYMINOR}-macosx_13_0_arm64.whl
    deactivate
}

cloudcompy_test()
{
    echo "# --- test CloudComPy ---"
    rm -rf ${PYTHONTESTENV}
    ${PYBASE}/bin/python3.${PYMINOR} -m venv ${PYTHONTESTENV}
    source ${PYTHONTESTENV}/bin/activate
    python3 -m pip install --upgrade pip
    cd ${CLOUDCOMPY_SRC}
    pip install ${WHEELHOUSE_REPO}/cloudcompy-${CLOUDCOMPY_VERSION}-cp3${PYMINOR}-cp3${PYMINOR}-macosx_13_0_arm64.whl
    rm -rf ~/CloudComPy/Data && \
    cd ${PYTHONTESTENV}/lib/python3.${PYMINOR}/site-packages/cloudComPy/doc/PythonAPI_test && ctest
}

conda_buildenv && \
python_buildenv &&\
cloudcompy_setenv && \
cloudcompy_configure && \
cloudcompy_build && \
cloudcompy_gen_wheel&& \
cloudcompy_sign_wheel && \
cloudcompy_test

#cloudcompy_tarfile && \
