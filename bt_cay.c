// Exhaustive backtracking over Cayley oriented graphs Cay(G,S), arc g -> g*s, S ∩ S^-1 = ∅.
// usage: bt_cay groupfile A T  ; prints first FOUND or "none"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
typedef uint64_t u64;
static int N,A,T,mul[64][64],inv_[64],np,pr[64],st[64]; static long nodes=0;
static u64 out[64],maybe[64],ALLM;
static long cntTT(u64 cand,int d){ if(!d) return 1; u64 it=cand; while(it){int v=__builtin_ctzll(it); it&=it-1; if(cntTT(cand&out[v],d-1)) return 1;} return 0; }
static long cntIS(u64 cand,int d){ if(!d) return 1; while(cand){int v=__builtin_ctzll(cand); cand&=cand-1; if(cntIS(cand&~maybe[v],d-1)) return 1;} return 0; }
static int bad(int upto){
  for(int g=0;g<N;g++){ out[g]=0; maybe[g]=0; }
  for(int p=0;p<np;p++){ int s=pr[p], si=inv_[s];
    if(p<=upto){ if(st[p]==0) continue; int a = st[p]==1? s: si;
      for(int g=0;g<N;g++){ out[g]|=1ULL<<mul[g][a]; maybe[g]|=(1ULL<<mul[g][s])|(1ULL<<mul[g][si]); } }
    else for(int g=0;g<N;g++) maybe[g]|=(1ULL<<mul[g][s])|(1ULL<<mul[g][si]); }
  if(cntTT(out[0],T-1)) return 1;
  if(cntIS(ALLM&~maybe[0]&~1ULL,A-1)) return 1; return 0; }
static void rec(int p,int used){ nodes++;
  if(p==np){ printf("FOUND N=%d A=%d T=%d S=",N,A,T); for(int q=0;q<np;q++) if(st[q]) printf("%d ", st[q]==1?pr[q]:inv_[pr[q]]); printf("\n"); exit(0); }
  for(int v=0;v<3;v++){ if(v==2&&!used) continue; st[p]=v; if(!bad(p)) rec(p+1,used||v); } st[p]=0; }
int main(int argc,char**argv){ FILE*f=fopen(argv[1],"r"); if(fscanf(f,"%d",&N)!=1) return 2;
  for(int i=0;i<N;i++)for(int j=0;j<N;j++) if(fscanf(f,"%d",&mul[i][j])!=1) return 2;
  A=atoi(argv[2]);T=atoi(argv[3]); ALLM=(N==64)?~0ULL:((1ULL<<N)-1);
  for(int i=0;i<N;i++)for(int j=0;j<N;j++) if(mul[i][j]==0) inv_[i]=j;
  int seen[64]={0}; np=0; for(int s=1;s<N;s++){ if(seen[s]||inv_[s]==s) continue; seen[s]=seen[inv_[s]]=1; pr[np++]=s; }
  rec(0,0); fprintf(stderr,"none N=%d A=%d T=%d nodes=%ld file=%s\n",N,A,T,nodes,argv[1]); return 1; }
