# SAT search: oriented graph on N vertices with no independent set of size A and no transitive triangle.
# Case split on out/in degree of vertex 0 (p,q) with vertex 0 of maximum degree (symmetry breaking).
# usage: python3 r63.py N A p q timeout_s
import sys, itertools, time
from pysat.solvers import Cadical153
from pysat.card import CardEnc, EncType
from pysat.formula import IDPool
N,A,p,q,TO=map(int,sys.argv[1:6])
pool=IDPool()
a=lambda i,j: pool.id(('a',i,j))
cl=[]
for i,j in itertools.permutations(range(N),2):
    if i<j: cl.append([-a(i,j),-a(j,i)])
for u in range(N):
    for v,w in itertools.combinations([x for x in range(N) if x!=u],2):
        cl.append([-a(u,v),-a(u,w),-a(v,w)]); cl.append([-a(u,v),-a(u,w),-a(w,v)])  # out-nbhd independent
for S in itertools.combinations(range(N),A):
    cl.append([a(i,j) for i,j in itertools.permutations(S,2)])
# vertex 0: out = 1..p, in = p+1..p+q, others non-adjacent
for j in range(1,N):
    if j<=p: cl.append([a(0,j)]); cl.append([-a(j,0)])
    elif j<=p+q: cl.append([a(j,0)]); cl.append([-a(0,j)])
    else: cl.append([-a(0,j)]); cl.append([-a(j,0)])
# every vertex has degree <= p+q (vertex 0 has max degree)
for v in range(1,N):
    lits=[a(v,j) for j in range(N) if j!=v]+[a(j,v) for j in range(N) if j!=v]
    enc=CardEnc.atmost(lits=lits,bound=p+q,vpool=pool,encoding=EncType.seqcounter)
    cl.extend(enc.clauses)
    if len(sys.argv)>6:
        enc=CardEnc.atleast(lits=lits,bound=int(sys.argv[6]),vpool=pool,encoding=EncType.seqcounter); cl.extend(enc.clauses)
t=time.time(); s=Cadical153(bootstrap_with=cl)
import threading
timer=threading.Timer(TO, lambda: s.interrupt()); timer.start()
r=s.solve_limited(expect_interrupt=True); timer.cancel()
print(f"N={N} A={A} p={p} q={q} result={r} time={time.time()-t:.1f}s clauses={len(cl)}",flush=True)
if r:
    m=set(x for x in s.get_model() if x>0)
    arcs=[(i,j) for i,j in itertools.permutations(range(N),2) if a(i,j) in m]
    print("ARCS",arcs)
