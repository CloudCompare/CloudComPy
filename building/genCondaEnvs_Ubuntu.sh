echo "# --- build conda environment ---"

cd $HOME/projets/CloudComPy/CloudComPy/building

conda install -y -n base mamba -c conda-forge
conda update -y -n base -c defaults conda
conda config --set solver libmamba
conda config --set channel_priority strict

export PYMINVER="10"                                                                           # Python minor version
export CONDA_ENV=CloudComPy3${PYMINVER}                                                # conda environment name
conda init && \
conda activate && \
mamba create -n CloudComPy3${PYMINVER} -y && \
mamba env update -y -n CloudComPy3${PYMINVER} -f CloudComPy3${PYMINVER}Qt6_Ubuntu.yml && \
conda activate ${CONDA_ENV} || error_exit "conda environment ${CONDA_ENV} cannot be built"
conda list > conda-list_Ubuntu20.04_3${PYMINVER}

export PYMINVER="11"                                                                           # Python minor version
export CONDA_ENV=CloudComPy3${PYMINVER}                                                # conda environment name
conda init && \
conda activate && \
mamba create -n CloudComPy3${PYMINVER} -y && \
mamba env update -y -n CloudComPy3${PYMINVER} -f CloudComPy3${PYMINVER}Qt6_Ubuntu.yml && \
conda activate ${CONDA_ENV} || error_exit "conda environment ${CONDA_ENV} cannot be built"
conda list > conda-list_Ubuntu20.04_3${PYMINVER}

export PYMINVER="12"                                                                           # Python minor version
export CONDA_ENV=CloudComPy3${PYMINVER}                                                # conda environment name
conda init && \
conda activate && \
mamba create -n CloudComPy3${PYMINVER} -y && \
mamba env update -y -n CloudComPy3${PYMINVER} -f CloudComPy3${PYMINVER}Qt6_Ubuntu.yml && \
conda activate ${CONDA_ENV} || error_exit "conda environment ${CONDA_ENV} cannot be built"
conda list > conda-list_Ubuntu20.04_3${PYMINVER}

export PYMINVER="13"                                                                           # Python minor version
export CONDA_ENV=CloudComPy3${PYMINVER}                                                # conda environment name
conda init && \
conda activate && \
mamba create -n CloudComPy3${PYMINVER} -y && \
mamba env update -y -n CloudComPy3${PYMINVER} -f CloudComPy3${PYMINVER}Qt6_Ubuntu.yml && \
conda activate ${CONDA_ENV} || error_exit "conda environment ${CONDA_ENV} cannot be built"
conda list > conda-list_Ubuntu20.04_3${PYMINVER}

export PYMINVER="14"                                                                           # Python minor version
export CONDA_ENV=CloudComPy3${PYMINVER}                                                # conda environment name
conda init && \
conda activate && \
mamba create -n CloudComPy3${PYMINVER} -y && \
mamba env update -y -n CloudComPy3${PYMINVER} -f CloudComPy3${PYMINVER}Qt6_Ubuntu.yml && \
conda activate ${CONDA_ENV} || error_exit "conda environment ${CONDA_ENV} cannot be built"
conda list > conda-list_Ubuntu20.04_3${PYMINVER}


