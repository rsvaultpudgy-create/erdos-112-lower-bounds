# Convert a bt_bicirc FOUND line to an arc list file ("u v" per line). Vertex (i,a) -> a*n+i.
import sys,re
line=sys.argv[1]; outf=sys.argv[2]
n=int(re.search(r'n=(\d+)',line).group(1))
get=lambda key:[int(x) for x in re.search(key+r'=([\d,]*)',line).group(1).split(',') if x]
S0,S1,B01,B10=get('S0'),get('S1'),get('B01'),get('B10')
arcs=set()
for i in range(n):
    for s in S0: arcs.add((i,(i+s)%n))
    for s in S1: arcs.add((n+i,n+(i+s)%n))
    for d in B01: arcs.add((i,n+(i+d)%n))
    for d in B10: arcs.add((n+(i+d)%n,i))
open(outf,'w').write('\n'.join(f"{u} {v}" for u,v in sorted(arcs))+'\n')
print(len(arcs),'arcs, N=',2*n)
