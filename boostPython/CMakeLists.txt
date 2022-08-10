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

message(STATUS "Python3 and BoostPython processing")

# --- Python detection: if several versions installed, select one with all the required packages installed

set(PYTHON_PREFERED_VERSION "3.9" CACHE STRING "Select a Python version: it should be installed with all packages required for CloudCompare (pyqt, sip, numpy)")
set_property(CACHE PYTHON_PREFERED_VERSION PROPERTY STRINGS "3.6" "3.7" "3.8" "3.9" "3.10")
message(STATUS "  python prefered version: " ${PYTHON_PREFERED_VERSION})
set(Python_ADDITIONAL_VERSIONS ${PYTHON_PREFERED_VERSION}) # priority to this version 
find_package (Python3 COMPONENTS Interpreter Development)
message(STATUS "  python executable: " ${Python3_EXECUTABLE})
message(STATUS "  python version: " ${Python3_VERSION})
message(STATUS "  check match for ${PYTHON_PREFERED_VERSION}.x")
if(NOT ${Python3_VERSION} MATCHES "${PYTHON_PREFERED_VERSION}\\..")
    message(SEND_ERROR "no Python found matching ${PYTHON_PREFERED_VERSION}.x")
else()
    message(STATUS "Python found ${Python3_VERSION} matches prefered version ${PYTHON_PREFERED_VERSION}.x")
    message(STATUS "  Python3_LIBRARIES:" ${Python3_LIBRARIES})
    message(STATUS "  Python3_INCLUDE_DIRS" ${Python3_INCLUDE_DIRS})
endif()

# --- Boost detection
#message( STATUS "Python3_VERSION_MAJOR: " ${Python3_VERSION_MAJOR} )
#message( STATUS "Python3_VERSION_MINOR: " ${Python3_VERSION_MINOR} )
#find_package(Boost COMPONENTS python numpy)
#find_package(Boost COMPONENTS python${Python3_VERSION_MAJOR}${Python3_VERSION_MINOR}  numpy${Python3_VERSION_MAJOR}${Python3_VERSION_MINOR} REQUIRED)

#message(STATUS "Boost_LIBRARIES = ${Boost_LIBRARIES}")

# --- pybind11 detection
find_package(pybind11 REQUIRED)
message( STATUS "pybind11_VERSION: ${pybind11_VERSION}")
message( STATUS "pybind11_LIBRARIES: ${pybind11_LIBRARIES}")
message( STATUS "pybind11_DEFINITIONS: ${pybind11_DEFINITIONS}")
message( STATUS "pybind11_INCLUDE_DIR: ${pybind11_INCLUDE_DIR}")
message( STATUS "pybind11_INCLUDE_DIRS: ${pybind11_INCLUDE_DIRS}")

# --- ----------------------------------------------------------
# --- target boost.Python library: Python module cloudComPy

project( _cloudComPy VERSION 2.12.0 )

add_library( ${PROJECT_NAME} SHARED )

set_property(TARGET ${PROJECT_NAME} PROPERTY AUTOMOC ON)

    target_sources( ${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/cloudComPy.cpp
    ${CMAKE_CURRENT_LIST_DIR}/colorsPy.cpp
#    ${CMAKE_CURRENT_LIST_DIR}/ScalarFieldPy.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ccGenericCloudPy.cpp
#    ${CMAKE_CURRENT_LIST_DIR}/ccOctreePy.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ccPointCloudPy.cpp
#    ${CMAKE_CURRENT_LIST_DIR}/ccMeshPy.cpp
#    ${CMAKE_CURRENT_LIST_DIR}/ccPrimitivesPy.cpp
#    ${CMAKE_CURRENT_LIST_DIR}/ccPolylinePy.cpp
#    ${CMAKE_CURRENT_LIST_DIR}/distanceComputationToolsPy.cpp
#    ${CMAKE_CURRENT_LIST_DIR}/geometricalAnalysisToolsPy.cpp
    ${CMAKE_CURRENT_LIST_DIR}/registrationToolsPy.cpp
#    ${CMAKE_CURRENT_LIST_DIR}/cloudSamplingToolsPy.cpp
#    ${CMAKE_CURRENT_LIST_DIR}/ccFacetPy.cpp
#    ${CMAKE_CURRENT_LIST_DIR}/ccSensorPy.cpp
#    ${CMAKE_CURRENT_LIST_DIR}/NeighbourhoodPy.cpp
    )

target_include_directories( ${PROJECT_NAME} PRIVATE
#    ${Boost_INCLUDE_DIRS}
    ${pybind11_INCLUDE_DIR}
    ${Python3_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../pyAPI
    ${CCCoreLib_SOURCE_DIR}/include
    ${QCC_DB_LIB_SOURCE_DIR}
    )

target_link_libraries( ${PROJECT_NAME}
    PYCC_LIB
    CCAppCommon
#    ${Boost_LIBRARIES}
    ${Python3_LIBRARIES}
    )

# Qt libraries

target_link_libraries( ${PROJECT_NAME}
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::OpenGL
    Qt5::PrintSupport
    )

if (WIN32)
   target_link_libraries( ${PROJECT_NAME} Qt5::WinMain)
endif()

# --- environment scripts, with conda

set (ENV_SCRIPTS condaCloud.sh)
option(USE_CONDA_PACKAGES "Use Conda packages and environment for build: add commands for set/unset run environment" OFF)

# --- Python scripts with added functions

set(PYTHONAPI_SCRIPTS
    __init__.py
	minimalBoundingBox.py
	)

# --- install

if( WIN32 OR APPLE)
	install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION CloudCompare )
	install(PROGRAMS ${PYTHONAPI_SCRIPTS} DESTINATION CloudCompare/cloudComPy )
else()
    set(INSTALL_CLOUDCOMPARE_PYTHON ${CMAKE_INSTALL_LIBDIR}/cloudcompare )
    message(STATUS "INSTALL_CLOUDCOMPARE_PYTHON: " ${INSTALL_CLOUDCOMPARE_PYTHON})
    install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION ${INSTALL_CLOUDCOMPARE_PYTHON} )
    install(PROGRAMS ${PYTHONAPI_SCRIPTS} DESTINATION ${INSTALL_CLOUDCOMPARE_PYTHON}/cloudComPy)
    if (USE_CONDA_PACKAGES)
        install(PROGRAMS ${ENV_SCRIPTS} DESTINATION ${CMAKE_INSTALL_BINDIR})
    endif()
