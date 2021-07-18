#! /bin/sh
w > info.txt
last boot >> info.txt
cat /proc/version >> info.txt
lshw -short >> info.txt


# cat info.txt
exit 0
