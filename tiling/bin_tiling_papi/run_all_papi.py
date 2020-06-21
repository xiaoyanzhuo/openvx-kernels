import subprocess

import json
import pandas as pd
import os
from pandas.io.json import json_normalize

def find_between( s, first, last ):
        try:
            start = s.index( first ) + len( first )
            end = s.index( last, start )
            return s[start:end]
        except ValueError:
            return ""

def read_papi_rec(path):
        file_name = os.listdir(path)[0]
        file_path = os.path.join(path, file_name)

        with open(str(file_path)) as json_file:
            rec = json.load(json_file)

        df = json_normalize(rec['threads'][0]['regions'])
        df_0 = df.iloc[0]
        return df_0

path = '/home/usrname/papi/output/papi_hl_output' # path/dir where papi output saved
if not os.path.exists(path):
    os.makedirs(path)

kernels = ["non_tiling_alpha", "non_tiling_gaussian", "non_tiling_box", "non_tiling_add", "non_tiling_erode", "non_tiling_dilate", "non_tiling_median", "non_tiling_sobel", "non_tiling_absdiff", "non_tiling_sub", "non_tiling_magnitude", "non_tiling_phase", "non_tiling_threshold", "non_tiling_accumulateSquare", "non_tiling_accumulate"]
images = ["lena_512x512.pgm", "lena_1024x1024.pgm", "lena_2048x2048.pgm", "lena_4096x4096.pgm"]

for iter_num in range(1,4):
    print("start iteration {}....".format(iter_num))
    dict_data = {}
    for image in images:
        image_width = int(find_between(image, "_", "x"))
        image_height = int(find_between(image, "x", "."))
        image_name = str(image).split('.')[0]
        subprocess.call("./change_image.sh {} {} {}".format(image, image_width, image_height), shell=True)
        print("change image size to {}x{}.\n".format(image_width, image_height))
        subprocess.call("./compile_kernels.sh", shell=True)
        for kernel in kernels:
            print("running kernel {}\n".format(kernel))
            subprocess.call("./dcache", shell=True)
            subprocess.call("./{}".format(kernel), shell=True)
            kernel_sub = str(kernel).split('_')[-1]
            dict_key = kernel_sub + '_' + str(image_width)
            df_0 = read_papi_rec(path)
            dict_data[dict_key] = df_0

    df_saved = pd.DataFrame.from_dict(dict_data, orient='index')
    df_saved.to_csv('papi_data_iter_{}.csv'.format(iter_num))
    print("iter {}: save papi data to csv file".format(iter_num))
    subprocess.call("mkdir ./output/papi_data_{}".format(iter_num), shell=True)
    subprocess.call("mv ./output/papi_hl_* ./output/papi_data_{}".format(iter_num), shell=True)

print("Job done.")

