#! /bin/bash

date +"%s.%3N" >> net.txt

awk -v old="$(awk '{print $0}' net.txt)" '
BEGIN{
	split(old,prev," ")
    recieve = 0
    transmit = 0
}
{
if ((NR > 2)){
    recieve += $2
    transmit += $10
}
}
END{
	delta_time = prev[4] - prev[1]
# in kbytes instead of bytes
    printf "%.2f\t",(recieve - prev[2]) / (delta_time * 1024)
    printf "%.2f\t",(transmit - prev[3]) / (delta_time * 1024)
}
' /proc/net/dev >> data.txt
exit 0
