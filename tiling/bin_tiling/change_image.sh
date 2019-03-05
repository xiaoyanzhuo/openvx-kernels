#!/usr/bin/env bash
#title          :change_image.sh
#description    :This script is used to change the image file and related size in tiling*.c and non_tiling*.c automatically.
#author         :Xiaoyan Zhuo <xiaoyanzhuo2@gmail.com>
#create date    :2.13.19
#lastest update :3.1.19
#version        :1.1    
#usage          :./change_image.sh <image_file_name> <image_width> <image_height> (./change_image.sh lena_1024x1024.pgm 1024 1024)
#=================================================================================
img_orig=$1
img_w=$2
img_h=$3
img_in=\"$1\"
echo "input image: "$img_in
echo "image width: "$img_w
echo "input height: "$img_h

for file in $(find "$(cd ../examples_tiling; pwd)" -name 'tiling*.c' -o -name 'non_tiling*.c'); do
        echo "start changing image and size info in:" $file
        fname=$(basename "$file")
        fname_no_ext=${fname%.*}
        img_out=\"$fname_no_ext"_"$img_orig\"
        echo "output image: "$img_out
        sed -i "/.*#define img_w.*/c #define img_w $2" $file
        sed -i "/.*#define img_h.*/c #define img_h $3" $file
        if [ $fname == "tiling_add.c" ]; then
        	img_in2=\""tiling_alpha_"$img_orig\"
		echo "another input image: "$img_in2" execute tiling_alpha first to obtain it."
                n1=$(awk '/vxFReadImageNode/{++n; if (n==1) { print NR; exit}}' $file)
                n2=$(awk '/vxFReadImageNode/{++n; if (n==2) { print NR; exit}}' $file)
                sed -i "${n1}s/.*/                    vxFReadImageNode(graph, $img_in, images[1]),/" $file
                sed -i "${n2}s/.*/                    vxFReadImageNode(graph, $img_in2, images[2]),/" $file
                sed -i "/.*vxFWriteImageNode.*/c \                    vxFWriteImageNode(graph, images[3], $img_out)," $file
        elif [ $fname == "non_tiling_add.c" ]; then
        	img_in2=\""non_tiling_alpha_"$img_orig\"
		echo "another input image: "$img_in2" execute non_tiling_alpha first to obtain it."
                n1=$(awk '/vxFReadImageNode/{++n; if (n==1) { print NR; exit}}' $file)
                n2=$(awk '/vxFReadImageNode/{++n; if (n==2) { print NR; exit}}' $file)
                sed -i "${n1}s/.*/                    vxFReadImageNode(graph, $img_in, images[1]),/" $file
                sed -i "${n2}s/.*/                    vxFReadImageNode(graph, $img_in2, images[2]),/" $file
                sed -i "/.*vxFWriteImageNode.*/c \                    vxFWriteImageNode(graph, images[3], $img_out)," $file
        else
        	sed -i "/.*vxFReadImageNode.*/c \                    vxFReadImageNode(graph, $img_in, images[1])," $file
        	sed -i "/.*vxFWriteImageNode.*/c \                    vxFWriteImageNode(graph, images[2], $img_out)," $file
        fi
done
echo "Done."
