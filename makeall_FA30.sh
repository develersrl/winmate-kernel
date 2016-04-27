#!/bin/bash

make distclean

make FA30_android_defconfig

make uImage -j8

cd ../
# make boot.img 
make bootimage
