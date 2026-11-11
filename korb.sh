#!/bin/bash
# usage: korb.sh m k A T seedstart seedend iters
for seed in $(seq $5 $6); do
  out=$(timeout 600 ./sa_korb $1 $2 $3 $4 $seed $7)
  echo "seed=$seed $(echo "$out" | head -1)" >> gen/korb_$1_$2_$3_$4.log
  if echo "$out" | head -1 | grep -q " cost=0 "; then echo "$out" | tail -n +2 > gen/hitK_$1_$2_$3_$4_$seed.txt; break; fi
done
