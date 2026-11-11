# One-vertex extension test for an arbitrary oriented graph (arc list) w.r.t. (A,T)-freeness.
# usage: python3 extend_edges.py file N A T  -> prints extendable/NOT and writes extended arc list if found
import sys, itertools
from pysat.solvers import Cadical153
from pysat.formula import IDPool
fn=sys.argv[1]; N,A,T=map(int,sys.argv[2:5])
out=[set() for _ in range(N)]
for l in open(fn):
    u,v=map(int,l.split()); out[u].add(v)
adj=lambda i,j: j in out[i] or i in out[j]
def tts(k):
    res=[]
    def rec(ch,cand):
        if len(ch)==k: res.append(tuple(ch)); return
        for v in cand: rec(ch+[v],[w for w in cand if w in out[v]])
    for v in range(N): rec([v],sorted(out[v]))
    return res
pool=IDPool(); o=lambda v: pool.id(('o',v)); i_=lambda v: pool.id(('i',v))
cl=[[-o(v),-i_(v)] for v in range(N)]
for tup in tts(T-1):
    for p in range(T): cl.append([-i_(v) for v in tup[:p]]+[-o(v) for v in tup[p:]])
for c in itertools.combinations(range(N),A-1):
    if all(not adj(p,q) for p,q in itertools.combinations(c,2)): cl.append([l for v in c for l in (o(v),i_(v))])
s=Cadical153(bootstrap_with=cl); r=s.solve()
print(fn,'extendable' if r else 'NOT extendable')
if r:
    m=set(x for x in s.get_model() if x>0); x=N
    with open(fn+'.ext','w') as f:
        for u in range(N):
            for v in out[u]: f.write(f"{u} {v}\n")
        for v in range(N):
            if o(v) in m: f.write(f"{x} {v}\n")
            if i_(v) in m: f.write(f"{v} {x}\n")
