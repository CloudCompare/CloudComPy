## docker-miniconda

Thanks to this [proposal](https://github.com/CloudCompare/CloudComPy/issues/10) from [arkanoid87](https://github.com/arkanoid87) you can try to use CloudComPy on Linux with miniconda docker images.

The original proposal is dedicated to automatic testing, but can be adapted to the common use of CloudComPy scripts. 
The docker technique is to be deepened for automatic tests, by adding the phase of construction from sources.
The use of docker images for the use of CloudComPy can also facilitate its portability.
I have not yet explored the possibility of configuring the docker image for graphical use.

Install

```
docker pull continuumio/miniconda3
```
Launch

```
docker run -i -t continuumio/miniconda3 /bin/bash
```

### Create a docker image from a  dockerfile :

The dockerfile is put in a file `Dockerfile_CloudComPy39_20211018` :


```
FROM continuumio/miniconda3:master

RUN . /opt/conda/etc/profile.d/conda.sh && \
    conda activate && \
    conda create --name CloudComPy39 python=3.9 && \
    conda activate CloudComPy39 && \
    conda config --add channels conda-forge && \
    conda config --set channel_priority strict && \
    conda install qt numpy psutil boost xerces-c pcl gdal cgal cmake pdal opencv ffmpeg mysql "qhull=2019.1" matplotlib "eigen=3.3.9" tbb openmp

RUN mkdir -p /opt/cloudcompy && \
    wget "https://www.simulation.openfields.fr/index.php/download-binaries/send/2-cloudcompy-binaries/8-cloudcompy-conda39-linux64-20211018-tgz" && \
    tar -xvzf "8-cloudcompy-conda39-linux64-20211018-tgz" -C /opt/cloudcompy && \
    rm "8-cloudcompy-conda39-linux64-20211018-tgz"

RUN apt-get update && apt-get install -y libgl1

RUN echo "#!/bin/bash\n\
\n\
. /opt/conda/etc/profile.d/conda.sh\n\
conda activate CloudComPy39\n\
export LD_LIBRARY_PATH=/opt/conda/envs/CloudComPy39/lib:\${LD_LIBRARY_PATH}\n\
export LD_LIBRARY_PATH=/opt/cloudcompy/installConda39/lib/cloudcompare:\${LD_LIBRARY_PATH}\n\
export LD_LIBRARY_PATH=/opt/cloudcompy/installConda39/lib/cloudcompare/plugins:\${LD_LIBRARY_PATH}\n\
export QT_QPA_PLATFORM=offscreen\n\
cd /opt/cloudcompy/installConda39/doc/PythonAPI_test\n\
ctest" > /entrypoint.sh && chmod +x /entrypoint.sh

ENTRYPOINT ["/entrypoint.sh"]
```

Build the docker image from the directory containing the the dockerfile `Dockerfile_CloudComPy39_20211018`. The docker image will be named `cloudcompy39_20211018` :

```
docker build -f Dockerfile_CloudComPy39_20211018 -t cloudcompy39_20211018 .
```

Execute the docker image in a container: execute ctest. The option `--rm` destroy the container after execution.


```
docker run -it --rm cloudcompy39_20211018 /bin/bash
```

Execute the docker image while remplacing the entrypoint:

```
docker run -it --rm --entrypoint="" cloudcompy39_20211018 /bin/bash
```

It is possible to set the CloudComPy environment and execute Python scripts:

```
conda activate CloudComPy39
CLOUDCOMPY_ROOT=/opt/cloudcompy/installConda39
export LD_LIBRARY_PATH=/opt/conda/envs/CloudComPy39/lib:${LD_LIBRARY_PATH}
export LD_LIBRARY_PATH=${CLOUDCOMPY_ROOT}/lib/cloudcompare:${LD_LIBRARY_PATH}
export LD_LIBRARY_PATH=${CLOUDCOMPY_ROOT}/lib/cloudcompare/plugins:${LD_LIBRARY_PATH}
export PYTHONPATH=${CLOUDCOMPY_ROOT}/lib/cloudcompare:${PYTHONPATH}
export PYTHONPATH=${CLOUDCOMPY_ROOT}/doc/PythonAPI_test:${PYTHONPATH}
export PATH=${CLOUDCOMPY_ROOT}/bin:${PATH}
export QT_QPA_PLATFORM=offscreen
```

To share a directory `~/CloudComPy/test` from the host (**warning**, files produced are natively `root`):


```
docker run -it --rm --entrypoint="" -v ~/CloudComPy/test:/root/CloudComPy cloudcompy39_20211018 /bin/bash
```

