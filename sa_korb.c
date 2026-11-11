// SA over k-orbit circulant oriented graphs: vertices (i,a), i in Z_m, a in [k], N=m*k <= 64.
// usage: sa_korb m k A T seed iters
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
typedef uint64_t u64;
static int m,K,N,A,T,nv; static int vk_a[2048],vk_b[2048],vk_d[2048],st[2048];
static u64 out[64],adj[64],ALLM; static u64 rng=88172645463325252ULL;
static inline u64 xr(){rng^=rng<<13;rng^=rng>>7;rng^=rng<<17;return rng;}
static int Vx(int i,int a){ return a*m+((i%m)+m)%m; }
static void build(){ memset(out,0,sizeof out);
  for(int v=0;v<nv;v++){ if(!st[v]) continue; for(int i=0;i<m;i++){ int u=Vx(i,vk_a[v]), w=Vx(i+vk_d[v],vk_b[v]); if(st[v]==1) out[u]|=1ULL<<w; else out[w]|=1ULL<<u; } }
  for(int i=0;i<N;i++) adj[i]=out[i];
  for(int i=0;i<N;i++){u64 o=out[i]; while(o){int j=__builtin_ctzll(o); o&=o-1; adj[j]|=1ULL<<i;}} }
static long chains(u64 c,int d){ if(!d) return 1; long r=0; u64 it=c; while(it){int v=__builtin_ctzll(it); it&=it-1; r+=chains(c&out[v],d-1);} return r; }
static long cntIS(u64 c,int d){ if(!d) return 1; long r=0; while(c){int v=__builtin_ctzll(c); c&=c-1; r+=cntIS(c&~adj[v],d-1);} return r; }
static long cost(){ long c=0; for(int a=0;a<K;a++){ int v=Vx(0,a); c+=chains(out[v],T-1); c+=cntIS(ALLM&~adj[v]&~(1ULL<<v),A-1);} return c; }
int main(int argc,char**argv){ m=atoi(argv[1]);K=atoi(argv[2]);A=atoi(argv[3]);T=atoi(argv[4]); rng+=atoll(argv[5])*0x9E3779B97F4A7C15ULL; long iters=atol(argv[6]);
  N=m*K; ALLM=(N==64)?~0ULL:((1ULL<<N)-1); nv=0;
  for(int a=0;a<K;a++) for(int d=1; 2*d<m; d++){ vk_a[nv]=a; vk_b[nv]=a; vk_d[nv]=d; nv++; }
  for(int a=0;a<K;a++) for(int b=a+1;b<K;b++) for(int d=0;d<m;d++){ vk_a[nv]=a; vk_b[nv]=b; vk_d[nv]=d; nv++; }
  for(int v=0;v<nv;v++) st[v]=xr()%3;
  build(); long c=cost(), best=c; double temp=1.0;
  for(long it=0; it<iters && c>0; it++){ int v=xr()%nv; int old=st[v]; st[v]=(old+1+xr()%2)%3; build(); long c2=cost();
    if(c2<=c || exp((c-c2)/temp) > (xr()%1000000)/1e6) c=c2; else st[v]=old; if(c<best) best=c; temp*=0.9999; if(temp<0.05) temp=0.8; }
  build(); c=cost(); printf("m=%d k=%d N=%d A=%d T=%d cost=%ld best=%ld\n",m,K,N,A,T,c,best);
  if(c==0){ for(int i=0;i<N;i++) for(int j=0;j<N;j++) if((out[i]>>j)&1) printf("%d %d\n",i,j); }
  return c!=0; }
