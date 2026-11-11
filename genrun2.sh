#!/bin/bash
# usage: genrun2.sh N A T seedstart seedend tlimit iters
N=$1; A=$2; T=$3
for seed in $(seq $4 $5); do
  out=$(timeout $6 ./sa_gen $N $A $T $seed $7)
  echo "seed=$seed $(echo "$out" | head -1)" >> gen/gen_${N}_${A}_${T}.log
  if echo "$out" | head -1 | grep -q "recount 0)"; then echo "$out" | tail -n +2 > gen/hit_${N}_${A}_${T}_$seed.txt; break; fi
done
