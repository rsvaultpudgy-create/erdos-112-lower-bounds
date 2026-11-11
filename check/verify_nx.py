# Second independent verifier (uses networkx exact maximum clique for the independence number,
# and direct enumeration for transitive tournaments). Circulant input like verify.py.
import sys, itertools, networkx as nx
N,A,T=map(int,sys.argv[1:4]); S=set(int(x)%N for x in sys.argv[4:])
assert 0 not in S and all((-s)%N not in S for s in S)
D=nx.DiGraph(); D.add_nodes_from(range(N)); D.add_edges_from((i,(i+s)%N) for i in range(N) for s in S)
U=D.to_undirected()
comp=nx.complement(U)
alpha=max(len(c) for c in nx.find_cliques(comp))
# transitive T-tournaments: enumerate cliques of U of size T and test orientation acyclicity
tt=0
for c in nx.enumerate_all_cliques(U):
    if len(c)>T: break
    if len(c)==T and nx.is_directed_acyclic_graph(D.subgraph(c)): tt+=1
print(f"N={N} A={A} T={T} alpha={alpha} transitive_T{T}={tt} ->","VALID" if alpha<A and tt==0 else "INVALID")
