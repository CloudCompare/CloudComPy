## docker-miniconda

**WARNING: This is not up to date and does not work as is. Take it as an example to adapt...**

Thanks to this [proposal](https://github.com/CloudCompare/CloudComPy/issues/10) from [arkanoid87](https://github.com/arkanoid87) 
you can try to use CloudComPy on Linux with miniconda docker images.

The original proposal is dedicated to automatic tests, but one can consider several use cases for docker:


 1. build a Linux binary of CloudComPy and run CloudComPy's automatic tests. 

 2. provide a CloudComPy execution environment (bash prompt to launch CloudComPy scripts, without graphical environment)

 3. provide an execution environment for CloudComPy and CloudCompare (bash prompt to launch CloudComPy, CloudCompare scripts, 
 with a graphical environment, using your host GPU for 3D rendering)

In all cases, the process of building the binary from the sources is described in [building with Docker](BuildLinuxCondaDocker.md)).

For the last case (3D graphics), with an Nvidia GPU, the NVIDIA Container Toolkit allows users to build and run 
[GPU accelerated containers](https://docs.nvidia.com/datacenter/cloud-native/container-toolkit/overview.html).

------------------------------------
### chapter to be redesigned. 
Install

```
docker pull continuumio/miniconda3
```
Launch

```
docker run -i -t continuumio/miniconda3 /bin/bash
```

#### Create a docker image from a dockerfile :

The dockerfile is put in a file `Dockerfile_CloudComPy39_20211208` :


```
FROM continuumio/miniconda3:master

RUN . /opt/conda/etc/profile.d/conda.sh && \
    conda activate && \
    conda create --name CloudComPy39 python=3.9 && \
    conda activate CloudComPy39 && \
    conda config --add channels conda-forge && \
    conda config --set channel_priority strict && \
    conda install boost cgal cmake eigen ffmpeg gdal jupyterlab matplotlib mysql numpy opencv openmp pcl pdal psutil "qhull=2019.1" qt scipy sphinx_rtd_theme spyder tbb tbb-devel xerces-c

RUN mkdir -p /opt/cloudcompy && \
    wget "https://www.simulation.openfields.fr/phocadownload/cloudcompy_conda39_linux64_20211208.tgz" && \
    tar -xvzf "cloudcompy_conda39_linux64_20211208.tgz" -C /opt/cloudcompy && \
    rm "cloudcompy_conda39_linux64_20211208.tgz"

RUN apt-get update && apt-get install -y libgl1 libomp5

RUN echo "#!/bin/bash\n\
. /opt/conda/etc/profile.d/conda.sh\n\
cd /opt/cloudcompy/CloudComPy39\n\
. bin/condaCloud.sh activate CloudComPy39\n\
export QT_QPA_PLATFORM=offscreen\n\
cd /opt/cloudcompy/CloudComPy39/doc/PythonAPI_test\n\
ctest" > /entrypoint.sh && chmod +x /entrypoint.sh

ENTRYPOINT ["/entrypoint.sh"]
```

Build the docker image from the directory containing the the dockerfile `Dockerfile_CloudComPy39_20211208`. The docker image will be named `cloudcompy39_20211208` :

```
docker build -f Dockerfile_CloudComPy39_20211208 -t cloudcompy39_20211208 .
```

Execute the docker image in a container: it will execute ctest. The option `--rm` destroys the container after execution.


```
docker run -it --rm cloudcompy39_20211208 /bin/bash
```

Execute the docker image while remplacing the entrypoint:

```
docker run -it --rm --entrypoint="" cloudcompy39_20211208 /bin/bash
```

It is possible to set the CloudComPy environment and execute Python scripts:

```
cd /opt/cloudcompy/CloudComPy39/
. bin/condaCloud.sh activate CloudComPy39
export QT_QPA_PLATFORM=offscreen
cd doc/PythonAPI_test/
python test001.py
```

To share an existing directory `~/CloudComPy/test` from the host (**warning**, files produced are natively `root`):


```
docker run -it --rm --entrypoint="" -v ~/CloudComPy/test:/root/CloudComPy cloudcompy39_20211208 /bin/bash
```

Running ctest as above will create a visible Data directory in the host system in `~/CloudComPy/test`.

```
cd /opt/cloudcompy/CloudComPy39/
. bin/condaCloud.sh activate CloudComPy39
export QT_QPA_PLATFORM=offscreen
cd doc/PythonAPI_test/
ctest
```

