# Independent brute-force verifier for Cayley oriented graphs.
# Usage: python3 verify_cay.py groupfile A T s1 s2 ...   (arc g -> g*s for s in S)
# Also checks the table is a group (associative, identity 0, inverses).
import sys, itertools
gf=sys.argv[1]; A,T=int(sys.argv[2]),int(sys.argv[3]); S=[int(x) for x in sys.argv[4:]]
L=open(gf).read().split(); N=int(L[0]); M=[[int(L[1+i*N+j]) for j in range(N)] for i in range(N)]
assert all(M[0][j]==j and M[j][0]==j for j in range(N))
assert all(M[M[a][b]][c]==M[a][M[b][c]] for a in range(N) for b in range(N) for c in range(N)), "not associative"
assert all(any(M[a][b]==0 for b in range(N)) for a in range(N))
arc=[[False]*N for _ in range(N)]
for g in range(N):
    for s in S: arc[g][M[g][s]]=True
for i in range(N):
    assert not arc[i][i]
    for j in range(N): assert not (arc[i][j] and arc[j][i]), "2-cycle"
adj=lambda i,j: arc[i][j] or arc[j][i]
ind=sum(1 for c in itertools.combinations(range(N),A) if all(not adj(i,j) for i,j in itertools.combinations(c,2)))
tt=0
for c in itertools.combinations(range(N),T):
    if all(adj(i,j) for i,j in itertools.combinations(c,2)):
        if sorted(sum(arc[i][j] for j in c if j!=i) for i in c)==list(range(T)): tt+=1
print(f"{gf} N={N} A={A} T={T} independent_{A}_sets={ind} transitive_T{T}={tt} ->","VALID" if ind==0 and tt==0 else "INVALID")
