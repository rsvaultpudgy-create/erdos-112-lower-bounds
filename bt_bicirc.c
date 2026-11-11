// Exhaustive backtracking over bi-circulant oriented graphs on Z_n x {0,1} (N=2n vertices).
// Variables: S0[x], S1[x] for x=1..floor((n-1)/2) (state 0 none, 1 +x, 2 -x), B[d] for d in Z_n
// (pair (i,0),(i+d,1): 0 none, 1 (i,0)->(i+d,1), 2 reverse).
// Finds first graph with no independent A-set and no transitive T-tournament.
// usage: bt_bicirc n A T
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
typedef unsigned __int128 u128;
static inline int ctz128(u128 x){ unsigned long long lo=(unsigned long long)x; return lo? __builtin_ctzll(lo) : 64+__builtin_ctzll((unsigned long long)(x>>64)); }
static int n,N,A,T,H,NV; static int var_kind[256], var_val[256], st[256]; static long nodes=0;
static u128 out[128], maybe[128], ALLM;
static inline int V(int i,int a){ return a*n + ((i%n)+n)%n; }
static void addarc(int u,int v){ out[u]|=((u128)1)<<v; maybe[u]|=((u128)1)<<v; maybe[v]|=((u128)1)<<u; }
static void addmaybe(int u,int v){ maybe[u]|=((u128)1)<<v; maybe[v]|=((u128)1)<<u; }
static void build(int upto){
  memset(out,0,sizeof out); memset(maybe,0,sizeof maybe);
  for(int k=0;k<NV;k++){ int kind=var_kind[k], x=var_val[k]; int s = k<=upto ? st[k] : -1;
    for(int i=0;i<n;i++){
      int u,v;
      if(kind<2){ u=V(i,kind); v=V(i+x,kind); }
      else { u=V(i,0); v=V(i+x,1); }
      if(s==-1) addmaybe(u,v); else if(s==1) addarc(u,v); else if(s==2) addarc(v,u);
    } }
}
static int cntTT(u128 cand,int d){ if(!d) return 1; u128 it=cand; while(it){int v=ctz128(it); it&=it-1; if(cntTT(cand&out[v],d-1)) return 1;} return 0; }
static int cntIS(u128 cand,int d){ if(!d) return 1; while(cand){int v=ctz128(cand); cand&=cand-1; if(cntIS(cand&~maybe[v],d-1)) return 1;} return 0; }
static int bad(int upto){ build(upto);
  for(int a=0;a<2;a++){ int v=V(0,a);
    if(cntTT(out[v],T-1)) return 1;
    if(cntIS(ALLM&~maybe[v]&~(((u128)1)<<v),A-1)) return 1; }
  return 0; }
static void rec(int k){ nodes++;
  if(k==NV){ printf("FOUND n=%d N=%d A=%d T=%d ",n,N,A,T);
    printf("S0="); for(int j=0;j<NV;j++) if(var_kind[j]==0&&st[j]) printf("%d,", st[j]==1?var_val[j]:n-var_val[j]);
    printf(" S1="); for(int j=0;j<NV;j++) if(var_kind[j]==1&&st[j]) printf("%d,", st[j]==1?var_val[j]:n-var_val[j]);
    printf(" B01="); for(int j=0;j<NV;j++) if(var_kind[j]==2&&st[j]==1) printf("%d,",var_val[j]);
    printf(" B10="); for(int j=0;j<NV;j++) if(var_kind[j]==2&&st[j]==2) printf("%d,",var_val[j]);
    printf("\n"); fflush(stdout); exit(0); }
  for(int v=0;v<3;v++){ st[k]=v; if(!bad(k)) rec(k+1); } st[k]=0; }
int main(int argc,char**argv){ n=atoi(argv[1]);A=atoi(argv[2]);T=atoi(argv[3]); N=2*n; ALLM=(N==128)?~(u128)0:((((u128)1)<<N)-1);
  H=(n%2==0)? n/2-1 : (n-1)/2; NV=0;
  // interleave variables: B first (more coupling), then S0,S1
  for(int d=0; d<n; d++){ var_kind[NV]=2; var_val[NV]=d; NV++; }
  for(int x=1;x<=H;x++){ var_kind[NV]=0; var_val[NV]=x; NV++; var_kind[NV]=1; var_val[NV]=x; NV++; }
  rec(0); fprintf(stderr,"none n=%d N=%d A=%d T=%d nodes=%ld\n",n,N,A,T,nodes); return 1; }
