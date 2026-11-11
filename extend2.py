# One-vertex extension test for circulant (A,T)-free oriented graphs (any T).
# usage: python3 extend2.py A T file_with_FOUND_lines
import sys, itertools
from pysat.solvers import Cadical153
from pysat.formula import IDPool
A,T=int(sys.argv[1]),int(sys.argv[2])
def tts(N,out,k):  # all transitive k-tournaments as ordered tuples (source first)
    res=[]
    def rec(ch,cand):
        if len(ch)==k: res.append(tuple(ch)); return
        for v in cand: rec(ch+[v],[w for w in cand if w in out[v]])
    for v in range(N): rec([v],sorted(out[v]))
    return res
for line in open(sys.argv[3]):
    if not line.startswith('FOUND'): continue
    N=int(line.split('N=')[1].split()[0]); S=set(int(s)%N for s in line.split('S=')[1].split())
    out=[set((i+s)%N for s in S) for i in range(N)]
    adj=lambda i,j: j in out[i] or i in out[j]
    pool=IDPool(); o=lambda v: pool.id(('o',v)); i_=lambda v: pool.id(('i',v))
    cl=[[-o(v),-i_(v)] for v in range(N)]
    for tup in tts(N,out,T-1):
        for p in range(T):   # x inserted at position p: x <- tup[:p], x -> tup[p:]
            cl.append([-i_(v) for v in tup[:p]]+[-o(v) for v in tup[p:]])
    for c in itertools.combinations(range(N),A-1):
        if all(not adj(p,q) for p,q in itertools.combinations(c,2)):
            cl.append([l for v in c for l in (o(v),i_(v))])
    s=Cadical153(bootstrap_with=cl); r=s.solve()
    print(N,A,T,sorted(S),'extendable' if r else 'NOT extendable',flush=True)
