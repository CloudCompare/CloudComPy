# Common Principles Across All Platforms

The various scripts and configuration files used for compilation are located in the CloudComPy source code, primarily in the “building” directory or at the root level.

CloudCompare and its plugins depend on numerous prerequisite libraries (Qt6 and many others). We use a Conda environment to provide these libraries.
Each Conda environment (one for each operating system/Python version combination) is defined in a .yml file (CloudComPy3xxQt6_os.yml, where xx corresponds to the minor Python version, ranging from 10 to 14, and os refers to one of the following operating systems: macOS, Ubuntu, or Windows).

We also need a Python venv environment for the post-installation process: generating documentation, building and repairing the wheel file.

# Building the cloudComPy wheel file on Windows

The various Conda environments are created using the genCondaEnvs_Windows.bat file.

A PowerShell 7 script is used to configure, compile, install, build the wheel file, and test the software for a minor version of Python.

At the PowerShell 7 prompt (replace “xx” with the Python minor version between 10 and 14):

cd CloudComPy/building
conda activate CloudComPy3xx
.\build.ps1 -all -Pyminor=xx

# Building the cloudComPy wheel file on Linux

We use a virtual machine running Ubuntu 20.04 for the entire compilation process in order to use a minimal version of GLIBC. Due to the recent versions of the many prerequisite packages used, it is difficult to compile with an older distribution. Furthermore, the procedure for creating and repairing wheel packages limits GLIBC compatibility to newer distributions.

The various Conda environments are created using the genCondaEnvs_Ubuntu.sh script.

The allWheels_U2004.sh script is responsible for managing all wheel packages on Linux (configuration, compilation, installation, creation of wheel packages, and testing).

# Building the cloudComPy wheel file on macOS

The various Conda environments are created using the genCondaEnvs_macOS.sh script.

The genCloudComPy_Conda3xx_macos.zsh script takes an argument (from 0 to 4, corresponding to Python versions 3.10 through 3.14) and generates a wheel file. It handles all the steps, including signing and notarization.

