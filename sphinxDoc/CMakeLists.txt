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

message(STATUS "cloudComPy Sphinx doc generation")

# --- Sphinx Documentation

set(RSTFILES
    ccFacet.rst
    ccMesh.rst
    ccOctree.rst
    ccPointCloud.rst
    ccPolyline.rst
    ccPrimitives.rst
    cloudComPy.rst
    colors.rst
    DistanceComputationTools.rst
    index.rst
    introduction.rst
    ScalarField.rst
    )

set(SPHINXOPTS )
set(SOURCEDIR ${CMAKE_CURRENT_SOURCE_DIR})
set(SPHINXDOCTREES "${CMAKE_CURRENT_BINARY_DIR}/doctrees")
set(ALLSPHINXOPTS "${SPHINXOPTS} ${SOURCEDIR}")
set(SPHINXBUILD sphinx-build)

configure_file(conf.py.in conf.py)

if(WIN32)
    configure_file(genSphinxDocWindows.bat.in genSphinxDoc.bat)
elseif (USE_CONDA_PACKAGES)
    if(APPLE)
	    configure_file(genSphinxDocMacOSConda.zsh.in genSphinxDoc.zsh)
	    configure_file(libBundleCloudComPy.py.in libBundleCloudComPy.py)
	    configure_file(signatureCloudComPy.sh.in signatureCloudComPy.sh @ONLY )
    else()
	    configure_file(genSphinxDocConda.sh.in genSphinxDoc.sh)
    endif()
else()
    configure_file(genSphinxDoc.sh.in genSphinxDoc.sh)
endif()

add_custom_target(BUILD_HTML ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory doctrees
    DEPENDS ${RSTFILES} _cloudComPy
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

option( BUILD_REFERENCE_DOC "Build reference documentation for CloudComPy" OFF )

# --- Sphinx Documentation at install time

if ( BUILD_REFERENCE_DOC )
    message( STATUS "Sphinx documentation will be generated at install" )
    install( SCRIPT postInstall.cmake )
endif()

