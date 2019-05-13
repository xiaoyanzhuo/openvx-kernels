#!/usr/bin/env bash
#title          :tile_blk_size.sh
#description    :This script is used to change the tile block size in vx_tiling_ext.c automatically and rebuild openvx env.
#author         :Xiaoyan Zhuo <xiaoyanzhuo2@gmail.com>
#create date    :2.4.19
#lastest update :5.13.19
#version        :1.2    
#usage          :./tile_blk_size.sh <tile_blk_size1> <tile_blk_size2> <tile_blk_multiple> (./tile_blk_size.sh 4 4 32)
#=================================================================================

path_install="/home/firefly/build-openvx/openvx_sample/examples/"  # path need to be changed to where you build the openvx env
cd $path_install

tile_blk_size_1=$1
tile_blk_size_2=$2

echo "replace tile block size with: ""{"$tile_blk_size_1, $tile_blk_size_2"}"

sed -i "612s/.*/          {$tile_blk_size_1, $tile_blk_size_2},/" vx_tiling_ext.c  # line 260-gaussian(612)
sed -i "626s/.*/          {$tile_blk_size_1, $tile_blk_size_2},/" vx_tiling_ext.c  # line 274-alpha(626)
sed -i "639s/.*/          {$tile_blk_size_1, $tile_blk_size_2},/" vx_tiling_ext.c  # line 287-box(639)
sed -i "653s/.*/          {$tile_blk_size_1, $tile_blk_size_2},/" vx_tiling_ext.c  # line 301-add(653)
sed -i "666s/.*/          {$tile_blk_size_1, $tile_blk_size_2},/" vx_tiling_ext.c  # line 666-erode
sed -i "679s/.*/          {$tile_blk_size_1, $tile_blk_size_2},/" vx_tiling_ext.c  # line 679-dilate
sed -i "692s/.*/          {$tile_blk_size_1, $tile_blk_size_2},/" vx_tiling_ext.c  # line 692-median
sed -i "706s/.*/          {$tile_blk_size_1, $tile_blk_size_2},/" vx_tiling_ext.c  # line 706-sobel
sed -i "720s/.*/          {$tile_blk_size_1, $tile_blk_size_2},/" vx_tiling_ext.c  # line 720-absdiff
sed -i "734s/.*/          {$tile_blk_size_1, $tile_blk_size_2},/" vx_tiling_ext.c  # line 734-sub
sed -i "748s/.*/          {$tile_blk_size_1, $tile_blk_size_2},/" vx_tiling_ext.c  # line 748-magnitude
sed -i "762s/.*/          {$tile_blk_size_1, $tile_blk_size_2},/" vx_tiling_ext.c  # line 762-phase
sed -i "776s/.*/          {$tile_blk_size_1, $tile_blk_size_2},/" vx_tiling_ext.c  # line 776-threshold
sed -i "790s/.*/          {$tile_blk_size_1, $tile_blk_size_2},/" vx_tiling_ext.c  # line 790-accumulate squared
sed -i "803s/.*/          {$tile_blk_size_1, $tile_blk_size_2},/" vx_tiling_ext.c  # line 803-accumulate

echo "Finish changing tile block size."

cd ../sample/targets/c_model/ #path to change block multiple 

tile_blk_multiple=$3

echo "Replacing tile block multiple with: ""$tile_blk_multiple"

sed -i "429s/.*/    vx_uint32 block_multiple = $tile_blk_multiple;/" vx_interface.c # line 429 for block_multiple

echo "Finished changing tile block multiple."


echo "Start recompling..."

cd ../../../ # go back to the path for rebuild openvx env

python Build.py --os=Linux --tiling --openmp --conf=Debug --rebuild=True --c=gcc --cpp=g++

echo "Finish recompling openvx."

path_kernels="/home/firefly/xyz/repo_test/openvx-kernels/tiling" #path may vary, where you download the openvx_kernels repo.
cd $path_kernels
echo "Recompile kernels..."
make
echo "Finish recompiling, ready to run kernels."
