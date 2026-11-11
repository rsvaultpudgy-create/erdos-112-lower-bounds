# Driver: for target (A,T), try increasing N with SA restarts; verify hits independently; log.
import subprocess, sys, time
A,T,N0,Nmax,restarts,iters,tlimit=map(int,sys.argv[1:8])
log=open(f'drive_{A}_{T}.log','a')
N=N0; fails=0
while N<=Nmax:
    ok=False
    for r in range(restarts):
        try:
            out=subprocess.run(['./sa_circ',str(N),str(A),str(T),str(1000*N+r+int(time.time())%997),str(iters)],capture_output=True,text=True,timeout=tlimit).stdout.strip()
        except subprocess.TimeoutExpired:
            out='timeout'
        log.write(out+'\n'); log.flush()
        if 'cost=0 ' in out:
            S=out.split('S=')[1].split()
            v=subprocess.run(['python3','check/verify.py',str(N),str(A),str(T)]+S,capture_output=True,text=True).stdout.strip()
            log.write('VERIFY: '+v+'\n'); log.flush()
            if 'VALID' in v and 'INVALID' not in v:
                open('witnesses.txt','a').write(f"{N} {A} {T} : {' '.join(S)}\n"); ok=True; break
    if not ok:
        fails+=1
        if fails>=2: break
    else: fails=0
    N+=1
log.write(f'DONE A={A} T={T} stopped at N={N}\n')
