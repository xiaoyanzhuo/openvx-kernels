# OpenVX Kernel Benchmark
Mainly includes 44 standard kernels of OpenVX 1.2, run each kernel independently to verify and understand how each kernel works. 

## Prerequisite
To run the openvx kernel benchmark programs, you need to set up openvx environment first. There is an easy way to build openvx environment by following [this](https://github.com/tazzaoui/build-openvx).

## Usage
After the envoriment is ready, download the files from this repo. We can use *Makefile* to create executable files and run each of them. 

`$ cd openvx-kernels`

`$ make`  

`$ cd ./bin_kernels` 

`$ ./gaussian ` (or other executable files)

**Note**: Since we are trying to run only one kernel node once, some of them need to use the output of other kernel running results as input. So there are some running order for certain kernels: 

1. Before run '*absdiff*', run '*lut*' first.
2. Before run '*add, sub, multiply, bitwise_and, bitwise_or, bitwise_xor'*, run '*absdiff*'  first.
3. Before run '*erode, dilate*', run '*thresh*' first.


**Others**

4. '*canny_1, canny_2* ' with different thresholds, 'VX_NORM_L1' for '*canny_1*', and 'VX_NORM_L2' for '*canny_2* '. Mainly to observe the difference canny edge detection results.
5. '*multiply, remap*' have different policies for setting parameters, we just choose one of them to run a single node. Differet options have been implemented in '*multiply.c, remap.c*' and need to be commented out if needed.
6. '*absdiff\_1.c, multiply\_1.c, channel_combine\_1.c*' are used to verify the results of '*absdiff, multiply, channel_combine*'. They have been listed in *Makefile* but been commented, so if you want to run them, comment out them in *Makefile* to generate excutable files.


Credits: OpenVX (https://www.khronos.org/openvx/)
