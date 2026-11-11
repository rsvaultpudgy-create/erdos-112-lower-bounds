#!/bin/bash
for seed in $(seq 1 300); do
  out=$(timeout 300 ./sa_gen 30 6 3 $((seed+5000)) 40000000 witness_k63_N29.txt)
  echo "seed=$seed $(echo "$out" | head -1)" >> gen/seeded30.log
  if echo "$out" | head -1 | grep -q "recount 0)"; then echo "$out" | tail -n +2 > gen/hit_30_6_3_seeded_$seed.txt; break; fi
done
