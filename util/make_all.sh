#!/bin/bash

for dir in */; do
    if [ -f "${dir}Makefile" ]; then
        echo "Making in ${dir}"
        make -C "$dir"
    fi
done
