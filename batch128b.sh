#!/bin/bash
run(){ A=$1; T=$2; N0=$3; N1=$4; step=$5; best=0
  for N in $(seq $N0 $step $N1); do
    r=$(timeout ${6:-600} ./bt_circ128 $N $A $T 2>&1 | tail -1); [ -z "$r" ] && r="N=$N timeout"
    echo "$r" >> b128_${A}_${T}.log
    case "$r" in FOUND*) best=$N;; esac
  done; echo "A=$A T=$T circulant_best_found(range $N0-$N1 step $step)=$best" >> batch_summary.txt; }
run 3 6 55 127 4 600; run 4 5 66 127 4 600; run 6 4 66 127 4 600
