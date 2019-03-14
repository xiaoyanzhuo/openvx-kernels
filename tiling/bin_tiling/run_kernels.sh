#!/usr/bin/env bash
#title          :run_kernels.sh
#description    :This script is used to run openvx tiling and non_tiling kernels automatically, 
#author         :Xiaoyan Zhuo <xiaoyanzhuo2@gmail.com>
#create date    :2.2.19
#latest update  :13.3.19
#version        :1.1    
#usage          :./run_kernels.sh <para1:file name to be executed> <para2: tile_blk_size1> <para3: tile_blk_size2>
#=================================================================================

name="_perf"
file=$1
tile_blk_size1=$2
tile_blk_size2=$3
tile_blk_size=$tile_blk_size1"x"$tile_blk_size2

if [[ -n $(find . -name '*tiling*.pgm') ]]; then
        rm $1_*.pgm
        echo "clean pgm files from previous output"
fi

sudo sh -c 'echo 1 >/proc/sys/vm/drop_caches'

echo "running "$file"..."
# perf stat -o "$file"$name"_"$tile_blk_size.txt -e cache-misses $file > "$file""_"$tile_blk_size.txt
perf stat -o "$file"$name"_"$tile_blk_size.txt -e instructions,cpu-cycles,cache-references,cache-misses,L1-dcache-loads,L1-dcache-load-misses,L1-dcache-stores,L1-dcache-prefetches,L1-dcache-prefetch-misses,L1-icache-loads,L1-icache-load-misses,L1-icache-prefetches,L1-icache-prefetch-misses $file > "$file""_"$tile_blk_size.txt
echo $file" done."
