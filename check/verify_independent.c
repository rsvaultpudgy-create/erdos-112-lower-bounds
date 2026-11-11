// Independent checker (written without reference to the search code or the bundled checkers).
// Input: N A T then arcs "u v" (u->v) on stdin. Verifies: no loops, no 2-cycles, no independent A-set, no transitive T-tournament.
#include <stdio.h>
#include <stdlib.h>
typedef unsigned __int128 u128;
int N,A,T; u128 outm[128], adjm[128];
static int pc(u128 x){return __builtin_popcountll((unsigned long long)x)+__builtin_popcountll((unsigned long long)(x>>64));}
static int low(u128 x){unsigned long long lo=(unsigned long long)x; return lo?__builtin_ctzll(lo):64+__builtin_ctzll((unsigned long long)(x>>64));}
// is there an independent set of size need inside cand?
static int indep(u128 cand,int need){ if(need==0) return 1; if(pc(cand)<need) return 0;
  while(cand){ if(pc(cand)<need) return 0; int v=low(cand); cand&=~((u128)1<<v); if(indep(cand&~adjm[v],need-1)) return 1; } return 0; }
// transitive tournament v1->v2->...: each later vertex is an out-neighbour of all earlier ones
static int trans(u128 cand,int need){ if(need==0) return 1; if(pc(cand)<need) return 0;
  u128 c=cand; while(c){ int v=low(c); c&=~((u128)1<<v); if(trans(cand&outm[v],need-1)) return 1; } return 0; }
int main(){ if(scanf("%d %d %d",&N,&A,&T)!=3) return 2; int u,v; long arcs=0;
  while(scanf("%d %d",&u,&v)==2){ if(u==v||u<0||v<0||u>=N||v>=N){puts("BAD ARC");return 1;} outm[u]|=(u128)1<<v; arcs++; }
  for(u=0;u<N;u++) for(v=0;v<N;v++) if(((outm[u]>>v)&1)&&((outm[v]>>u)&1)){puts("2-CYCLE");return 1;}
  for(u=0;u<N;u++) for(v=0;v<N;v++) if((outm[u]>>v)&1){ adjm[u]|=(u128)1<<v; adjm[v]|=(u128)1<<u; }
  u128 all=((u128)1<<N)-1; int i=indep(all,A), t=trans(all,T);
  printf("N=%d arcs=%ld independent %d-set: %s; transitive %d-tournament: %s => %s\n",N,arcs,A,i?"YES":"no",T,t?"YES":"no",(!i&&!t)?"VALID witness k(A,T)>N":"INVALID"); return (i||t); }
