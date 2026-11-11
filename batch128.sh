#!/bin/bash
run(){ A=$1; T=$2; N0=$3; N1=$4; best=0
  for N in $(seq $N0 $N1); do
    r=$(timeout ${5:-900} ./bt_circ128 $N $A $T 2>&1 | tail -1); [ -z "$r" ] && r="N=$N timeout"
    echo "$r" >> b128_${A}_${T}.log
    case "$r" in FOUND*) best=$N;; esac
  done; echo "A=$A T=$T circulant_max_N(range $N0-$N1)=$best" >> batch_summary.txt; }
run 9 3 65 74; run 4 5 65 100 900; run 6 4 65 100 900; run 5 4 61 70 900
