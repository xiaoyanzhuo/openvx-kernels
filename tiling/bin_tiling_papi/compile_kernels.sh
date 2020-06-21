#!/usr/bin/env bash
path_kernels="/home/firefly/xyz/repo_test/openvx-kernels/tiling" #path may vary, where you download the openvx_kernels repo.
cd $path_kernels
echo "Recompile kernels..."
make
echo "Finish recompiling, ready to run kernels."

bin_path="/home/firefly/xyz/repo_test/openvx-kernels/tiling/bin_tiling_papi" # change dir back to bin dir
cd $bin_path

if [[ -n $(find . -name '*tiling*.pgm') ]]; then
        rm *tiling*.pgm
        echo "clean pgm files from previous output"
fi

sudo sh -c 'echo 1 >/proc/sys/vm/drop_caches'

