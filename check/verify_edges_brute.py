# Pure brute-force verifier for an oriented graph given as arc list ("u v" = arc u->v).
# Usage: python3 verify_edges_brute.py file N A T
# Checks: no loops, no 2-cycles; every A-subset contains an arc; no T-subset induces a transitive tournament.
import sys, itertools
fn=sys.argv[1]; N,A,T=map(int,sys.argv[2:5])
arc=[[False]*N for _ in range(N)]
for line in open(fn):
    p=line.split()
    if len(p)==2: u,v=int(p[0]),int(p[1]); assert u!=v and 0<=u<N and 0<=v<N; arc[u][v]=True
for u in range(N):
    for v in range(N): assert not (arc[u][v] and arc[v][u]), "2-cycle"
adj=[[arc[u][v] or arc[v][u] for v in range(N)] for u in range(N)]
ind=sum(1 for c in itertools.combinations(range(N),A) if not any(adj[a][b] for a,b in itertools.combinations(c,2)))
tt=0
for c in itertools.combinations(range(N),T):
    if all(adj[a][b] for a,b in itertools.combinations(c,2)):
        outd=sorted(sum(arc[a][b] for b in c if b!=a) for a in c)
        if outd==list(range(T)): tt+=1
print(f"{fn} N={N} A={A} T={T} independent_{A}_sets={ind} transitive_T{T}={tt} ->","VALID" if ind==0 and tt==0 else "INVALID")
