#!/bin/bash

make uImage -j8

cd ../
# make boot.img 
make bootimage
