// Exhaustive backtracking over circulant oriented graphs on Z_N (arc i->j iff j-i in S).
// Finds all (or first) S with no independent A-set and no transitive T-tournament.
// Symmetry: S -> -S (reversal) broken by requiring the smallest used difference x to be oriented +x.
// usage: bt_circ N A T [all=0/1]
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
typedef unsigned __int128 u64;
static inline int ctz128(u64 x){ unsigned long long lo=(unsigned long long)x; return lo? __builtin_ctzll(lo) : 64+__builtin_ctzll((unsigned long long)(x>>64)); }
static int N,A,T,H,ALL; static int st[128]; static long nodes=0, found=0;
static u64 out[128], maybe[128]; // out: present arcs; maybe: adjacency possibly present (present or undecided)
static u64 ALLM;
static inline u64 rot(u64 m,int k){ // rotate N-bit mask left by k
  k%=N; if(!k) return m; return ((m<<k)|(m>>(N-k)))&ALLM; }
static long cntTT(u64 cand,int depth){ if(depth==0) return 1; long c=0; u64 it=cand;
  while(it){int v=ctz128(it); it&=it-1; c+=cntTT(cand&out[v],depth-1); if(c) return c;} return c; }
static long cntIS(u64 cand,int depth){ if(depth==0) return 1; long c=0;
  while(cand){int v=ctz128(cand); cand&=cand-1; c+=cntIS(cand&~maybe[v],depth-1); if(c) return c;} return c; }
static void build(int upto){ // decided: x<=upto ; undecided x>upto counted as maybe
  u64 o0=0, m0=0;
  for(int x=1;x<=H;x++){
    if(x<=upto){ if(st[x]==1){o0|=((u64)1)<<x; m0|=(((u64)1)<<x)|(((u64)1)<<(N-x));} else if(st[x]==2){o0|=((u64)1)<<(N-x); m0|=(((u64)1)<<x)|(((u64)1)<<(N-x));} }
    else { m0|=(((u64)1)<<x)|(((u64)1)<<((N-x)%N)); }
  }
  if(N%2==0) m0 &= ~(((u64)1)<<(N/2)); // N/2 can never be used
  for(int i=0;i<N;i++){ out[i]=rot(o0,i); maybe[i]=rot(m0,i); }
}
static int bad(int upto){ build(upto);
  if(cntTT(out[0],T-1)) return 1;
  if(cntIS(ALLM&~maybe[0]&~((u64)1),A-1)) return 1; return 0; }
static void rec(int x,int used){
  nodes++;
  if(x>H){ found++; printf("FOUND N=%d A=%d T=%d S=",N,A,T); for(int y=1;y<=H;y++) if(st[y]) printf("%d ",st[y]==1?y:N-y); printf("\n"); fflush(stdout); if(!ALL) exit(0); return; }
  for(int v=0;v<3;v++){
    if(v==2 && !used) continue; // reversal symmetry
    st[x]=v; if(!bad(x)) rec(x+1, used|| v!=0);
  }
  st[x]=0;
}
int main(int argc,char**argv){ N=atoi(argv[1]);A=atoi(argv[2]);T=atoi(argv[3]); ALL=argc>4?atoi(argv[4]):0;
  ALLM=(N==64)?~0ULL:((((u64)1)<<N)-1); H=(N%2==0)? N/2-1 : (N-1)/2;
  rec(1,0); fprintf(stderr,"N=%d A=%d T=%d nodes=%ld found=%ld\n",N,A,T,nodes,found); return 0; }
