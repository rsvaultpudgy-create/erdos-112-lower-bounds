#!/bin/bash
N=$1; A=$2; T=$3
for seed in $(seq $4 $5); do
  out=$(timeout $6 ./sa_gen2 $N $A $T $seed $7 - $8 $9 ${10} ${11})
  echo "seed=$seed $(echo "$out" | head -1)" >> gen/g3_${N}_${A}_${T}.log
  if echo "$out" | head -1 | grep -q "recount 0)"; then echo "$out" | tail -n +2 > gen/hit3_${N}_${A}_${T}_$seed.txt; break; fi
done
