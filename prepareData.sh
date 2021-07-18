#! /bin/bash
# old values of cpu data
awk '/cpu/ {print $2 + $4; print $5}' /proc/stat > cpu.txt
# record time when started
date +"%s.%3N" > net.txt
# old values for net data
awk '
BEGIN{
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
    print recieve
    print transmit
}
' /proc/net/dev >> net.txt
exit 0
