
============================
Introduction
============================

------------------------------
Set the CloudComPy environment
------------------------------

Before importing CloudComPy, you need to configure and load the required environment, 
mainly the PATH and PYTHONPATH used by the different executables, library and Python modules.

You should read the installation instructions in your CloudComPy installation, in the doc directory,
or on `GitHub <https://github.com/CloudCompare/CloudComPy#readme>`_.

Here, we suppose you are using a CloudComPy installation built on top of the Conda Packages.

Windows
~~~~~~~

The Windows Conda packaging provides a batch file to call after loading the conda environment, from the conda prompt:
::

    cd <path install>\CloudComPy310
    conda activate CloudComPy310
    envCloudComPy.bat


Linux
~~~~~

The LinuxConda packaging provides a shell script:
::

    cd <path install>/CloudComPy310
    . bin/condaCloud.sh activate CloudComPy310

macOS
~~~~~

If you use a conda environment, the shell script is similar to the Linux script:
::

    cd <path install>/CloudComPy310
    . bin/condaCloud.zsh activate CloudComPy310


--------------------------
Python3: CloudComPy import
--------------------------

The starting sequence is:
::

    import os
    os.environ["_CCTRACE_"]="ON"          # only if you want debug traces from C++
    import cloudComPy as cc               # import the CloudComPy module

It is no longer necessary to call ``initCC`` to discover all the CloudCompare plugins, this is done automatically at import.
::

    cc.initCC()  # done automatically at import, before using plugins
