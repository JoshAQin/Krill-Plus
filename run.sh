#!/bin/sh

echo "mbfs"
make all PROJECT_NAME=mbfs

echo "msssp"
make all PROJECT_NAME=msssp WEIGHTED=1

echo "homo1"
make all PROJECT_NAME=homo1

echo "homo2"
make all PROJECT_NAME=homo2 WEIGHTED=1

echo "heter"
make all PROJECT_NAME=heter WEIGHTED=1