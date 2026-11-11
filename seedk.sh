#!/bin/bash
for r in $(seq 1 100); do for s in 1 2 3 4 5 6; do
  out=$(timeout 300 ./sa_gen2 30 6 3 $((r*10+s)) 20000000 gen/k1_$s.arcs 0.25 0.03 0.2 0.999998)
  echo "r=$r s=$s $(echo "$out" | head -1)" >> gen/seedk.log
  if echo "$out" | head -1 | grep -q "recount 0)"; then echo "$out" | tail -n +2 > gen/hitSK_${r}_$s.txt; exit; fi
done; done
