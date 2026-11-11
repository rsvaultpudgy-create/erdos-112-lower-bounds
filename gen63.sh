#!/bin/bash
for seed in $(seq 1000 1300); do
  out=$(timeout 400 ./sa_gen 29 6 3 $seed 60000000)
  echo "seed=$seed $(echo "$out" | head -1)" >> gen/gen63.log
  if echo "$out" | head -1 | grep -q "recount 0)"; then echo "$out" | tail -n +2 > gen/hit_29_6_3_$seed.txt; break; fi
done
