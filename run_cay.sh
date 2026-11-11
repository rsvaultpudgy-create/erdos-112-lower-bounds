#!/bin/bash
# exhaustive Cayley search over all non-cyclic group tables of given orders
A=$1; T=$2; lo=$3; hi=$4
for f in groups/*.txt; do n=$(head -1 $f); [ $n -ge $lo ] && [ $n -le $hi ] || continue
  case $(basename $f) in Z$n.txt) continue;; esac
  r=$(timeout ${5:-300} ./bt_cay $f $A $T 2>&1 | tail -1); [ -z "$r" ] && r="timeout $f"
  echo "$f $r" >> cayx_${A}_${T}.log
done; echo DONE >> cayx_${A}_${T}.log
