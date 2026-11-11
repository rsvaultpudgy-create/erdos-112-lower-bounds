# For target (A,T) and order range, run SA on all non-cyclic group tables of that order; verify hits.
import subprocess, sys, os, glob, time
A,T,lo,hi,restarts,iters,tl=map(int,sys.argv[1:8])
log=open(f'cay_{A}_{T}.log','a')
groups=[]
for f in glob.glob('groups/*.txt'):
    n=int(open(f).readline())
    if lo<=n<=hi and not os.path.basename(f).startswith('Z'+str(n)+'.'): groups.append((n,f))
groups.sort()
for n,f in groups:
    for r in range(restarts):
        try: out=subprocess.run(['./sa_cay',f,str(A),str(T),str(r*7919+n),str(iters)],capture_output=True,text=True,timeout=tl).stdout.strip()
        except subprocess.TimeoutExpired: out=f'timeout {f}'
        log.write(out+'\n'); log.flush()
        if 'cost=0 ' in out:
            S=out.split('S=')[1].split()
            v=subprocess.run(['python3','check/verify_cay.py',f,str(A),str(T)]+S,capture_output=True,text=True).stdout.strip()
            log.write('VERIFY: '+v+'\n'); log.flush()
            if v.endswith('VALID') and 'INVALID' not in v:
                open('witnesses_cay.txt','a').write(f"{f} {A} {T} : {' '.join(S)}\n"); break
log.write('DONE\n')
