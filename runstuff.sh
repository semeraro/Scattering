#!/bin/bash
# run anariRenderTests tests
module load gcc
# make sure the tbb library is set correctly
export LD_LIBRARY_PATH=/opt/intel/compilers_and_libraries_2020.1.217/linux/tbb/lib/intel64_lin/gcc4.8:$LD_LIBRARY_PATH
#set the library to use
export ANARI_LIBRARY=ospray
# add the anari library path
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$WORK2/ANARI/Install/lib64
# add the anari-ospray path
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$WORK2/anari-ospray/buildgcc/install/lib64
# add the ospray library path
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$WORK2/OSPRay/buildgcc/install/lib64
# addn the openvkl library path
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$WORK2/OpenVKL/buildgcc/install/lib64
Install/bin/anariRenderTests 
Install/bin/anariTutorialCpp
