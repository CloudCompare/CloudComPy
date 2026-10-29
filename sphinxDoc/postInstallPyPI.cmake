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

message( STATUS "post install process ...")
if (WIN32)
    message( STATUS "generate bundle and documentation (a few minutes, silent mode)...")
    execute_process( COMMAND sphinxDoc/genBundle.bat )
    execute_process( COMMAND sphinxDoc/genSphinxDoc.bat )
elseif( APPLE )
    execute_process( COMMAND pwd )
    message( STATUS "add libraries to bundle ...")
    execute_process( COMMAND python ${CMAKE_SOURCE_DIR}/CloudCompare/scripts/mac/bundle/libBundleCloudCompare.py ${CMAKE_INSTALL_PREFIX}/cloudComPy )
    message( STATUS "signature ...")
    # --- developper signature (CC_BUNDLE_SIGN) is set in .zshrc 
    execute_process( COMMAND python ${CMAKE_SOURCE_DIR}/CloudCompare/scripts/mac/bundle/signatureCloudCompare.py ${CMAKE_INSTALL_PREFIX}/cloudComPy )
    message( STATUS "generate documentation ...")
    execute_process( COMMAND chmod +x sphinxDoc/genSphinxDoc.zsh )
    execute_process( COMMAND zsh sphinxDoc/genSphinxDoc.zsh ERROR_QUIET )
else()
    execute_process( COMMAND pwd )
    # specific behavior for post install script, while builing wheel package on Linux
    message( STATUS "generate documentation ... ${CMAKE_BINARY_DIR}/build/${WHEEL_TAG}/sphinxDoc/genSphinxDoc.sh")
    #execute_process( COMMAND chmod +x ${CMAKE_BINARY_DIR}/build/${WHEEL_TAG}/sphinxDoc/genSphinxDoc.sh )
    #execute_process( COMMAND chmod +x ${CMAKE_BINARY_DIR}/build/${WHEEL_TAG}/sphinxDoc/genLinuxBundle.sh )
    execute_process( COMMAND bash ${CMAKE_BINARY_DIR}/build/${WHEEL_TAG}/sphinxDoc/genLinuxBundle.sh )
    execute_process( COMMAND bash ${CMAKE_BINARY_DIR}/build/${WHEEL_TAG}/sphinxDoc/genSphinxDoc.sh )
endif()
message( STATUS "... Done")
