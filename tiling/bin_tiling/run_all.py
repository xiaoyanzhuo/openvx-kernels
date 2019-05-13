import subprocess

def find_between( s, first, last ):
        try:
            start = s.index( first ) + len( first )
            end = s.index( last, start )
            return s[start:end]
        except ValueError:
            return ""

kernels = ["tiling_alpha", "tiling_gaussian", "tiling_box", "tiling_add", "tiling_erode", "tiling_dilate", "tiling_median", "tiling_sobel", "tiling_absdiff", "tiling_sub", "tiling_magnitude", "tiling_phase", "tiling_threshold", "tiling_accumulateSquare", "tiling_accumulate"]
images = ["lena_512x512.pgm", "lena_1024x1024.pgm", "lena_2048x2048.pgm", "lena_4096x4096.pgm"]

max_tile_height = 64
#max_tiling_size = 2048
for iter_num in range(6):
    for image in images:
        image_width = int(find_between(image, "_", "x"))
        image_height = int(find_between(image, "x", "."))
        image_name = str(image).split('.')[0]
        subprocess.call("./change_image.sh {} {} {}".format(image, image_width, image_height), shell=True)
        tile_height = 1
        while tile_height < max_tile_height + 1:
            t_size = 1
            while t_size < tile_height + 1:
                block_multiple = image_height / tile_height
                subprocess.call("./tile_blk_size.sh {} {} {}".format(t_size, t_size, block_multiple), shell=True, executable='/bin/bash')
                for kernel in kernels:
                    subprocess.call("./dcache", shell=True)
                    subprocess.call("./run_kernels.sh ./{} {} {}".format(kernel, t_size, t_size), shell=True, executable='/bin/bash')
                t_size *= 2
            subprocess.call("python3 extract_perf_data.py {} {}".format(image, tile_height), shell=True)
            subprocess.call("mkdir csv_{}_{}".format(image_name, tile_height), shell=True)    # more than 10 types of perf data
            subprocess.call("mv *.csv csv_{}_{}/".format(image_name, tile_height), shell=True)
            subprocess.call("mkdir txt_{}_{}".format(image_name, tile_height), shell=True)  # txt files as backup
            subprocess.call("mv *.txt txt_{}_{}/".format(image_name, tile_height), shell=True)
            tile_height *= 2
    subprocess.call("mkdir res_{}".format(iter_num), shell=True)
    subprocess.call("mv csv_* res_{}".format(iter_num), shell=True)
    subprocess.call("mv txt_* res_{}".format(iter_num), shell=True)
