#!/usr/bin/env bash
#title          :tile_blk_size.sh
#description    :This script is used to change the tile block size in vx_tiling_ext.c automatically and rebuild openvx env.
#author         :Xiaoyan Zhuo <xiaoyanzhuo2@gmail.com>
#date           :2.4.19
#version        :1    
#usage          :./python tile_blk_size.sh
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
echo "start recompling..."

cd ..
python Build.py --os=Linux --tiling --openmp --conf=Debug --rebuild=True --c=gcc --cpp=g++

echo "Finish recompling."



