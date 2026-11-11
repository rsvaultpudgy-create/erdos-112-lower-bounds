# Working log: Erdős Problem #112 lower bounds

Workspace: 2 CPU cores, 7 GB RAM, no GPU. Produced by an AI system (Claude, Anthropic).

## Problem 112 (digraph Ramsey numbers k(n,m) = r(I_n, L_m)) -- ATTEMPTED
- Statement: k(n,m) = least k such that every directed graph on k vertices has an independent set of size n or a transitive tournament of size m. "Determine k(n,m)." Open in general.
- Known (literature): r(I_3,L_3)=9 (Bermond 1974), r(I_4,L_3)=15, r(I_5,L_3)=23, r(I_6,L_3) in [23,33] (Ihringer-Rajendraprasad-Weinert, arXiv 1707.09556). Forum (Sep 2026, islomifaridun): k(3,4)=21, 34<=k(4,4)<=50, 31<=k(3,5)<=55.
- Approach: simulated annealing over circulant oriented graphs (p112/sa_circ.c), independent brute-force verifier (p112/check/verify.py, enumerates all A-subsets and T-subsets directly, no symmetry shortcuts).
- Bug caught: first version of the TT counter restricted chains to increasing vertex index; the independent verifier rejected its "witnesses" (N=20, N=21). Fixed; all witnesses below pass the brute-force verifier.
- Results so far (verified): k(6,3) >= 29 (was 23); k(3,5) >= 43 (was 31). See p112/witnesses.txt.
- Wrote exhaustive circulant backtracking search (p112/bt_circ.c; reversal symmetry broken). Reproduces: circulant witness at N=20 for (3,4), none at N=21 (consistent with forum k(3,4)=21).
- Exhaustive circulant results (each hit re-verified by brute force):
  - (n,m)=(4,4): witnesses at N=33,37,39; none at 34-36,38,40-49  => k(4,4) >= 40 (forum had 34).
  - (3,5): largest circulant N=42; none for 43..54  => k(3,5) >= 43 (forum had 31).
  - (6,3): largest circulant N=28; none 29..32 => r(I_6,L_3) >= 29 (IRW had 23; upper bound 33).
  - (7,3): circulants N=30..36, none 37..44 => r(I_7,L_3) >= 37 (upper bound m^2-m+3 = 45).
- Running in background: batch circulant search for (8,3),(5,4),(4,5),(9,3),(6,4); SA over Cayley digraphs on non-cyclic groups (orders up to 64).
- SAT attempt to determine r(I_6,L_3) exactly (is it 29?): CaDiCaL via pysat, case split on vertex-0 degrees with max-degree symmetry breaking (p112/sat/r63.py). Calibrated OK on N=22 (finds the known 22-vertex witness in 8 s). But even N=33 (known UNSAT by the IRW counting bound) did not finish in 10 min, and N=29 case (5,5) ran ~1 h without result. Dropped: plain SAT is too weak here; would need structural case analysis.
- More exhaustive circulant results (64-bit run): (8,3) max N=45 -> r(I_8,L_3) >= 46; (9,3) max N=60 (none 61..65) -> >= 61; (5,4) max N=60 (none 61..64) -> >= 61; (4,5) and (6,4) have circulant witnesses at N=64 (search capped at 64 bits) -> >= 65.

