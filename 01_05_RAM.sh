#! /bin/bash
free | awk '/:/ {printf "%.2f\t", ($3 / (1024*1024))}' > data.txt


exit 0
