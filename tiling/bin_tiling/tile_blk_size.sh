#!/usr/bin/env bash
#title          :tile_blk_size.sh
#description    :This script is used to change the tile block size in vx_tiling_ext.c automatically and rebuild openvx env.
#author         :Xiaoyan Zhuo <xiaoyanzhuo2@gmail.com>
#date           :2.4.19
#version        :1    
#usage          :./tile_blk_size.sh <tile_blk_size1> <tile_blk_size2> (./tile_blk_size.sh 4 4)
#=================================================================================

path_install="/home/firefly/build-openvx/openvx_sample/examples/"  # path need to be changed to where you build the openvx env
cd $path_install

tile_blk_size_1=$1
tile_blk_size_2=$2

echo "replace tile block size with: ""{"$tile_blk_size_1, $tile_blk_size_2"}"

sed -i "260s/.*/          {$tile_blk_size_1, $tile_blk_size_2},/" vx_tiling_ext.c  # line 260-gaussian
sed -i "274s/.*/          {$tile_blk_size_1, $tile_blk_size_2},/" vx_tiling_ext.c  # line 274-alpha
sed -i "287s/.*/          {$tile_blk_size_1, $tile_blk_size_2},/" vx_tiling_ext.c  # line 287-box
sed -i "301s/.*/          {$tile_blk_size_1, $tile_blk_size_2},/" vx_tiling_ext.c  # line 301-add

echo "Finish changing tile block size."
echo "Start recompling..."

cd ..
python Build.py --os=Linux --tiling --openmp --conf=Debug --rebuild=True --c=gcc --cpp=g++

echo "Finish recompling openvx."

path_kernels="/home/firefly/xyz/openvx-kernels/tiling" #path may vary, where you download the openvx_kernels repo.
cd $path_kernels
echo "Recompile kernels..."
make
echo "Finish recompiling, ready to run kernels."
