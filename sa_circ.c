// Simulated annealing search for circulant oriented graphs on Z_N with
// no independent set of size A and no transitive tournament of size T.
// Arc i->j iff (j-i) mod N in S, where S ∩ -S = empty.
// usage: sa_circ N A T seed iters
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
typedef uint64_t u64;
static int N,A,T;
static u64 out[64], in_[64], adj[64];
static int st[64]; // st[x] for x in 1..N/2: 0 absent, 1 x in S, 2 -x in S (N-x)
static u64 rng=88172645463325252ULL;
static inline u64 xr(){rng^=rng<<13;rng^=rng>>7;rng^=rng<<17;return rng;}
static void build(){
  memset(out,0,sizeof out);memset(in_,0,sizeof in_);
  for(int x=1;x<=N/2;x++){ if(2*x==N && st[x]) { /* x==-x: cannot orient */ }
    if(st[x]==0) continue; int d = st[x]==1? x : N-x;
    for(int i=0;i<N;i++){int j=(i+d)%N; out[i]|=1ULL<<j; in_[j]|=1ULL<<i;}
  }
  for(int i=0;i<N;i++) adj[i]=out[i]|in_[i];
}
static long cntTT(u64 cand,int depth){ // count chains of remaining length depth from cand
  if(depth==0) return 1; long c=0;
  u64 it=cand; while(it){int v=__builtin_ctzll(it); it&=it-1; c+=cntTT(cand&out[v],depth-1);} return c; }
static long cntIS(u64 cand,int depth){
  if(depth==0) return 1; long c=0;
  while(cand){int v=__builtin_ctzll(cand); cand&=cand-1; c+=cntIS(cand&~adj[v],depth-1);} return c; }
static long cost(){
  u64 all = (N==64)?~0ULL:((1ULL<<N)-1);
  long a=cntTT(out[0],T-1);
  long b=cntIS(all&~adj[0]&~1ULL,A-1);
  return a+b;
}
int main(int argc,char**argv){
  N=atoi(argv[1]);A=atoi(argv[2]);T=atoi(argv[3]);rng+=atoll(argv[4])*0x9E3779B97F4A7C15ULL; long iters=atol(argv[5]);
  int H=N/2; int maxx = (N%2==0)? H-1 : H; // x=N/2 cannot be oriented (x==-x)
  for(int x=1;x<=maxx;x++) st[x]=xr()%3;
  build(); long c=cost(); long best=c; double temp=2.0;
  for(long it=0;it<iters && c>0;it++){
    int x=1+xr()%maxx; int old=st[x]; int nw=(old+1+xr()%2)%3; st[x]=nw; build(); long c2=cost();
    if(c2<=c || exp((c-c2)/temp) > (xr()%1000000)/1e6){ c=c2; } else { st[x]=old; }
    temp*=0.99995; if(temp<0.05) temp=1.5;
    if(c<best){best=c;}
  }
  build(); c=cost();
  printf("N=%d A=%d T=%d cost=%ld S=",N,A,T,c);
  for(int x=1;x<=maxx;x++) if(st[x]) printf("%d ", st[x]==1?x:N-x);
  printf("\n"); return c!=0;
}
