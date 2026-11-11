// TABU variant (sampled best-move) over general oriented graphs on N vertices: minimize #independent A-sets + #transitive T-tournaments.
// usage: sa_gen N A T seed iters [initfile]   initfile: lines "i j" meaning arc i->j (optional)
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
typedef uint64_t u64;
static int N,A,T; static u64 out[64],in_[64],adj[64],ALLM;
static u64 rng=88172645463325252ULL;
static inline u64 xr(){rng^=rng<<13;rng^=rng>>7;rng^=rng<<17;return rng;}
static long cntIS(u64 cand,int d){ if(d==0) return 1; long c=0; while(cand){int v=__builtin_ctzll(cand); cand&=cand-1; c+=cntIS(cand&~adj[v],d-1);} return c; }
static long chains(u64 cand,int d,u64 H){ if(d==0) return 1; long c=0; u64 it=cand; while(it){int v=__builtin_ctzll(it); it&=it-1; c+=chains(cand&out[v]&H,d-1,H);} return c; }
static long ttIn(u64 H){ // number of TT_T inside vertex set H
  long c=0; u64 it=H; while(it){int v=__builtin_ctzll(it); it&=it-1; c+=chains(out[v]&H,T-1,H);} return c; }
static long pairObjs(int i,int j){ // objects (indep A-sets or TT_T) containing both i and j
  u64 bi=1ULL<<i,bj=1ULL<<j;
  if(!((adj[i]>>j)&1)){ u64 cand=ALLM&~adj[i]&~adj[j]&~bi&~bj; return cntIS(cand,A-2); }
  u64 C=adj[i]&adj[j]; u64 H=C|bi|bj;
  return ttIn(H)-ttIn(H&~bi)-ttIn(H&~bj)+ttIn(H&~bi&~bj);
}
static void setp(int i,int j,int s){ // s: 0 none, 1 i->j, 2 j->i
  u64 bi=1ULL<<i,bj=1ULL<<j;
  out[i]&=~bj; out[j]&=~bi; in_[i]&=~bj; in_[j]&=~bi; adj[i]&=~bj; adj[j]&=~bi;
  if(s==1){out[i]|=bj; in_[j]|=bi; adj[i]|=bj; adj[j]|=bi;}
  if(s==2){out[j]|=bi; in_[i]|=bj; adj[i]|=bj; adj[j]|=bi;}
}
static int getp(int i,int j){ if((out[i]>>j)&1) return 1; if((out[j]>>i)&1) return 2; return 0; }
static long total(){ long c=0; c+=ttIn(ALLM); // independent sets
  for(int v=0;v<N;v++){ u64 cand=ALLM&~adj[v]; cand&= ~((2ULL<<v)-1); c+=cntIS(cand,A-1);} return c; }
int main(int argc,char**argv){ N=atoi(argv[1]);A=atoi(argv[2]);T=atoi(argv[3]); rng+=atoll(argv[4])*0x9E3779B97F4A7C15ULL; long iters=atol(argv[5]);
  ALLM=(N==64)?~0ULL:((1ULL<<N)-1);
  if(argc>6){ FILE*f=fopen(argv[6],"r"); int a,b; while(fscanf(f,"%d %d",&a,&b)==2) if(a<N&&b<N) setp(a,b,1); fclose(f);} 
  else { for(int i=0;i<N;i++) for(int j=i+1;j<N;j++){ int r=xr()%3; setp(i,j,r);} }
  long c=total(); long best=c; static long tabu[64][64]; memset(tabu,0,sizeof tabu);
  int SAMPLE = argc>7? atoi(argv[7]) : 60; int TEN = argc>8? atoi(argv[8]) : 2*N;
  static int bestst[64][64];
  for(int i=0;i<N;i++) for(int j=0;j<N;j++) bestst[i][j]=getp(i,j);
  for(long it=1; it<=iters && c>0; it++){
    int bi=-1,bj=-1,bn=0; long bd=1L<<60;
    for(int sm=0; sm<SAMPLE; sm++){
      int i=xr()%N, j=xr()%N; if(i==j) continue; if(i>j){int t=i;i=j;j=t;}
      int old=getp(i,j); int nw=(old+1+xr()%2)%3;
      long before=pairObjs(i,j); setp(i,j,nw); long after=pairObjs(i,j); setp(i,j,old);
      long d=after-before;
      if(tabu[i][j]>it && c+d>=best) continue;
      if(d<bd){bd=d;bi=i;bj=j;bn=nw;}
    }
    if(bi<0) continue;
    setp(bi,bj,bn); c+=bd; tabu[bi][bj]=it+TEN+(xr()%5);
    if(c<best){ best=c; for(int i=0;i<N;i++) for(int j=0;j<N;j++) bestst[i][j]=getp(i,j); }
  }
  if(c>0){ // restore best
    for(int i=0;i<N;i++) for(int j=i+1;j<N;j++) setp(i,j,bestst[i][j]==1?1:(bestst[i][j]==2?2:0));
    c=best; }
  long chk=total();
  printf("N=%d A=%d T=%d cost=%ld (recount %ld)\n",N,A,T,c,chk);
  if(chk==0){ for(int i=0;i<N;i++) for(int j=0;j<N;j++) if((out[i]>>j)&1) printf("%d %d\n",i,j); }
  return chk!=0; }
