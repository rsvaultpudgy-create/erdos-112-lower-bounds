# More group tables: all semidirect Z_n x|_r Z_m, dicyclic groups, products of small groups with cyclic groups, A4, S4.
import itertools, os
from sympy.combinatorics.named_groups import SymmetricGroup, AlternatingGroup
def write(name, elems, op):
    idx={e:i for i,e in enumerate(elems)}; N=len(elems)
    assert elems[0]==op(elems[0],elems[0]) and all(op(elems[0],e)==e for e in elems)
    with open(f'groups/{name}.txt','w') as f:
        f.write(f'{N}\n')
        for a in elems: f.write(' '.join(str(idx[op(a,b)]) for b in elems)+'\n')
def table_of(name):
    L=open(f'groups/{name}.txt').read().split(); N=int(L[0]); return N,[[int(L[1+i*N+j]) for j in range(N)] for i in range(N)]
def product(n1,n2,out):
    N1,M1=table_of(n1); N2,M2=table_of(n2)
    elems=[(a,b) for a in range(N1) for b in range(N2)]
    write(out, elems, lambda x,y:(M1[x[0]][y[0]],M2[x[1]][y[1]]))
def semi(n,m,r):
    write(f'Z{n}sZ{m}r{r}', [(a,b) for b in range(m) for a in range(n)], lambda x,y: ((x[0]+pow(r,x[1],n)*y[0])%n,(x[1]+y[1])%m))
def dicyclic(n): # Dic_n of order 4n: <a,x | a^{2n}=1, x^2=a^n, x a x^-1 = a^-1>; elements a^i x^j, j in {0,1}
    def op(p,q):
        i,j=p; k,l=q
        if j==0: return ((i+k)%(2*n), l)
        # x a^k = a^-k x
        if l==0: return ((i-k)%(2*n), 1)
        return ((i-k+n)%(2*n), 0)   # a^i x a^k x = a^{i-k} x^2 = a^{i-k+n}
    write(f'Dic{n}', [(i,j) for j in range(2) for i in range(2*n)], op)
def perm_group(G,name):
    els=list(G.generate()); e=[g for g in els if g.is_Identity][0]; els.remove(e); els=[e]+els
    write(name, els, lambda a,b: a*b)
cnt=0
for n in range(3,33):
    for m in range(2,22):
        if n*m>64: continue
        for r in range(2,n):
            if pow(r,m,n)==1: semi(n,m,r); cnt+=1
for n in range(2,17): dicyclic(n)
perm_group(AlternatingGroup(4),'A4'); perm_group(SymmetricGroup(4),'S4')
for base in ['A4','S4','D5','D6','D7','D8','D9','D10','Dic2','Dic3','Dic4','Dic5','Dic6']:
    Nb=int(open(f'groups/{base}.txt').readline())
    for k in range(2,33):
        if Nb*k<=64:
            if not os.path.exists(f'groups/Z{k}.txt'):
                write(f'Z{k}', list(range(k)), lambda a,b,k=k:(a+b)%k)
            product(base,f'Z{k}',f'{base}xZ{k}')
print(len(os.listdir('groups')))
