// SA over Cayley oriented graphs Cay(G,S): arc g -> g*s, S ∩ S^-1 = empty.
// usage: sa_cay groupfile A T seed iters
// groupfile: first line N, then N lines of N ints: mult[i][j] = i*j ; element 0 = identity.
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
typedef uint64_t u64;
static int N,A,T, mul[64][64], inv[64];
static u64 out[64], adj[64];
static int npairs, pr[64]; // representative s of each pair {s,s^-1}, s != s^-1
static int st[64];
static u64 rng=88172645463325252ULL;
static inline u64 xr(){rng^=rng<<13;rng^=rng>>7;rng^=rng<<17;return rng;}
static void build(){
  memset(out,0,sizeof out);
  for(int p=0;p<npairs;p++){ if(!st[p]) continue; int s= st[p]==1? pr[p] : inv[pr[p]];
    for(int g=0;g<N;g++) out[g]|=1ULL<<mul[g][s]; }
  for(int g=0;g<N;g++) adj[g]=out[g];
  for(int g=0;g<N;g++){u64 o=out[g]; while(o){int h=__builtin_ctzll(o);o&=o-1; adj[h]|=1ULL<<g;}}
}
static long cntTT(u64 cand,int depth){ if(depth==0) return 1; long c=0; u64 it=cand;
  while(it){int v=__builtin_ctzll(it); it&=it-1; c+=cntTT(cand&out[v],depth-1);} return c; }
static long cntIS(u64 cand,int depth){ if(depth==0) return 1; long c=0;
  while(cand){int v=__builtin_ctzll(cand); cand&=cand-1; c+=cntIS(cand&~adj[v],depth-1);} return c; }
static long cost(){ u64 all=(N==64)?~0ULL:((1ULL<<N)-1);
  return cntTT(out[0],T-1)+cntIS(all&~adj[0]&~1ULL,A-1); }
int main(int argc,char**argv){
  FILE*f=fopen(argv[1],"r"); if(fscanf(f,"%d",&N)!=1) return 2;
  for(int i=0;i<N;i++)for(int j=0;j<N;j++) if(fscanf(f,"%d",&mul[i][j])!=1) return 2;
  A=atoi(argv[2]);T=atoi(argv[3]);rng+=atoll(argv[4])*0x9E3779B97F4A7C15ULL; long iters=atol(argv[5]);
  for(int i=0;i<N;i++) for(int j=0;j<N;j++) if(mul[i][j]==0) inv[i]=j;
  int seen[64]={0}; npairs=0;
  for(int s=1;s<N;s++){ if(seen[s]||inv[s]==s) continue; seen[s]=seen[inv[s]]=1; pr[npairs++]=s; }
  for(int p=0;p<npairs;p++) st[p]=xr()%3;
  build(); long c=cost(); double temp=2.0;
  for(long it=0;it<iters && c>0;it++){
    int p=xr()%npairs; int old=st[p]; st[p]=(old+1+xr()%2)%3; build(); long c2=cost();
    if(c2<=c || exp((c-c2)/temp) > (xr()%1000000)/1e6) c=c2; else st[p]=old;
    temp*=0.99995; if(temp<0.05) temp=1.5;
  }
  build(); c=cost();
  printf("G=%s N=%d A=%d T=%d cost=%ld S=",argv[1],N,A,T,c);
  for(int p=0;p<npairs;p++) if(st[p]) printf("%d ", st[p]==1?pr[p]:inv[pr[p]]);
  printf("\n"); return c!=0;
}
