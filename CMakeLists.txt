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

cmake_minimum_required(VERSION 3.17)


# Build CloudComPy for PyPI packages
option( BUILD_PYPI "Build CloudComPy for PyPI packages" OFF )

if ( BUILD_PYPI )
    # Scikit-build-core sets these values for you, or you can just hard-code the name and version.
    project(
    #${SKBUILD_PROJECT_NAME}
    #VERSION ${SKBUILD_PROJECT_VERSION}
    CloudComPy
    VERSION ${CLOUDCOMPY_VERSION}
    LANGUAGES CXX)
    if (UNIX AND NOT APPLE)
        # set architecture flag to x86-64 (and not native or x86-64-v2) for linux wheel (manylinux) compatibility
        # (since manylinux2014, see https://www.python.org/dev/peps/pep-0599/)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=x86-64")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=x86-64")
    endif()
else()
    project(
    CloudComPy
    VERSION ${CLOUDCOMPY_VERSION}
    LANGUAGES CXX)
endif()

# specify the C++ standard: C++14 required for boost python (use of variable templates for some pointer functions)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# --- Qt5 detection

find_package( Qt6
    COMPONENTS
        Concurrent
        Core
        Gui
        OpenGL
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

add_definitions(-D_CLOUDCOMPY_BUILD_)
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
	set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/cloudComPy" )
endif()

# scripts to set environment
if(WIN32)
    configure_file(activate_cloudcompy_env.bat.in ${CMAKE_CURRENT_BINARY_DIR}/activate_cloudcompy_env.bat @ONLY)
    set(ENV_CLOUDCOMPY envCloudComPy.bat checkenv.py ${CMAKE_CURRENT_BINARY_DIR}/activate_cloudcompy_env.bat)
    set (VSCODE_FILES .vscode/launch.json .vscode/settings.json)
    if ( BUILD_PYPI )
        install(PROGRAMS ${ENV_CLOUDCOMPY}
                DESTINATION "${CMAKE_INSTALL_PREFIX}/cloudComPy"
            )
        install(FILES ${VSCODE_FILES}
                DESTINATION "${CMAKE_INSTALL_PREFIX}/cloudComPy/.vscode"
            )
    else()
        install(PROGRAMS ${ENV_CLOUDCOMPY}
                DESTINATION .
            )
        install(FILES ${VSCODE_FILES}
                DESTINATION .vscode
            )
    endif()
elseif(APPLE)
    set(ENV_CLOUDCOMPY envCloudComPyMacOS.zsh checkenv.py)
    install(PROGRAMS ${ENV_CLOUDCOMPY}
            DESTINATION cloudComPy
           )
else()
    set(ENV_CLOUDCOMPY envCloudComPy.sh checkenv.py)
    install(PROGRAMS ${ENV_CLOUDCOMPY}
            DESTINATION cloudComPy
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
add_subdirectory( building )

# --- Sphinx doc, post install (must be in last position)

add_subdirectory( sphinxDoc )

set( README_DOCS
    README.md
    License.txt
    )

if ( BUILD_PYPI )
    install(FILES ${README_DOCS} DESTINATION ${CMAKE_INSTALL_PREFIX}/cloudComPy)
else()
    install(FILES ${README_DOCS} DESTINATION .)
endif()
    
