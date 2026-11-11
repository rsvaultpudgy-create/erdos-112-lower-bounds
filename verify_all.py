# Re-verify every witness in witnesses.txt with the independent checkers.
# Brute force (check/verify.py) when feasible, networkx (check/verify_nx.py) always.
import subprocess
from math import comb
seen=set()
for line in open('witnesses.txt'):
    if line.startswith('#') or ':' not in line: continue
    head,S=line.split(':'); N,A,T=map(int,head.split()); S=S.split()
    key=(N,A,T)
    if key in seen: continue
    seen.add(key)
    res=[]
    if comb(N,A)+comb(N,T)<=4e7:
        res.append(subprocess.run(['python3','check/verify.py',str(N),str(A),str(T)]+S,capture_output=True,text=True).stdout.strip().split('->')[-1].strip()+'(brute)')
    res.append(subprocess.run(['python3','check/verify_nx.py',str(N),str(A),str(T)]+S,capture_output=True,text=True).stdout.strip().split('->')[-1].strip()+'(networkx)')
    print(f"N={N:3d} n(indep)={A} m(TT)={T}  k({A},{T}) > {N}   " + ' '.join(res), flush=True)
