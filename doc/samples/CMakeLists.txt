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

set( NOTEBOOK_SAMPLES
     histogramOnDistanceComputation.ipynb
    )

if ( LINUX )
    set( NOTEBOOK_SAMPLES_DIR cloudComPy/doc/samples )
else()
    if(BUILD_PYPI)
        set( NOTEBOOK_SAMPLES_DIR ${CMAKE_INSTALL_PREFIX}/cloudComPy/doc/samples )
    else()
        set( NOTEBOOK_SAMPLES_DIR doc/samples )
    endif()
endif()

install(PROGRAMS ${NOTEBOOK_SAMPLES}
        DESTINATION ${NOTEBOOK_SAMPLES_DIR}
       )
