# Independent brute-force verifier for problem 112 witnesses.
# Usage: python3 verify.py N A T s1 s2 ...   (circulant: arc i->j iff (j-i)%N in S)
# Checks: S is an oriented connection set (no s with -s in S, no 0),
#         no independent set of size A, no transitive tournament on T vertices.
import sys, itertools
N,A,T=map(int,sys.argv[1:4]); S=set(int(x)%N for x in sys.argv[4:])
assert 0 not in S and all((-s)%N not in S for s in S), "not oriented"
arc=[[((j-i)%N) in S for j in range(N)] for i in range(N)]
for i in range(N):
    for j in range(N):
        assert not (arc[i][j] and arc[j][i])
adj=lambda i,j: arc[i][j] or arc[j][i]
ind=0
for c in itertools.combinations(range(N),A):
    if all(not adj(i,j) for i,j in itertools.combinations(c,2)): ind+=1
tt=0
for c in itertools.combinations(range(N),T):
    if not all(adj(i,j) for i,j in itertools.combinations(c,2)): continue
    # a tournament is transitive iff out-degrees within it are all distinct
    outd=sorted(sum(arc[i][j] for j in c if j!=i) for i in c)
    if outd==list(range(T)): tt+=1
print(f"N={N} A={A} T={T} |S|={len(S)} independent_{A}_sets={ind} transitive_T{T}={tt} ->", "VALID" if ind==0 and tt==0 else "INVALID")
