#!/bin/bash
# general SA restarts for tight cases; save any valid graph
for seed in $(seq 1 400); do
  for cfg in "29 6 3" "40 4 4" "43 3 5" "37 7 3"; do
    set -- $cfg
    out=$(timeout 300 ./sa_gen $1 $2 $3 $seed 30000000)
    echo "seed=$seed $(echo "$out" | head -1)" >> gen/gen.log
    if echo "$out" | head -1 | grep -q "recount 0)"; then echo "$out" | tail -n +2 > gen/hit_$1_$2_$3_$seed.txt; fi
  done
done
