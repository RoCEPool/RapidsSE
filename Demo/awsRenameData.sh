#!/bin/bash

FILES=($(find /mnt -name "*.tbl.*" | xargs))

for f in "${FILES[@]}"
do
  echo mv "${f}" "${f%.*}"
  mv "${f}" "${f%.*}"
done
