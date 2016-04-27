#!/bin/bash

make distclean

make imx6_android_defconfig

make uImage

cd ../

make bootimage
