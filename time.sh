#!/bin/bash

start=$(date +%s,%N)
s1=$(echo $start | cut -d',' -f1)   # sec
s2=$(echo $start | cut -d',' -f2)   # nano sec

./server

end=$(date +%s,%N)
e1=$(echo $end | cut -d',' -f1)
e2=$(echo $end | cut -d',' -f2)

time_cost=$(bc <<< "scale=3; $e1 - $s1 + ($e2 - $s2)/1000000000")

echo "Total travel time: $time_cost Seconds"