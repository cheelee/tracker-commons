#!/bin/bash
# Pre-requisite - run python-install.sh
PYTHON_VERSION=3.5
MINICONDA_DIR=~/miniconda3

export LC_ALL=en_US.UTF-8
export LANG=en_US.UTF-8

date +"%Y-%m-%d_%H-%M-%S"
sudo apt-get install -y octave
date +"%Y-%m-%d_%H-%M-%S"
# liboctave-dev is required for mkoctfile
sudo apt-get install -y liboctave-dev
sudo apt-get install -y swig
sudo apt-get install -y g++
sudo apt-get install -y make
export PATH=$MINICONDA_DIR/bin:$PATH
# Library paths to /lib/x86_64-linux-gnu and /usr/lib/x86_64-linux-gnu
#   are because the libgfortran.so.3 and libreadline.so.6 files installed
#   by miniconda3 are incompatible with the ones expected by swig and
#   octave.
export LD_LIBRARY_PATH=/lib/x86_64-linux-gnu:/usr/lib/x86_64-linux-gnu:$MINICONDA_DIR/lib:$LD_LIBRARY_PATH
cd ~/github/tracker-commons/src/octave/wrappers
make
cat >> ~/.profile <<EOF
export PATH=$MINICONDA_DIR/bin:\$PATH
export LD_LIBRARY_PATH=/lib/x86_64-linux-gnu:/usr/lib/x86_64-linux-gnu:$MINICONDA_DIR/lib:\$LD_LIBRARY_PATH
export LC_ALL=en_US.UTF-8
export LANG=en_US.UTF-8
EOF
echo "Please run '. ~/.profile' before proceeding with testing."
# Copying the root schema to the appropriate position in Python for now.
# Else things don't work and WCON-Octave isn't happy
cp ~/github/tracker-commons/wcon_schema.json ~/github/tracker-commons/src/Python/wcon
# But we can proceed to test here because the variables are already set
date +"%Y-%m-%d_%H-%M-%S"
octave driver.m
date +"%Y-%m-%d_%H-%M-%S"
