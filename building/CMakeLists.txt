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

if(WIN32)
        set( BUILDING_DOCS
            conda-list_Windows11_conda3${PYMINOR}
            cpy312.yml
            )
elseif (USE_CONDA_PACKAGES)
    if(APPLE)
        set( BUILDING_DOCS
            conda-list_macOS_3${PYMINOR}
            )
    else()
        set( BUILDING_DOCS
            conda-list_Ubuntu20.04_3${PYMINOR}
            )
    endif()
endif()

if ( LINUX )
    install(FILES ${BUILDING_DOCS} DESTINATION cloudComPy/doc)
else()
    if (BUILD_PYPI)
        install(FILES ${BUILDING_DOCS} DESTINATION ${CMAKE_INSTALL_PREFIX}/cloudComPy/doc)
    else()
        install(FILES ${BUILDING_DOCS} DESTINATION doc)
    endif()
endif()