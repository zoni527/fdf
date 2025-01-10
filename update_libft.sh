#!/bin/bash
rm -f ./lib/libft/*
cp ~/Repos/libft/* ./lib/libft/
cd ./lib/libft/
make all clean
cd ..
