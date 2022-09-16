@echo off
@set PREVPATH=%PATH%
@set PREVPYTHONPATH=%PYTHONPATH%
@set SCRIPT_DIR=%~dp0
@set CLOUDCOMPY_ROOT=%SCRIPT_DIR%..\..
@set PYTHONPATH=%CLOUDCOMPY_ROOT%\CloudCompare;%PYTHONPATH%
@set PYTHONPATH=%SCRIPT_DIR%;%PYTHONPATH%
@set PATH=.;%CLOUDCOMPY_ROOT%\CloudCompare;%CLOUDCOMPY_ROOT%\ccViewer;%SCRIPT_DIR%;%PATH%
python %1
set PythonError=%errorlevel%
echo "Python exit code = %PythonError%"
@set PYTHONPATH=%PREVPYTHONPATH%
@set PATH=%PREVPATH%
exit %PythonError%
