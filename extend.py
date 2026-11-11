# Can a circulant (A,T)-free oriented graph on N vertices be extended by one vertex?
# SAT over the arcs between new vertex x and old vertices. usage: python3 extend.py A T file_of_FOUND_lines
import sys, itertools
from pysat.solvers import Cadical153
from pysat.formula import IDPool
A,T=int(sys.argv[1]),int(sys.argv[2])
assert T==3
for line in open(sys.argv[3]):
    if not line.startswith('FOUND'): continue
    N=int(line.split('N=')[1].split()[0]); S=set(int(s)%N for s in line.split('S=')[1].split())
    arc=lambda i,j: ((j-i)%N) in S
    adj=lambda i,j: arc(i,j) or arc(j,i)
    pool=IDPool(); o=lambda v: pool.id(('o',v)); i_=lambda v: pool.id(('i',v))  # x->v, v->x
    cl=[[-o(v),-i_(v)] for v in range(N)]
    # no transitive triangle containing x
    for u,v in itertools.combinations(range(N),2):
        if not adj(u,v): continue
        a,b=(u,v) if arc(u,v) else (v,u)   # a->b
        # triangle x,a,b transitive unless cycle: x->a->b->x  i.e. o(a) & i(b)
        # forbid any adjacency pattern other than (o(a),i(b)) when x adjacent to both
        for xa in (o(a),i_(a)):
            for xb in (o(b),i_(b)):
                if (xa,xb)!=(o(a),i_(b)): cl.append([-xa,-xb])
    # no independent A-set containing x: every independent (A-1)-set of old graph must have a neighbor of x
    for c in itertools.combinations(range(N),A-1):
        if all(not adj(p,q) for p,q in itertools.combinations(c,2)):
            cl.append([lit for v in c for lit in (o(v),i_(v))])
    s=Cadical153(bootstrap_with=cl); r=s.solve()
    print(N,sorted(S),'extendable' if r else 'NOT extendable')
