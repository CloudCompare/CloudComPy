
## Build on Linux64, with Docker and Conda

The build procedure is very similar to the [procedure used on Ubuntu with conda](BuildLinuxConda.md), 
but uses a docker container which provides a better control of the environment.

This procedure has been tested on Ubuntu 20.04.

You must have [installed docker](https://docs.docker.com/engine/install/) on your computer.

Create a directory (for instance `dockerbuild`) for building the docker image and copy into it the script `genCloudComPy_Conda310_docker.sh`,
the Docker file `Dockerfile_CloudComPy310_build` from the the `building` directory of the CloudComPy repository.

Create also an empty file `noplugin` on docker image build directory (`dockerbuild`):

```
cd dockerbuild
touch noplugin
```
This empty file will be used by the Dockerfile script if you don't want to install plugins
that do not depend on the libraries provided with the conda packaging (Cork, FBX, MeshBoolean, Step).

If you want the FBX I/O plugin, download the 
[FBX SDK for Linux](https://www.autodesk.com/content/dam/autodesk/www/adn/fbx/2020-0-1/fbx202001_fbxsdk_linux.tar.gz) 
from [Autodesk](https://www.autodesk.com/developer-network/platform-technologies/fbx-sdk-2020-0), 
create a `fbxsdk` directory on the directory for the docker image build and detar completely the sdk into it 
(two steps, you have to accept the License). 

To build without optional plugins:

```
cd dockerbuild
docker build -f Dockerfile_CloudComPy310_build -t cloudcompy310_build .
```
To build with FBX SDK plugin:

```
cd dockerbuild
docker build --build-arg FBXINC=fbxsdk/include --build-arg FBXLIB=fbxsdk/lib -f Dockerfile_CloudComPy310_build -t cloudcompy310_build .
```

**Note**: Building with other optional plugins is not yet documented.

You have to run the docker image to execute the tests and get the binary tar file. 
Create an empty directory (for instance `~/CloudComPy/result`) on your host to share the results with the docker container.

```
mkdir -p ~/CloudComPy/result
cd ~/CloudComPy/result
docker run -it -v `pwd`:/root/CloudComPy cloudcompy310_build /bin/bash
```

from the container prompt:

```
./execTests.sh
/bin/cp /opt/installConda/CloudComPy_Conda310_Linux64_*.tgz /root/CloudComPy/
```

On your host, on `~/CloudCompy/result`, get the files from the tests and the CloudComPy binary tarfile.
This binary can be used in the same way as the linux binary available on the site 
[simulation.openfields.fr](https://www.simulation.openfields.fr/index.php/cloudcompy-downloads).

**Remark**: shared results are root, change to current user:

```
cd ~/CloudComPy/result
sudo chown -R `id -nu`:`id -ng` .
```




