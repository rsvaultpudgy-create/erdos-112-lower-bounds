# Independent verifier for an arbitrary oriented graph given as an arc list file ("i j" per line = arc i->j).
# Usage: python3 verify_edges.py file N A T     (uses networkx max clique for independence number)
import sys, itertools, networkx as nx
fn=sys.argv[1]; N,A,T=map(int,sys.argv[2:5])
D=nx.DiGraph(); D.add_nodes_from(range(N))
for line in open(fn):
    p=line.split()
    if len(p)==2 and p[0].isdigit(): D.add_edge(int(p[0]),int(p[1]))
assert all(not D.has_edge(v,u) for u,v in D.edges()), "2-cycle"
assert all(u!=v for u,v in D.edges())
U=D.to_undirected(); alpha=max(len(c) for c in nx.find_cliques(nx.complement(U)))
tt=sum(1 for c in itertools.combinations(range(N),T) if all(U.has_edge(a,b) for a,b in itertools.combinations(c,2)) and nx.is_directed_acyclic_graph(D.subgraph(c)))
print(f"{fn} N={N} A={A} T={T} arcs={D.number_of_edges()} alpha={alpha} transitive_T{T}={tt} ->","VALID" if alpha<A and tt==0 else "INVALID")