endif()

if(WIN32)
  set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".pyd" DEBUG_OUTPUT_NAME _cloudComPy_d RELEASE_OUTPUT_NAME _cloudComPy)
else()
  set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")
endif()

if(WIN32)
  if( NOT INSTALL_PREREQUISITE_LIBRARIES )
    set( QT_DLL_FILES ${Qt5_DIR}/../../../bin/Qt5Concurrent.dll
                      ${Qt5_DIR}/../../../bin/Qt5Core.dll
                      ${Qt5_DIR}/../../../bin/Qt5Gui.dll
                      ${Qt5_DIR}/../../../bin/Qt5OpenGL.dll
                      ${Qt5_DIR}/../../../bin/Qt5PrintSupport.dll
                      ${Qt5_DIR}/../../../bin/Qt5Svg.dll
                      ${Qt5_DIR}/../../../bin/Qt5Widgets.dll
                      ${Qt5_DIR}/../../../bin/Qt5WebSockets.dll
                      ${Qt5_DIR}/../../../bin/Qt5Network.dll)
    message( STATUS "QT_DLL_FILES ${QT_DLL_FILES}" )
	install( FILES ${QT_DLL_FILES} DESTINATION CloudCompare )

    if ( PLUGIN_IO_QRDB )
        message( STATUS "rdblib.dll ${CMAKE_CURRENT_BINARY_DIR}/../_deps/rdb_sdk-src/library/rdblib.dll")
        install( FILES ${CMAKE_CURRENT_BINARY_DIR}/../_deps/rdb_sdk-src/library/rdblib.dll DESTINATION CloudCompare )
    endif()

    if ( PLUGIN_IO_QSTEP )
        set( OCCT_DLL_FILES
             ${OPENCASCADE_DLL_DIR}/TKSTEP.dll
             ${OPENCASCADE_DLL_DIR}/TKSTEPAttr.dll
             ${OPENCASCADE_DLL_DIR}/TKSTEP209.dll
             ${OPENCASCADE_DLL_DIR}/TKSTEPBase.dll
             ${OPENCASCADE_DLL_DIR}/TKShHealing.dll
             ${OPENCASCADE_DLL_DIR}/TKTopAlgo.dll
             ${OPENCASCADE_DLL_DIR}/TKGeomAlgo.dll
             ${OPENCASCADE_DLL_DIR}/TKBRep.dll
             ${OPENCASCADE_DLL_DIR}/TKGeomBase.dll
             ${OPENCASCADE_DLL_DIR}/TKG3D.dll
             ${OPENCASCADE_DLL_DIR}/TKG2D.dll
             ${OPENCASCADE_DLL_DIR}/TKMath.dll
             ${OPENCASCADE_DLL_DIR}/TKernel.dll
             ${OPENCASCADE_DLL_DIR}/TKXSBase.dll
             ${OPENCASCADE_DLL_DIR}/TKMesh.dll
             ${OPENCASCADE_DLL_DIR}/../../../../tbb_2017.0.100/bin/intel64/vc14/tbb.dll
           )
        message( STATUS "OCCT_DLL_FILES ${OCCT_DLL_FILES}")
	    install( FILES ${OCCT_DLL_FILES} DESTINATION CloudCompare )
    endif()
		
	if (${OPTION_BUILD_CCVIEWER})
	  install( FILES ${QT_DLL_FILES} DESTINATION ccViewer ) 
	endif()

    install( DIRECTORY ${Qt5_DIR}/../../../plugins/ DESTINATION qtplugins )
  endif()
endif()

# --- plugins

if ( PLUGIN_STANDARD_QM3C2 )
#    add_subdirectory( M3C2 )
endif()
if ( PLUGIN_STANDARD_QPCV )
#    add_subdirectory( PCV )
endif()
if ( PLUGIN_STANDARD_QHPR )
#    add_subdirectory( HPR )
endif()
if ( PLUGIN_STANDARD_QMESH_BOOLEAN )
#    add_subdirectory( MeshBoolean )
endif()
if ( PLUGIN_STANDARD_QRANSAC_SD )
#    add_subdirectory( RANSAC_SD )
endif()
if ( PLUGIN_STANDARD_QPCL )
#    add_subdirectory( PCL )
endif()
if ( PLUGIN_STANDARD_QCSF )
#    add_subdirectory( CSF )
endif()



