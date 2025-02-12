#!/usr/bin/env bash
set -e

echo ">>> Installing system-level dependencies..."
# By default, the dev container runs as root, so you can apt-get install directly.
# If it doesn't, you may need 'sudo'.
apt-get update && apt-get install -y \
    build-essential \
    gdb \
    git \
    && rm -rf /var/lib/apt/lists/*

echo ">>> Creating and activating the 'numpy-dev' conda environment..."
conda create -y -n numpy-dev python=3.12
# We’re in a non-interactive script, so we can’t just 'conda activate' in the usual way.
# Instead, we temporarily source the Conda activation script so we can install packages.
source /usr/local/conda/etc/profile.d/conda.sh
conda activate numpy-dev

echo ">>> Installing baseline packages in numpy-dev..."
conda install -y pip wheel

# ----------------------
# OPTION 1: Install a specific release of NumPy
# If you just want to replicate an issue with a particular version (e.g., 1.25.0),
# you can do:
# conda install -y numpy=1.25.0
# Or if you truly want to replicate a "2.2.0" environment (if that existed), do so.
#
# Once installed, you can run your segfault test:
# python test_script.py
# ----------------------

# ----------------------
# OPTION 2: Build NumPy from source (debug mode)
# This is useful if you need to patch or step into the C code to see where the segfault occurs.
# We'll illustrate how to do that:
# ----------------------

echo ">>> Cloning NumPy from GitHub..."
if [ ! -d "/workspaces/numpy" ]; then
    git clone https://github.com/numpy/numpy.git /workspaces/numpy
fi

# (Optional) checkout a specific branch/tag if you need an older version
# cd /workspaces/numpy
# git checkout v1.25.0

echo ">>> Building NumPy in debug mode..."
cd /workspaces/numpy
pip install -r requirements.txt
# Build the in-place version of NumPy. --no-build-isolation + --no-use-pep517 can help if you need a lower-level build.
pip install -v -e . --no-build-isolation --no-use-pep517 --no-deps

# Note: 'pip install -e .' or 'python setup.py build' etc. 
# This step puts your local debug build into your numpy-dev environment.

echo ">>> Submodules, if you need them for your project..."
git submodule update --init

echo ">>> Done with setup.sh"

# TIP: If you want the new environment to be automatically activated each time 
# you open a new terminal, append a line to your ~/.bashrc:
echo "conda activate numpy-dev" >> ~/.bashrc
