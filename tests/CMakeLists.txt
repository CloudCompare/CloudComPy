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

message(STATUS "tests")

enable_testing()

# --- ------------
# --- target tests

set(PYTHONAPI_TEST_DIRECTORY "CloudComPyData" CACHE STRING "Directory path for tests to be created in your user directory, to store samples and generated data")
set(PYTHONAPI_EXTDATA_DIRECTORY "CloudComPyExternalData" CACHE STRING "Directory path for example files to be created in your user directory, to store downloaded external samples")
if(WIN32)
    set(USER_HOME "USERPROFILE")
else()
    set(USER_HOME "HOME")
endif()

# where to install the tests scripts
set(PYTHONAPI_TEST_SCRIPTS_DIRECTORY doc/PythonAPI_test)

# data sample generator and other utilities for test
configure_file(gendata.py.in gendata.py)
configure_file(DartConfiguration.tcl.in DartConfiguration.tcl)

# shell scripts to execute a test, a build or install step
if(WIN32)
    configure_file(execTest.bat.in execTest.bat)
    set(EXEC_TEST execTest.bat)
	set(EXEC_INSTALL_TEST execInstallTest.bat)
	set(ENV_INSTALL_TEST envPyCC.bat)
else()
    configure_file(execTest.sh.in execTest.sh)
    set(EXEC_TEST execTest.sh)
    set(EXEC_INSTALL_TEST execInstallTest.sh)
    set(ENV_INSTALL_TEST envPyCC.sh)
endif()

# list of tests
set(PYTHONAPI_TEST_SCRIPTS
    test001.py
    test002.py
    test003.py
    test004.py
    test005.py
    test006.py
    test007.py
    test008.py
    test009.py
    test010.py
    test011.py
    test012.py
    test013.py
    test014.py
    test015.py
    test016.py
    test017.py
    test018.py
    test019.py
    test020.py
    test021.py
    test022.py
    test023.py
    test024.py
    test025.py
    test026.py
    test027.py
    test028.py
    test029.py
    test030.py
    test031.py
    test032.py
    test033.py
    test034.py
    test035.py
    test036.py
    test037.py
    test038.py
    test039.py
    test040.py
    test041.py
    test042.py
    test043.py
    test044.py
    test045.py
    test046.py
    test047.py
    test048.py
    test049.py
    test050.py
    test051.py
    test052.py
    test053.py
    test054.py
    )

# list of utilities
set(PYTHONAPI_TEST_UTILITIES
    ${CMAKE_CURRENT_BINARY_DIR}/gendata.py
    ${CMAKE_CURRENT_BINARY_DIR}/DartConfiguration.tcl
    )

if(WIN32)
    install(PROGRAMS DartTestfile.win.txt
            DESTINATION ${PYTHONAPI_TEST_SCRIPTS_DIRECTORY}
            RENAME DartTestfile.txt
           )
else()
    install(PROGRAMS DartTestfile.txt
            DESTINATION ${PYTHONAPI_TEST_SCRIPTS_DIRECTORY}
           )
endif()

# files to install    
set(PYTHONAPI_TEST_FILES
    ${PYTHONAPI_TEST_UTILITIES}
    ${PYTHONAPI_TEST_SCRIPTS}
    )

install(PROGRAMS ${PYTHONAPI_TEST_FILES} ${ENV_INSTALL_TEST}
        DESTINATION ${PYTHONAPI_TEST_SCRIPTS_DIRECTORY}
       )

# the shell script executing a test is renamed a install step      
install(PROGRAMS ${EXEC_INSTALL_TEST}
        DESTINATION ${PYTHONAPI_TEST_SCRIPTS_DIRECTORY}
        RENAME ${EXEC_TEST}
       )

# --- define a function to simplify adding tests

function(do_test arg)
    add_test(NAME PYCC_${arg}
    COMMAND ${EXEC_TEST} ${arg}.py
    )
endfunction(do_test)

do_test(test001)
do_test(test002)
do_test(test003)
do_test(test004)
do_test(test005)
do_test(test006)
do_test(test007)
do_test(test008)
do_test(test009)
do_test(test010)
do_test(test011)
do_test(test012)
do_test(test013)
do_test(test014)
do_test(test015)
do_test(test016)
do_test(test017)
do_test(test018)
do_test(test019)
do_test(test020)
do_test(test021)
do_test(test022)
do_test(test023)
do_test(test024)
do_test(test025)
do_test(test026)
do_test(test027)
do_test(test028)
do_test(test029)
do_test(test030)
do_test(test031)
do_test(test032)
do_test(test033)
do_test(test034)
do_test(test035)
do_test(test036)
do_test(test037)
do_test(test038)
do_test(test039)
do_test(test040)
do_test(test041)
do_test(test042)
do_test(test043)
do_test(test044)
do_test(test045)
do_test(test046)
do_test(test047)
do_test(test048)
do_test(test049)
do_test(test050)
do_test(test051)
do_test(test052)
do_test(test053)

