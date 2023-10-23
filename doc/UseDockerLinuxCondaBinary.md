## docker-miniconda (Work in Progress...)

**WARNING: This is not up to date and does not work as is. Take it as an example to adapt...**

Installation

```
docker pull continuumio/miniconda3
```
Lancement

```
docker run -i -t continuumio/miniconda3 /bin/bash
```

### Créer une image depuis un dockerfile :

Le dockerfile est dans un fichier nommé `Dockerfile_CloudComPy39_20211018` :


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
    wget "https://www.simulation.openfields.fr/phocadownload/cloudcompy_conda39_linux64_20211018.tgz" && \
    tar -xvzf "cloudcompy_conda39_linux64_20211018.tgz" -C /opt/cloudcompy && \
    rm "cloudcompy_conda39_linux64_20211018.tgz"

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

Construction de l'image, depuis le répertoire contenant le fichier `Dockerfile_CloudComPy39_20211018`. On la nomme `cloudcompy39_20211018` :

```
docker build -f Dockerfile_CloudComPy39_20211018 -t cloudcompy39_20211018 .
```

Execution de l'image dans un conteneur : déroule ctest. L'option --rm detruit le conteneur créé.


```
docker run -it --rm cloudcompy39_20211018 /bin/bash
```

Execution de l'image en surchargeant l'entrypoint :

```
docker run -it --rm --entrypoint="" cloudcompy39_20211018 /bin/bash
```

On peut charger un environnement, puis passer des scripts Python :

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

Répertoires partagés avec l'hote (les fichiers produits par le conteneur sont root):

```
docker run -it --rm --entrypoint="" -v ~/CloudComPy/test:/root/CloudComPy cloudcompy39_20211018 /bin/bash
```

Il n'y a pas de graphique à ce stade...

## Executer des applications graphiques (voire 3D) dans le container

```
xhost +local:root; \
docker run -it --rm \
-e DISPLAY=$DISPLAY \
-v /tmp/.X11-unix:/tmp/.X11-unix:rw \
 --entrypoint="" cloudcompy39_20211018 /bin/bash
```

```
export QT_QPA_PLATFORM=xcb
designer
```
Il reste des problèmes...
```
QStandardPaths: XDG_RUNTIME_DIR not set, defaulting to '/tmp/runtime-root'
libGL error: No matching fbConfigs or visuals found
libGL error: failed to load driver: swrast
```

Voir les solutions mettant en oeuvre vlc, webgl...


