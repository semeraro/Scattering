#!/bin/bash
# Set environment for builds using Intel rendering foo
# change compiler to gcc and remove mpi stuff
module restore baregcc
# Set the TBB_DIR environment variable for cmake to use to locate tbb
export TBB_ROOT=$TACC_INTEL_DIR/tbb
export TBB_LIB=$TBB_ROOT/lib/intel64_lin/gcc4.8
# Explicitly place the tbb lib dir at the beginning of LD_LIBRARY_PATH 
# so that it is found before the one in /usr/lib
export LD_LIBRARY_PATH=$TBB_LIB:$LD_LIBRARY_PATH
# Set the open_vkl install directory
export openvkl_DIR=$WORK2/OpenVKL/buildgcc/install/lib64/cmake/openvkl-1.2.0
