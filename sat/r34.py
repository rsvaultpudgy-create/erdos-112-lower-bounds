# SAT: is there an oriented graph on N vertices with no independent set of size A and no transitive T-tournament?
# Plain encoding + simple symmetry breaking (vertex 0 has maximum out-degree, out(0)={1..d}).
# usage: python3 r34.py N A T d timeout [cnf_out]
import sys, itertools, time, threading
from pysat.solvers import Cadical153
from pysat.card import CardEnc, EncType
from pysat.formula import IDPool, CNF
N,A,T,d,TO=map(int,sys.argv[1:6]); pool=IDPool(); a=lambda i,j: pool.id((i,j)); cl=[]
for i,j in itertools.combinations(range(N),2): cl.append([-a(i,j),-a(j,i)])
for S in itertools.combinations(range(N),A): cl.append([a(i,j) for i,j in itertools.permutations(S,2)])
for tup in itertools.permutations(range(N),T):   # forbid transitive tournament with order tup
    if tup[0]>tup[-1]: pass
    cl.append([-a(tup[i],tup[j]) for i in range(T) for j in range(i+1,T)])
for j in range(1,N): cl.append([a(0,j)] if j<=d else [-a(0,j)])
for v in range(1,N):
    enc=CardEnc.atmost(lits=[a(v,j) for j in range(N) if j!=v],bound=d,vpool=pool,encoding=EncType.seqcounter); cl.extend(enc.clauses)
if len(sys.argv)>6: CNF(from_clauses=cl).to_file(sys.argv[6])
s=Cadical153(bootstrap_with=cl); t=time.time(); tm=threading.Timer(TO,lambda: s.interrupt()); tm.start()
r=s.solve_limited(expect_interrupt=True); tm.cancel()
print(f"N={N} A={A} T={T} d={d} result={r} time={time.time()-t:.1f}s clauses={len(cl)}",flush=True)
