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

message(STATUS "Plugin Canupo pybind11 processing")

# --- ----------------------------------------------------------
# --- target pybind11 library: Python module cloudComPy

project( _Canupo VERSION 0.1 )

add_library( ${PROJECT_NAME} MODULE )

if (WIN32)
    target_link_libraries( ${PROJECT_NAME}
	PUBLIC pybind11::module
    PRIVATE
    QCANUPO_PLUGIN
    PYCC_LIB
    CCAppCommon
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::OpenGL
    Qt6::PrintSupport
    )
else()
    target_link_libraries( ${PROJECT_NAME}
	PUBLIC pybind11::module
    QCANUPO_PLUGIN
    PYCC_LIB
    CCAppCommon
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::OpenGL
    Qt6::PrintSupport
    )
endif()

set_property(TARGET ${PROJECT_NAME} PROPERTY AUTOMOC ON)

target_sources( ${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/CanupoPy.cpp
    )

target_include_directories( ${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../pybind11
    ${CMAKE_CURRENT_SOURCE_DIR}/../../pyAPI
    ${CMAKE_CURRENT_SOURCE_DIR}/../../CloudCompare/plugins/core/Standard/qCanupo/include
    ${CCCoreLib_SOURCE_DIR}/include
    ${QCC_DB_LIB_SOURCE_DIR}
    )

pybind11_extension( ${PROJECT_NAME} )

if(NOT MSVC AND NOT ${CMAKE_BUILD_TYPE} MATCHES Debug|RelWithDebInfo)
    # Strip unnecessary sections of the binary on Linux/macOS
    pybind11_strip( ${PROJECT_NAME} )
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_VISIBILITY_PRESET "hidden"
                                      		  	 CUDA_VISIBILITY_PRESET "hidden")

# --- Python scripts with added functions

configure_file(__init__.py.in __init__.py)
set(PYTHONAPI_SCRIPTS
    ${CMAKE_CURRENT_BINARY_DIR}/__init__.py
    )

# --- install

if( WIN32 )
    if(BUILD_PYPI)
        install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/cloudComPy )
        install(PROGRAMS ${PYTHONAPI_SCRIPTS} DESTINATION ${CMAKE_INSTALL_PREFIX}/cloudComPy/Canupo )
    else()
        install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION CloudCompare )
        install(PROGRAMS ${PYTHONAPI_SCRIPTS} DESTINATION CloudCompare/cloudComPy/Canupo )
    endif()
elseif( APPLE )
    install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/cloudComPy/CloudCompare/CloudCompare.app/Contents/Frameworks )
    install(PROGRAMS ${PYTHONAPI_SCRIPTS} DESTINATION ${CMAKE_INSTALL_PREFIX}/cloudComPy/Canupo )
else()
    install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/cloudComPy )
    install(PROGRAMS ${PYTHONAPI_SCRIPTS} DESTINATION ${CMAKE_INSTALL_PREFIX}/cloudComPy/Canupo )
endif()
