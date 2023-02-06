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

message(STATUS "Plugin HPR pybind11 processing")

# --- ----------------------------------------------------------
# --- target pybind11 library: Python module cloudComPy

project( _HPR VERSION 0.1 )

add_library( ${PROJECT_NAME} SHARED )

set_property(TARGET ${PROJECT_NAME} PROPERTY AUTOMOC ON)

    target_sources( ${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/HPRPy.cpp
    )

#qt5_wrap_ui( generated_ui_list ${CMAKE_CURRENT_SOURCE_DIR}/../../CloudCompare/plugins/core/Standard/qHPR/ui/qHPRDialog.ui )
#message(STATUS "generated_ui_list: ${generated_ui_list}")
#target_sources( ${PROJECT_NAME} PRIVATE ${generated_ui_list} )

target_include_directories( ${PROJECT_NAME} PRIVATE
    ${pybind11_INCLUDE_DIR}
    ${Python3_INCLUDE_DIRS}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../pybind11
    ${CMAKE_CURRENT_SOURCE_DIR}/../../pyAPI
    ${CMAKE_CURRENT_SOURCE_DIR}/../../CloudCompare/plugins/core/Standard/qHPR/include
    ${CCCoreLib_SOURCE_DIR}/include
    ${QCC_DB_LIB_SOURCE_DIR}
    )

target_link_libraries( ${PROJECT_NAME}
    PYCC_LIB
    CCAppCommon
    ${Python3_LIBRARIES}
    )

if ( UNIX )
    target_link_libraries( ${PROJECT_NAME}
        QHPR_PLUGIN
        )
endif()
if ( MSVC )
    target_link_libraries( ${PROJECT_NAME}
        QHPR_PLUGIN
        )
endif()

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

# --- Python scripts with added functions

set(PYTHONAPI_SCRIPTS
    __init__.py
    )

# --- install

if( WIN32 OR APPLE)
    install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION CloudCompare/cloudComPy/HPR )
    install(PROGRAMS ${PYTHONAPI_SCRIPTS} DESTINATION CloudCompare/cloudComPy/HPR )
else()
    set(INSTALL_CLOUDCOMPARE_PYTHON ${CMAKE_INSTALL_LIBDIR}/cloudcompare/cloudComPy/HPR)
    message(STATUS "INSTALL_CLOUDCOMPARE_PYTHON: " ${INSTALL_CLOUDCOMPARE_PYTHON})
    install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION ${INSTALL_CLOUDCOMPARE_PYTHON})
    install(PROGRAMS ${PYTHONAPI_SCRIPTS} DESTINATION ${INSTALL_CLOUDCOMPARE_PYTHON})
endif()

if(WIN32)
  set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".pyd" DEBUG_OUTPUT_NAME _HPR_d RELEASE_OUTPUT_NAME _HPR)
else()
  set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")
endif()

