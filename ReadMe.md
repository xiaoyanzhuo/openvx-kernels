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

Credits: OpenVX (https://www.khronos.org/openvx/)