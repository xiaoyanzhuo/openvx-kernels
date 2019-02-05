#!/usr/bin/env bash
#title          :run_kernels.sh
#description    :This script is used to run openvx tiling and non_tiling kernels automatically, 
#author         :Xiaoyan Zhuo <xiaoyanzhuo2@gmail.com>
#create date    :2.2.19
#latest update  :2.4.19
#version        :1.1    
#usage          :./run_kernels.sh <para1:file name to be executed> <para2: tile block size, like 4x4, 16x16, 128x128, etc>
#=================================================================================

name="_perf"
file=$1
tile_blk_size=$2

if [[ -n $(find . -name '*tiling*.pgm') ]]; then
        rm $1_*.pgm
        echo "clean pgm files from previous output"
fi

function clean_cache()
{
    fincore --pages=false --summarize --only-cached * > cache.txt
    cat cache.txt
    ref="total cached size: 0"
    echo "ref:"$ref
    while [[ $list != $ref ]]; do
        sudo sh -c 'echo 1 >/proc/sys/vm/drop_caches'
        fincore --pages=false --summarize --only-cached * > cache.txt
        cat cache.txt
        list=$(cat cache.txt | grep "total cached size: 0")
        echo "grep:"$list
        if [[ -z $list ]]; then
                echo "cache size is not 0 yet, cleaning..."
        fi
    done

    if [[ $list == $ref ]]; then
            echo "cache is clear."
            cat cache.txt  #for debugging
            return 0
    fi
}

#name="_perf"
#file=$1
#tile_blk_size=$2

if clean_cache; then
    echo "ready to run "$file
    echo "running "$file"..."
    perf stat -o "$file"$name"_"$tile_blk_size.txt -e cache-misses $file > "$file""_"$tile_blk_size.txt
    echo $file" done."
fi
