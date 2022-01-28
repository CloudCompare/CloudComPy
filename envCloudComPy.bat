rem this script sets the environment for CloudComPy. The Conda environment must be activated before calling this script.

@echo off
@set SCRIPT_DIR=%~dp0
@set CLOUDCOMPY_ROOT=%SCRIPT_DIR%
@set QT_PLUGIN_PATH=%CLOUDCOMPY_ROOT%\qtplugins
@set PYTHONPATH=%CLOUDCOMPY_ROOT%\CloudCompare;%PYTHONPATH%
@set PYTHONPATH=%CLOUDCOMPY_ROOT%\doc\PythonAPI_test;%PYTHONPATH%
@set PATH=%CLOUDCOMPY_ROOT%\CloudCompare;%CLOUDCOMPY_ROOT%\ccViewer;%SCRIPT_DIR%;%PATH%
@set PATH=%CLOUDCOMPY_ROOT%\CloudCompare\plugins;%PATH%
