#!/usr/bin/env python3
#title          :extract_perf_data.py
#description    :This script takes all perf output data (.txt files) in one directory with cache-miss and execution time information
#                output will contain: cache-miss.csv, exec_time.csv etc.
#author         :Xiaoyan Zhuo <xiaoyanzhuo2@gmail.com>
#create date    :2.2.19
#latest update  :14.3.19
#version        :1.2    
#usage          :python extract_perf_data.py <image_filename> <tile height>  (use python3 if needed)
#example        :python extract_perf_data.py lena_512x512.pgm 8 (using lena512x512 and tile height is 8)
#===================================================================================================

import glob, os, sys
import pandas as pd

# extract cache-miss and run time from perf output
def perf_values(file):
    with open (file, 'rt') as in_file:
        lines = in_file.readlines()      
    for line in lines:
        if "cache-misses" in line:
            if ',' in line.split()[0]:  # for number using thousand seperator ','
                cache_miss = int(line.split()[0].replace(',', ''))
            if '<not' in line.split()[0]:
                cache_miss = None
            else:
                cache_miss = int(line.split()[0])
        if "seconds time elapsed" in line:
            if ',' in line.split()[0]:
                exec_time = float(line.split()[0].replace(',', ''))
            if '<not' in line.split()[0]:
                exec_time = None
            else:
                exec_time = float(line.split()[0])
        if "instructions" in line:
            if ',' in line.split()[0]:  # for number using thousand seperator ','
                instruct = int(line.split()[0].replace(',', ''))
            if '<not' in line.split()[0]:
                instruct = None
            else:
                instruct = int(line.split()[0])
        if "cpu-cycles" in line:
            if ',' in line.split()[0]:  # for number using thousand seperator ','
                cpu_cycles = int(line.split()[0].replace(',', ''))
            if '<not' in line.split()[0]:
                cpu_cycles = None
            else:
                cpu_cycles = int(line.split()[0])
        if "cache-references" in line:
            if ',' in line.split()[0]:  # for number using thousand seperator ','
                cache_ref = int(line.split()[0].replace(',', ''))
            if '<not' in line.split()[0]:
                cache_ref = None
            else:
                cache_ref = int(line.split()[0])  
        if "L1-dcache-loads" in line:
            if ',' in line.split()[0]:  # for number using thousand seperator ','
                L1d_loads = int(line.split()[0].replace(',', ''))
            if '<not' in line.split()[0]:
                L1d_loads = None
            else:
                L1d_loads = int(line.split()[0]) 
        if "L1-dcache-load-misses" in line:
            if ',' in line.split()[0]:  # for number using thousand seperator ','
                L1d_load_miss = int(line.split()[0].replace(',', ''))
            if '<not' in line.split()[0]:
                L1d_load_miss = None
            else:
                L1d_load_miss = int(line.split()[0])
        if "L1-dcache-stores" in line:
            if ',' in line.split()[0]:  # for number using thousand seperator ','
                L1d_store = int(line.split()[0].replace(',', ''))
            if '<not' in line.split()[0]:
                L1d_store = None
            else:
                L1d_store = int(line.split()[0])
        if "L1-dcache-prefetch-misses" in line:
            if ',' in line.split()[0]:  # for number using thousand seperator ','
                L1d_prefe_miss = int(line.split()[0].replace(',', ''))
            if '<not' in line.split()[0]:
                L1d_prefe_miss = None
            else:
                L1d_prefe_miss = int(line.split()[0])
        if "L1-icache-loads" in line:
            if ',' in line.split()[0]:  # for number using thousand seperator ','
                L1i_load = int(line.split()[0].replace(',', ''))
            if '<not' in line.split()[0]:
                L1i_load = None
            else:
                L1i_load = int(line.split()[0])
        if "L1-icache-load-misses" in line:
            if ',' in line.split()[0]:  # for number using thousand seperator ','
                L1i_load_miss = int(line.split()[0].replace(',', ''))
            if '<not' in line.split()[0]:
                L1i_load_miss = None
            else:
                L1i_load_miss = int(line.split()[0])
    return cache_miss, exec_time, instruct, cpu_cycles, cache_ref, L1d_loads, L1d_load_miss, L1d_store, L1d_prefe_miss, L1i_load, L1i_load_miss

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

perf_data_instruct = {}
perf_data_cpu_cycles = {}
perf_data_cache_ref = {}
perf_data_L1d_loads = {}
perf_data_L1d_load_miss = {}
perf_data_L1d_store = {}
perf_data_L1d_prefe_miss = {}
perf_data_L1i_load = {}
perf_data_L1i_load_miss = {}

