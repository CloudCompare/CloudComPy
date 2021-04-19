
##########################################################################
#                                                                        #
#                               BoostPython                              #
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
#          Copyright 2021 Paul RASCLE www.openfields.fr                  #
#                                                                        #
##########################################################################

message(STATUS "Python3 and BoostPython processing")

# --- Python detection: if several versions installed, select one with all the required packages installed

set(PYTHON_PREFERED_VERSION "3.6" CACHE STRING "Select a Python version: it should be installed with all packages required for CloudCompare (pyqt, sip, numpy)")
set_property(CACHE PYTHON_PREFERED_VERSION PROPERTY STRINGS "3.6" "3.7" "3.8")
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
message( STATUS "Python3_VERSION_MAJOR: " ${Python3_VERSION_MAJOR} )
message( STATUS "Python3_VERSION_MINOR: " ${Python3_VERSION_MINOR} )
#find_package(Boost COMPONENTS python${Python3_VERSION_MAJOR}${Python3_VERSION_MINOR} numpy)
find_package(Boost 1.72.0 COMPONENTS python${Python3_VERSION_MAJOR}${Python3_VERSION_MINOR}  numpy${Python3_VERSION_MAJOR}${Python3_VERSION_MINOR} REQUIRED)

message(STATUS "Boost_LIBRARIES = ${Boost_LIBRARIES}")

# --- ----------------------------------------------------------
# --- target boost.Python library: Python module cloudComPy

project( cloudComPy VERSION 0.1 )

#Python3_add_library(${PROJECT_NAME} MODULE cloudComPy.cpp)

add_library( ${PROJECT_NAME} SHARED )

set_property(TARGET ${PROJECT_NAME} PROPERTY AUTOMOC ON)

    target_sources( ${PROJECT_NAME}
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/cloudComPy.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ScalarFieldPy.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ccPointCloudPy.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ccMeshPy.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ccPrimitivesPy.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ccPolylinePy.cpp
    ${CMAKE_CURRENT_LIST_DIR}/distanceComputationToolsPy.cpp
    )

target_include_directories( ${PROJECT_NAME} PUBLIC
    ${Boost_INCLUDE_DIRS}
    ${Python3_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../pyAPI
    ${CCCoreLib_SOURCE_DIR}/include
    ${QCC_DB_LIB_SOURCE_DIR}/include
    )

target_link_libraries( ${PROJECT_NAME}
    PYCC_LIB
    CCAppCommon
    ${Boost_LIBRARIES}
    ${Python3_LIBRARIES}
    )
#    ${Python3_LIBRARIES}
#set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-undefined dynamic_lookup")

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



if( WIN32 OR APPLE)
	install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION CloudCompare)
else()
    set(INSTALL_CLOUDCOMPARE_PYTHON ${CMAKE_INSTALL_LIBDIR}/cloudcompare)
    message(STATUS "INSTALL_CLOUDCOMPARE_PYTHON: " ${INSTALL_CLOUDCOMPARE_PYTHON})
    install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION ${INSTALL_CLOUDCOMPARE_PYTHON})
endif()

if(WIN32)
  set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".pyd" DEBUG_OUTPUT_NAME cloudComPy_d RELEASE_OUTPUT_NAME cloudComPy)
else()
  set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")
endif()
