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

message( STATUS "post install process ...")
if (WIN32)
message( STATUS "generate documentation ...")
execute_process( COMMAND sphinxDoc\genSphinxDoc.bat )
elseif( APPLE )
execute_process( COMMAND pwd )
message( STATUS "add libraries to bundle ...")
execute_process( COMMAND python sphinxDoc/libBundleCloudComPy.py )
message( STATUS "signature ...")
execute_process( COMMAND bash sphinxDoc/signatureCloudComPy.sh )
message( STATUS "generate documentation ...")
execute_process( COMMAND chmod +x sphinxDoc/genSphinxDoc.zsh )
execute_process( COMMAND zsh sphinxDoc/genSphinxDoc.zsh ERROR_QUIET )
else()
execute_process( COMMAND pwd )
message( STATUS "generate documentation ...")
execute_process( COMMAND bash sphinxDoc/genSphinxDoc.sh )
endif()
message( STATUS "... Done")

