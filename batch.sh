#!/bin/bash
# For each (A,T), exhaustive circulant search for N from N0 up to 64; record largest N found.
run(){ A=$1; T=$2; N0=$3; best=0
  for N in $(seq $N0 64); do
    r=$(timeout ${4:-600} ./bt_circ $N $A $T 2>&1 | tail -1)
    echo "$r" >> batch_${A}_${T}.log
    case "$r" in FOUND*) best=$N;; esac
  done; echo "A=$A T=$T circulant_max_N=$best" >> batch_summary.txt; }
run 8 3 37; run 5 4 40; run 4 5 40 900; run 9 3 45; run 6 4 50
