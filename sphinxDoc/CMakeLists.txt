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

message(STATUS "cloudComPy Sphinx doc generation")

# --- Sphinx Documentation

set(RSTFILES
    Canupo.rst
    ccFacet.rst
    ccMesh.rst
    ccOctree.rst
    ccPointCloud.rst
    ccPolyline.rst
    ccPrimitives.rst
    ccSensor.rst
    cloudComPy.rst
    cloudSamplingTools.rst
    colors.rst
    CSF.rst
    DistanceComputationTools.rst
    GeometricalAnalysisTools.rst
    HoughNormals.rst
    HPR.rst
    index.rst
    introduction.rst
    license.rst
    M3C2.rst
    MeshBoolean.rst
    MinimalBoundingBox.rst
    Neighbourhood.rst
    PCL.rst
    PCV.rst
    PoissonRecon.rst
    RANSAC_SD.rst
    ScalarField.rst
    SRA.rst
    tables.rst
    userEntities.rst
    userIntro.rst
    userLoad.rst
    userNumpy.rst
    userSave.rst
    userUseCases.rst
    version.rst
    )

if( BUILD_PYPI )
    set(IS_PYPI_BUILD "true")
    set(REPO_GIT_DIR "${REPO_DIR}")
else()
    set(IS_PYPI_BUILD "false")
    set(REPO_GIT_DIR "${CMAKE_SOURCE_DIR}")
endif()
message(STATUS "REPO_GIT_DIR: ${REPO_GIT_DIR}")

set(SPHINXOPTS )
set(SOURCEDIR ${CMAKE_CURRENT_SOURCE_DIR})
set(SPHINXDOCTREES "${CMAKE_CURRENT_BINARY_DIR}/doctrees")
set(ALLSPHINXOPTS "${SPHINXOPTS} ${SOURCEDIR}")
set(SPHINXBUILD sphinx-build)

string(TIMESTAMP BUILD_DATE "%Y/%m/%d")

execute_process(
    COMMAND git rev-parse --short HEAD
    WORKING_DIRECTORY ${REPO_GIT_DIR}
    OUTPUT_VARIABLE CLOUDCOMPY_GIT_COMMIT
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
message(STATUS "CLOUDCOMPY_GIT_COMMIT: ${CLOUDCOMPY_GIT_COMMIT}")

execute_process(
    COMMAND git rev-parse --short HEAD
    WORKING_DIRECTORY ${REPO_GIT_DIR}/CloudCompare
    OUTPUT_VARIABLE CLOUDCOMPARE_GIT_COMMIT
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
message(STATUS "CLOUDCOMPARE_GIT_COMMIT: ${CLOUDCOMPARE_GIT_COMMIT}")

execute_process(
    COMMAND git log -1 --format=%cd --date=format:%Y/%m/%d
    WORKING_DIRECTORY ${REPO_GIT_DIR}
    OUTPUT_VARIABLE CLOUDCOMPY_COMMIT_DATE
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
message(STATUS "CLOUDCOMPY_COMMIT_DATE: ${CLOUDCOMPY_COMMIT_DATE}")

execute_process(
    COMMAND git log -1 --format=%cd --date=format:%Y/%m/%d
    WORKING_DIRECTORY ${REPO_GIT_DIR}/CloudCompare
    OUTPUT_VARIABLE CLOUDCOMPARE_COMMIT_DATE
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
message(STATUS "CLOUDCOMPARE_COMMIT_DATE: ${CLOUDCOMPARE_COMMIT_DATE}")

configure_file(conf.py.in conf.py @ONLY)
configure_file(getDependency.py.in getDependency.py)

if(WIN32)
    if (INSTALL_PREREQUISITE_LIBRARIES)
        configure_file(genSphinxDocWindowsDeployQt.bat.in genSphinxDoc.bat @ONLY)
        configure_file(genBundleDeployQt.bat.in genBundle.bat @ONLY)
    else()
        configure_file(genSphinxDocWindows.bat.in genSphinxDoc.bat @ONLY)
        configure_file(genBundle.bat.in genBundle.bat @ONLY)
        configure_file(WinBundle.py.in.py WinBundle.py @ONLY)
    endif()
elseif (USE_CONDA_PACKAGES)
    if(APPLE)
	    configure_file(genSphinxDocMacOSConda.zsh.in genSphinxDoc.zsh @ONLY )
	    #configure_file(libBundleCloudComPy.py.in libBundleCloudComPy.py)
	    #configure_file(signatureCloudComPy.sh.in signatureCloudComPy.sh @ONLY )
    else()
	    configure_file(genSphinxDocConda.sh.in genSphinxDoc.sh @ONLY)
        configure_file(getDependency.py.in getDependency.py @ONLY)
        configure_file(genLinuxBundle.sh.in genLinuxBundle.sh)
    endif()
else()
    message( STATUS "Sphinx doc will be generated without conda dependencies (native build)" )
    configure_file(genSphinxDoc.sh.in genSphinxDoc.sh @ONLY)
    configure_file(getDependency.py.in getDependency.py @ONLY)
    configure_file(genLinuxBundle_native.sh.in genLinuxBundle.sh @ONLY)
endif()

add_custom_target(BUILD_HTML ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory doctrees
    DEPENDS ${RSTFILES} _cloudComPy
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

option( BUILD_REFERENCE_DOC "Build reference documentation for CloudComPy" ON )

# --- Sphinx Documentation at install time

if ( BUILD_REFERENCE_DOC )
    message( STATUS "Sphinx documentation will be generated at install" )
    install(CODE "set(CMAKE_SOURCE_DIR \"${CMAKE_SOURCE_DIR}\")")
    install(CODE "set(CMAKE_INSTALL_PREFIX \"${CMAKE_INSTALL_PREFIX}\")")
    if( BUILD_PYPI )
        install(CODE "set(WHEEL_TAG \"${WHEEL_TAG}\")")
        install( SCRIPT postInstallPyPI.cmake )
    else()
        install( SCRIPT postInstall.cmake )
    endif()
endif()

