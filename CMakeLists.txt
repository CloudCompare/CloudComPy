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

cmake_minimum_required(VERSION 3.13)

project( CloudCompareProjects )

# specify the C++ standard: C++14 required for boost python (use of variable templates for some pointer functions)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# --- Qt5 detection

find_package( Qt5
    COMPONENTS
        Concurrent
        Core
        Gui
        OpenGL
        OpenGLExtensions
        PrintSupport
        Svg
        Widgets
    REQUIRED
)

#find_package( FLANN REQUIRED )

# Testing
option( BUILD_PY_TESTING "Build tests for CloudComPy" ON )
if ( BUILD_PY_TESTING )
	include( CTest )
endif()

option(PYTHONAPI_TRACES "Activate console trace in the Python API" OFF)
if (PYTHONAPI_TRACES)
    message(STATUS "Python Api traces activated")
    add_definitions(-D_PYTHONAPI_DEBUG_)
else()
    message(STATUS "Python Api traces deactivated")
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" )

# RPATH Linux/Unix: (dynamic) libs are put in a subdir of prefix/lib,
# since they are only used by qCC/ccViewer
if( UNIX AND NOT APPLE )
	include( GNUInstallDirs )
	set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_FULL_LIBDIR}/cloudcompare")
endif()

# scripts to set environment
if(WIN32)
	set(ENV_CLOUDCOMPY envCloudComPy.bat checkenv.py)
    install(PROGRAMS ${ENV_CLOUDCOMPY}
            DESTINATION .
           )
endif()

# --- wrap plugin options
if( PLUGIN_STANDARD_QM3C2 )
    message(STATUS "Plugin PLUGIN_STANDARD_QM3C2")
endif()

add_subdirectory( CloudCompare )
add_subdirectory( viewer )
add_subdirectory( pyAPI )
add_subdirectory( pybind11 )
add_subdirectory( tests )
add_subdirectory( doc )

# --- Sphinx doc, post install (must be in last position)

add_subdirectory( sphinxDoc )

set( README_DOCS
    README.md
    License.txt
    )
install(FILES ${README_DOCS} DESTINATION ${CMAKE_INSTALL_PREFIX})
    
