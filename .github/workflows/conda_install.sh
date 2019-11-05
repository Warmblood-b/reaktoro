#!/bin/sh

wget -O miniconda.sh https://repo.continuum.io/miniconda/Miniconda3-latest-Linux-x86_64.sh
miniconda.sh -b -p $HOME/miniconda

export CONDA_BIN=$HOME/miniconda/bin
export CONDA=$CONDA_BIN/conda

$CONDA config --system --set always_yes yes --set changeps1 no
$CONDA config --system --append channels conda-forge
$CONDA install -n base conda-devenv
export "NUMBER_OF_COMPILATION_JOBS=4"
