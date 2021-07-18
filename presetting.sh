#! /bin/bash

nproc > preset.txt
free | awk '/:/ {printf "%.2f\n",($2 / (1024*1024))}' >> preset.txt

exit 0