perf_data_instruct['app'] = app_set
perf_data_cpu_cycles['app'] = app_set
perf_data_cache_ref['app'] = app_set
perf_data_L1d_loads['app'] = app_set
perf_data_L1d_load_miss['app'] = app_set
perf_data_L1d_store['app'] = app_set
perf_data_L1d_prefe_miss['app'] = app_set
perf_data_L1i_load['app'] = app_set
perf_data_L1i_load_miss['app'] = app_set


app_size = len(app_set)

for file in glob.glob("./*_perf*.txt"):
    basename = os.path.splitext(os.path.basename(file))[0]
    if (basename.split('_')[0] == "non"):
        app_tmp = basename.split('_')[2]
        tile_size = "0_non-tile"           # non-tile, add 0, easier for sorting by column name
    if (basename.split('_')[0] == "tiling"):
        app_tmp = basename.split('_')[1]
        tile_size = basename.split('_')[3]

    # cache_miss, exec_time = perf_values(file)
    cache_miss, exec_time, instruct, cpu_cycles, cache_ref, L1d_loads, L1d_load_miss, L1d_store, L1d_prefe_miss, L1i_load, L1i_load_miss = perf_values(file)


    if tile_size not in perf_data_cache:
        perf_data_cache[tile_size] = [0]*app_size  # to use index to insert values 
    if tile_size not in perf_data_time:
        perf_data_time[tile_size] = [0]*app_size

    if tile_size not in perf_data_instruct:
        perf_data_instruct[tile_size] = [0]*app_size  # to use index to insert values 
    if tile_size not in perf_data_cpu_cycles:
        perf_data_cpu_cycles[tile_size] = [0]*app_size
    if tile_size not in perf_data_cache_ref:
        perf_data_cache_ref[tile_size] = [0]*app_size  # to use index to insert values 
    if tile_size not in perf_data_L1d_loads:
        perf_data_L1d_loads[tile_size] = [0]*app_size
    if tile_size not in perf_data_L1d_load_miss:
        perf_data_L1d_load_miss[tile_size] = [0]*app_size  # to use index to insert values 
    if tile_size not in perf_data_L1d_store:
        perf_data_L1d_store[tile_size] = [0]*app_size
    if tile_size not in perf_data_L1d_prefe_miss:
        perf_data_L1d_prefe_miss[tile_size] = [0]*app_size  # to use index to insert values 
    if tile_size not in perf_data_L1i_load:
        perf_data_L1i_load[tile_size] = [0]*app_size
    if tile_size not in perf_data_L1i_load_miss:
        perf_data_L1i_load_miss[tile_size] = [0]*app_size


    loc_cache = perf_data_cache['app'].index(app_tmp)  # locate specific row(match kernel) to add features and values
    perf_data_cache[tile_size][loc_cache] = cache_miss
    loc_time = perf_data_time['app'].index(app_tmp)
    perf_data_time[tile_size][loc_time] = exec_time

    loc_instruct = perf_data_instruct['app'].index(app_tmp)  # locate specific row(match kernel) to add features and values
    perf_data_instruct[tile_size][loc_instruct] = instruct
    loc_cpu_cycles = perf_data_cpu_cycles['app'].index(app_tmp)
    perf_data_cpu_cycles[tile_size][loc_cpu_cycles] = cpu_cycles

    loc_cache_ref = perf_data_cache_ref['app'].index(app_tmp)  # locate specific row(match kernel) to add features and values
    perf_data_cache_ref[tile_size][loc_cache_ref] = cache_ref
    loc_L1d_loads = perf_data_L1d_loads['app'].index(app_tmp)
    perf_data_L1d_loads[tile_size][loc_L1d_loads] = L1d_loads

    loc_L1d_load_miss = perf_data_L1d_load_miss['app'].index(app_tmp)  # locate specific row(match kernel) to add features and values
    perf_data_L1d_load_miss[tile_size][loc_L1d_load_miss] = L1d_load_miss
    loc_L1d_store = perf_data_L1d_store['app'].index(app_tmp)
    perf_data_L1d_store[tile_size][loc_L1d_store] = L1d_store

    loc_L1d_prefe_miss = perf_data_L1d_prefe_miss['app'].index(app_tmp)  # locate specific row(match kernel) to add features and values
    perf_data_L1d_prefe_miss[tile_size][loc_L1d_prefe_miss] = L1d_prefe_miss
    loc_L1i_load = perf_data_L1i_load['app'].index(app_tmp)
    perf_data_L1i_load[tile_size][loc_L1i_load] = L1i_load

    loc_L1i_load_miss = perf_data_L1i_load_miss['app'].index(app_tmp)  # locate specific row(match kernel) to add features and values
    perf_data_L1i_load_miss[tile_size][loc_L1i_load_miss] = L1i_load_miss


