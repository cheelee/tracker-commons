#!/bin/bash
# PYTHON 3 STEPS FROM CLEAN UBUNTU AMI

date +"%Y-%m-%d_%H-%M-%S"

export LC_ALL=en_US.UTF-8
export LANG=en_US.UTF-8

# UPDATE SYSTEM
sudo apt-get -y update
sudo apt-get -y upgrade
sudo apt-get -y dist-upgrade
#sudo reboot

# INSTALL PYTHON AND LIBRARIES NEEDED BY WCON
PYTHON_VERSION=3.5
MINICONDA_DIR=~/miniconda3
wget http://repo.continuum.io/miniconda/Miniconda3-latest-Linux-x86_64.sh -O miniconda.sh
chmod +x miniconda.sh
./miniconda.sh -b
export PATH=$MINICONDA_DIR/bin:$PATH
conda install --yes python=$PYTHON_VERSION numpy scipy pandas jsonschema psutil

# INSTALL wcon.  Two options:

# 1. GET WCON FROM PYPI
#pip install wcon

# 2. ALTERNATIVELY: GET WCON FROM SOURCE
sudo apt-get install -y git
cd ~
mkdir github
cd github
git clone https://github.com/openworm/tracker-commons.git
cd tracker-commons/src/Python/tests
python tests.py

date +"%Y-%m-%d_%H-%M-%S"
