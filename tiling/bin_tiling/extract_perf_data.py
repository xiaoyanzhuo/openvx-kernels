#!/usr/bin/env python3
#title          :extract_perf_data.py
#description    :This script takes all perf output data (.txt files) in one directory with cache-miss and execution time information
#                and output will contain cache-miss.csv and exec_time.csv.
#author         :Xiaoyan Zhuo <xiaoyanzhuo2@gmail.com>
#date           :2.2.19
#version        :1    
#usage          :python extract_perf_data.py (use python3 if needed)
#=================================================================================

import glob, os
import pandas as pd

# extract cache-miss and run time from perf output
def perf_values(file):
    with open (file, 'rt') as in_file:
        lines = in_file.readlines()      
    for line in lines:
        if "cache-misses" in line:
            cache_miss = int(line.split()[0])
        if "seconds time elapsed" in line:
            exec_time = float(line.split()[0])
    return cache_miss, exec_time

# get kernel list
app_list = []
for file in glob.glob("./tiling_*_perf_*.txt"):
    basename = os.path.splitext(os.path.basename(file))[0]
    app_tmp = basename.split('_')[1]
    app_list.append(app_tmp)
app_set = set(app_list)
app_set = sorted(app_set)

# dict to store raw perf data 
perf_data_cache = {}
perf_data_time = {}
perf_data_cache['app'] = app_set
perf_data_time['app'] = app_set
app_size = len(app_set)

for file in glob.glob("./*_perf*.txt"):
    basename = os.path.splitext(os.path.basename(file))[0]
    if (basename.split('_')[0] == "non"):
        app_tmp = basename.split('_')[2]
        tile_size = "0_non-tile"           # non-tile, add 0, easier for sorting by column name
    if (basename.split('_')[0] == "tiling"):
        app_tmp = basename.split('_')[1]
        tile_size = basename.split('_')[3]

    cache_miss, exec_time = perf_values(file)

    if tile_size not in perf_data_cache:
        perf_data_cache[tile_size] = [0]*app_size  # to use index to insert values 
    if tile_size not in perf_data_time:
        perf_data_time[tile_size] = [0]*app_size

    loc_cache = perf_data_cache['app'].index(app_tmp)  # locate specific row(match kernel) to add features and values
    perf_data_cache[tile_size][loc_cache] = cache_miss
    loc_time = perf_data_time['app'].index(app_tmp)
    perf_data_time[tile_size][loc_time] = exec_time

df_cache = pd.DataFrame(perf_data_cache, index = perf_data_cache['app'])  # dict -> dataframe
df_time = pd.DataFrame(perf_data_time, index = perf_data_time['app']) 
df_cache.set_index('app', inplace=True)   # use kernel list as index
df_time.set_index('app', inplace=True)
df_time = df_time.sort_index(axis=1)    # sort by cols name
df_cache = df_cache.sort_index(axis=1)

df_time.to_csv('exec_time.csv')
df_cache.to_csv('cache_miss.csv')
