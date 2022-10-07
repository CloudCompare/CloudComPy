@echo off
@set SCRIPT_DIR=%~dp0
@set CLOUDCOMPY_ROOT=%SCRIPT_DIR%
@set PYTHONPATH=%CLOUDCOMPY_ROOT%\CloudCompare;%PYTHONPATH%
@set PYTHONPATH=%CLOUDCOMPY_ROOT%\doc\PythonAPI_test;%PYTHONPATH%
@set PATH=%CLOUDCOMPY_ROOT%\CloudCompare;%CLOUDCOMPY_ROOT%\ccViewer;%SCRIPT_DIR%;%PATH%
@set PATH=%CLOUDCOMPY_ROOT%\CloudCompare\plugins;%PATH%

2>NUL python %CLOUDCOMPY_ROOT%\checkenv.py || echo "Incorrect Environment! Problem with Python test!"
