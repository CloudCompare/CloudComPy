##########################################################################
#                                                                        #
#                             CLOUDCOMPY                                 #
#                                                                        #
#  This program is free software; you can redistribute it and/or modify  #
#  it under the terms of the GNU Library General Public License as       #
#  published by the Free Software Foundation; version 2 or later of the  #
#  License.                                                              #
#                                                                        #
#  This program is distributed in the hope that it will be useful,       #
#  but WITHOUT ANY WARRANTY; without even the implied warranty of        #
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the          #
#  GNU General Public License for more details.                          #
#                                                                        #
#          Copyright 2020 Paul RASCLE www.openfields.fr                  #
#                                                                        #
##########################################################################

cmake_minimum_required(VERSION 3.10)

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

# Testing
option( BUILD_PY_TESTING "Build tests for CloudComPy" OFF )
if ( BUILD_PY_TESTING )
	include( CTest )
endif()

option(PYTHONAPI_TRACES "Activate console trace in the Python API" OFF)
if (PYTHONAPI_TRACES)
    add_definitions(-D_PYTHONAPI_DEBUG_)
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" )

# RPATH Linux/Unix: (dynamic) libs are put in a subdir of prefix/lib,
# since they are only used by qCC/ccViewer
if( UNIX AND NOT APPLE )
	include( GNUInstallDirs )
	set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_FULL_LIBDIR}/cloudcompare")
endif()

add_subdirectory( CloudCompare )
add_subdirectory( pyAPI )
add_subdirectory( boostPython )
add_subdirectory( sip )
add_subdirectory( tests )