df_cache = pd.DataFrame(perf_data_cache, index = perf_data_cache['app'])  # dict -> dataframe
df_time = pd.DataFrame(perf_data_time, index = perf_data_time['app']) 
df_cache.set_index('app', inplace=True)   # use kernel list as index
df_time.set_index('app', inplace=True)
df_time = df_time.sort_index(axis=1)    # sort by cols name
df_cache = df_cache.sort_index(axis=1)

df_instruct = pd.DataFrame(perf_data_instruct, index = perf_data_instruct['app'])  # dict -> dataframe
df_cpu_cycles = pd.DataFrame(perf_data_cpu_cycles, index = perf_data_cpu_cycles['app']) 
df_instruct.set_index('app', inplace=True)   # use kernel list as index
df_cpu_cycles.set_index('app', inplace=True)
df_instruct = df_instruct.sort_index(axis=1)    # sort by cols name
df_cpu_cycles = df_cpu_cycles.sort_index(axis=1)

df_cache_ref = pd.DataFrame(perf_data_cache_ref, index = perf_data_cache_ref['app'])  # dict -> dataframe
df_L1d_loads = pd.DataFrame(perf_data_L1d_loads, index = perf_data_L1d_loads['app']) 
df_cache_ref.set_index('app', inplace=True)   # use kernel list as index
df_L1d_loads.set_index('app', inplace=True)
df_cache_ref = df_cache_ref.sort_index(axis=1)    # sort by cols name
df_L1d_loads = df_L1d_loads.sort_index(axis=1)

df_L1d_load_miss = pd.DataFrame(perf_data_L1d_load_miss, index = perf_data_L1d_load_miss['app'])  # dict -> dataframe
df_L1d_store = pd.DataFrame(perf_data_L1d_store, index = perf_data_L1d_store['app']) 
df_L1d_load_miss.set_index('app', inplace=True)   # use kernel list as index
df_L1d_store.set_index('app', inplace=True)
df_L1d_load_miss = df_L1d_load_miss.sort_index(axis=1)    # sort by cols name
df_L1d_store = df_L1d_store.sort_index(axis=1)

df_L1d_prefe_miss = pd.DataFrame(perf_data_L1d_prefe_miss, index = perf_data_L1d_prefe_miss['app'])  # dict -> dataframe
df_L1i_load = pd.DataFrame(perf_data_L1i_load, index = perf_data_L1i_load['app']) 
df_L1d_prefe_miss.set_index('app', inplace=True)   # use kernel list as index
df_L1i_load.set_index('app', inplace=True)
df_L1d_prefe_miss = df_L1d_prefe_miss.sort_index(axis=1)    # sort by cols name
df_L1i_load = df_L1i_load.sort_index(axis=1)

df_L1i_load_miss = pd.DataFrame(perf_data_L1i_load_miss, index = perf_data_L1i_load_miss['app'])  # dict -> dataframe
df_L1i_load_miss.set_index('app', inplace=True)   # use kernel list as index
df_L1i_load_miss = df_L1i_load_miss.sort_index(axis=1)    # sort by cols name


img_name = str(sys.argv[1]).split('.')[0]
tile_h = str(sys.argv[2])
df_time.to_csv("exec_time_%s_%s.csv" %(img_name, tile_h))
df_cache.to_csv("cache_miss_%s_%s.csv" %(img_name, tile_h))
df_instruct.to_csv("instruct_%s_%s.csv" %(img_name, tile_h))
df_cpu_cycles.to_csv("cpu_cycles_%s_%s.csv" %(img_name, tile_h))
df_cache_ref.to_csv("cache_ref_%s_%s.csv" %(img_name, tile_h))
df_L1d_loads.to_csv("L1d_loads_%s_%s.csv" %(img_name, tile_h))
df_L1d_load_miss.to_csv("L1d_load_miss_%s_%s.csv" %(img_name, tile_h))
df_L1d_store.to_csv("L1d_store_%s_%s.csv" %(img_name, tile_h))
df_L1d_prefe_miss.to_csv("L1d_prefe_miss_%s_%s.csv" %(img_name, tile_h))
df_L1i_load.to_csv("L1i_load_%s_%s.csv" %(img_name, tile_h))
df_L1i_load_miss.to_csv("L1i_load_miss_%s_%s.csv" %(img_name, tile_h))


