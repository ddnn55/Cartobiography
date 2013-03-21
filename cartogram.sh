#!/usr/bin/env bash

echo "Usage: $0 photos_json out grid_size"

photos_json=$1
out=$2
grid_size=$3

cmd1="node cartogram.js $photos_json $out $grid_size"
echo $cmd1
`$cmd1`

cmd2="./cart-1.2.2/cart $grid_size $grid_size data/$out.density.$grid_size.dat data/$out.distortion.$grid_size.dat"
echo $cmd2
`$cmd2`

node cartoDistortionViz.js data/$out.distortion.$grid_size.dat $photos_json $grid_size > data/$out.$grid_size.svg
