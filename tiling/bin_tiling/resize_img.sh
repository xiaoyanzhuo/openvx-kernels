#!/usr/bin/env bash
#title          :resize_img.sh
#description    :This script is used to resize the image file, the image name define like this way: lena_512x512.pgm 
#                (prefix(lena) followed by "_", and then "x" between width and height (512x512), "." before file type(pgm))
#author         :Xiaoyan Zhuo <xiaoyanzhuo2@gmail.com>
#date           :2.26.19
#version        :1    
#usage          :./resize_img.sh <image_file_name> <resized image_width> <resized image_height> 
#example        :./resize_img.sh lena_512x512.pgm 1024 1024 (output image will be "lena_1024x1024.pgm")
#==========================================================================================================================

img_orig=$1
img_w2=$2
img_h2=$3
img_in=\"$1\"

echo "input image: "$img_in
fname_no_ext=${img_orig%.*}
img_prefix=$(echo $fname_no_ext | awk -F'[_x]' '{print $1}')
img_w1=$(echo $fname_no_ext | awk -F'[_x]' '{print $2}')
img_h1=$(echo $fname_no_ext | awk -F'[_x]' '{print $3}')
echo "image prefix: "$img_prefix
echo "image width: "$img_w1
echo "image height: "$img_h1

echo "resize image width to: "$img_w2
echo "resize image height to: "$img_h2
img_out=\"$img_prefix"_"$2"x"$3".pgm"\"
echo "expected output image: "$img_out

sed -i "/.*#define img_w1.*/c #define img_w1 $img_w1" scale.c
sed -i "/.*#define img_h1.*/c #define img_h1 $img_h1" scale.c
sed -i "/.*#define img_w2.*/c #define img_w2 $img_w2" scale.c
sed -i "/.*#define img_h2.*/c #define img_h2 $img_h2" scale.c
echo "Finish replacing image size."

sed -i "/.*vxuFReadImage.*/c \		vxuFReadImage(context, $img_in, images[0]);" scale.c
sed -i "/.*vxuFWriteImage.*/c \				vxuFWriteImage(context, images[1], $img_out);" scale.c
echo "Finish replacing input and output image files."

if [ -f ./scale.out ]; then
	make clean
	echo "clean previous scale.out."
fi
echo "Compiling..."
make
echo "Running scale kernel..."
./scale.out

echo "Resize image done."
echo "Resized image: "$img_out
