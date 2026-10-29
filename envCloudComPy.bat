@echo off
@set SCRIPT_DIR=%~dp0
@set CLOUDCOMPY_ROOT=%SCRIPT_DIR%
@set PYTHONPATH=%CLOUDCOMPY_ROOT%\..;%PYTHONPATH%
@set PYTHONPATH=%CLOUDCOMPY_ROOT%;%PYTHONPATH%
@set PYTHONPATH=%CLOUDCOMPY_ROOT%\doc\PythonAPI_test;%PYTHONPATH%
@set PATH=%CLOUDCOMPY_ROOT%;%PATH%
@set PATH=%CLOUDCOMPY_ROOT%\plugins;%PATH%

if "%1"=="" (
    2>NUL python "%CLOUDCOMPY_ROOT%checkenv.py" || echo "Incorrect Environment! Problem with Python test!"
)