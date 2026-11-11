#!/bin/bash
# usage: seeded.sh N A T initfile seedstart seedend tlimit iters T0 TMIN TRS DEC
for seed in $(seq $5 $6); do
  out=$(timeout $7 ./sa_gen2 $1 $2 $3 $seed $8 $4 $9 ${10} ${11} ${12})
  echo "seed=$seed $(echo "$out" | head -1)" >> gen/seed_$1_$2_$3.log
  if echo "$out" | head -1 | grep -q "recount 0)"; then echo "$out" | tail -n +2 > gen/hitS_$1_$2_$3_$seed.txt; break; fi
done
