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

message(STATUS "Python API")

# --- -----------------------------------------
# --- target C++ library cc Python Api (pyCC)

project( PYCC_LIB VERSION 0.1 )

add_library( PYCC_LIB STATIC )
set_property(TARGET PYCC_LIB PROPERTY AUTOMOC ON)

if ( OPTION_SCALAR_DOUBLE )
	target_compile_definitions( PYCC_LIB PUBLIC SCALAR_TYPE_DOUBLE )
	message(STATUS "scalar type double")
else()
	target_compile_definitions( PYCC_LIB PUBLIC SCALAR_TYPE_FLOAT )
	message(STATUS "scalar type float")
endif()

configure_file(optdefines.h.in optdefines.h)

target_sources( PYCC_LIB
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/pyCC.h
    ${CMAKE_CURRENT_LIST_DIR}/pyccTrace.h
    ${CMAKE_CURRENT_LIST_DIR}/initCC.h
    PRIVATE
    pyCC.cpp
    initCC.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../CloudCompare/libs/CCAppCommon/src/ccPluginManager.cpp
    )
       
target_include_directories( PYCC_LIB PUBLIC
    ${CC_FBO_LIB_SOURCE_DIR}/include
    ${CCCoreLib_SOURCE_DIR}/include
    ${QCC_DB_LIB_SOURCE_DIR}
    )
    
if( MSVC )
   # TODO is this order of include relevant ? I use the same order as in qCC (CloudCompare target) 
   target_include_directories( PYCC_LIB PUBLIC
        ${QCC_DB_LIB_SOURCE_DIR}/msvc
        )
endif()

target_include_directories( PYCC_LIB PUBLIC
    ${QCC_IO_LIB_SOURCE_DIR}
    ${QCC_GL_LIB_SOURCE_DIR}
    ${CloudComparePlugins_SOURCE_DIR} 
    ${CloudCompare_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../CloudCompare/libs/CCAppCommon/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../CloudCompare/libs/CCPluginAPI/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../CloudCompare/libs/CCPluginStub/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../CloudCompare/libs/qCC_glWindow/include
    ${CMAKE_CURRENT_BINARY_DIR}
    )

target_link_libraries( PYCC_LIB
    CCAppCommon
    )

if( PLUGIN_IO_QFBX )
    target_include_directories( PYCC_LIB PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../CloudCompare/plugins/core/IO/qFBXIO/include
        )
    
    target_link_libraries( PYCC_LIB
        QFBX_IO_PLUGIN
        )
endif()

# Qt libraries

target_link_libraries( PYCC_LIB
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    )

