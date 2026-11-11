# Generate multiplication tables (element 0 = identity) for small groups.
import itertools, sys, os
def write(name, elems, op):
    idx={e:i for i,e in enumerate(elems)}
    N=len(elems)
    with open(f'groups/{name}.txt','w') as f:
        f.write(f'{N}\n')
        for a in elems: f.write(' '.join(str(idx[op(a,b)]) for b in elems)+'\n')
def cyc(n): write(f'Z{n}', list(range(n)), lambda a,b:(a+b)%n)
def prod(a,b): write(f'Z{a}xZ{b}', [(i,j) for i in range(a) for j in range(b)], lambda x,y:((x[0]+y[0])%a,(x[1]+y[1])%b))
def prod3(a,b,c): write(f'Z{a}xZ{b}xZ{c}', [(i,j,k) for i in range(a) for j in range(b) for k in range(c)], lambda x,y:((x[0]+y[0])%a,(x[1]+y[1])%b,(x[2]+y[2])%c))
def dih(n): # elements (r,s): r^a s^b ; s r = r^-1 s
    write(f'D{n}', [(a,b) for b in range(2) for a in range(n)], lambda x,y: ((x[0]+(y[0] if x[1]==0 else -y[0]))%n, (x[1]+y[1])%2))
def semi(n,m,r): # Z_n x| Z_m with generator of Z_m acting by x -> r x (r^m = 1 mod n)
    assert pow(r,m,n)==1
    write(f'Z{n}sZ{m}r{r}', [(a,b) for b in range(m) for a in range(n)], lambda x,y: ((x[0]+pow(r,x[1],n)*y[0])%n,(x[1]+y[1])%m))
for n in range(9,65): cyc(n)
for a in range(2,9):
    for b in range(a,33):
        if a*b<=64 and a*b>=9: prod(a,b)
for (a,b,c) in [(2,2,k) for k in range(2,17)]+[(2,3,3),(3,3,3),(2,3,6),(3,3,6),(2,2,2)]:
    if a*b*c<=64: prod3(a,b,c)
for n in range(5,33): dih(n)
for n in range(3,33):
    for m in range(2,17):
        if n*m>64: continue
        for r in range(2,n):
            if pow(r,m,n)==1 and all(pow(r,k,n)!=1 for k in range(1,m)) or (pow(r,m,n)==1 and r!=1 and m%2==0 and False):
                semi(n,m,r)
print(len(os.listdir('groups')))
