# Erdős Problem #112: new lower bounds for k(n,m)

Produced entirely by an AI system (Claude, Anthropic). Not checked by a human. Released under CC0.
These are lower bounds for small cases only; the problem remains open.

**Problem.** k(n,m) is the least k such that every directed graph on k vertices contains
either an independent set of size n or a transitive tournament on m vertices
(written r(I_n, L_m) in the literature). Erdős and Rado asked for the values. The problem is open.

**What is here.** Explicit graphs that push up the best known lower bounds for several
small cases. Each graph is a *circulant oriented graph*: the vertices are 0..N-1, and there
is an arc i -> j exactly when (j - i) mod N lies in the listed set S. S never contains both
x and -x, so there are no 2-cycles. A graph with no independent n-set and no transitive
m-tournament on N vertices shows k(n,m) > N.

| (n,m) | new lower bound | witness N | previous best I found | upper bound |
|---|---|---|---|---|
| (4,4) | k(4,4) >= 40 | 39 | 34 (forum, Sep 2026) | 50 (forum) |
| (3,5) | k(3,5) >= 43 | 42 | 31 (forum, Sep 2026) | 55 (forum) |
| (6,3) | k(6,3) >= 30 | 29 (non-circulant, `witness_k63_N29.txt`) | 23 (monotonicity from r(I_5,L_3)=23, IRW) | 33 (IRW) |
| (7,3) | k(7,3) >= 37 | 36 | none tabulated; trivial >= 30 (from the line above) | 45 (IRW) |
| (8,3) | k(8,3) >= 46 | 45 | none tabulated; trivial R(3,8)=28 | 59 (IRW) |
| (9,3) | k(9,3) >= 61 | 60 | none tabulated; trivial R(3,9)=36 | 75 (IRW) |
| (5,4) | k(5,4) >= 61 | 60 | none tabulated; blow-up bound 29 | 95 (recursion below) |
| (4,5) | k(4,5) >= 74 | 73 | none tabulated; blow-up bound 40 | 154 (recursion below) |
| (6,4) | k(6,4) >= 65 | 64 | none tabulated; blow-up bound 36 | 160 (recursion below) |
| (3,6) | k(3,6) >= 68 | 67 | none tabulated; blow-up bound 55 | 137 (recursion below) |

Upper bounds marked "recursion" come from k(n,m) <= 2k(n,m-1) + k(n-1,m) - 1 (stated on the forum;
proof: look at the out-neighbours, in-neighbours and non-neighbours of one vertex) using the
best upper bounds above and k(2,m) = 4, 8, 14, 28 for m = 3..6. They rely on the forum's
claimed k(3,4) = 21. I did not try to improve any upper bound.

"Blow-up bound": replace each vertex of a tournament on r(I_2,L_m)-1 vertices with no
transitive m-subtournament (r(I_2,L_m) = 8, 14, 28 for m = 4, 5, 6) by an independent set of
n-1 vertices. This gives k(n,m) > (n-1)(r(I_2,L_m)-1); I include it only as a baseline.

IRW = Ihringer, Rajendraprasad, Weinert, "New bounds on the Ramsey number r(I_m, L_n)",
arXiv:1707.09556 (they prove r(I_4,L_3)=15, r(I_5,L_3)=23, r(I_m,L_3) <= m^2-m+3 and give a
general upper-bound formula). Forum = erdosproblems.com/forum/thread/112 (post of 22 Sep 2026
claiming k(3,4)=21, 34 <= k(4,4) <= 50, 31 <= k(3,5) <= 55).

## The 29-vertex graph for k(6,3)

The best circulant for (6,3) has 28 vertices (exhaustive search finds none on 29-32, and no
Cayley digraph on the groups I tried does better). A general simulated-annealing search
(`sa_gen.c`, run `./sa_gen 29 6 3 1011 60000000`) found a 29-vertex oriented graph with
independence number 5 and every triangle cyclic. It is not vertex-transitive (out-degrees 4 and 5).
The arc list is `witness_k63_N29.txt`. Check it with
`python3 check/verify_edges_brute.py witness_k63_N29.txt 29 6 3` (pure brute force) or
`python3 check/verify_edges.py witness_k63_N29.txt 29 6 3` (networkx). It cannot be extended by
one more vertex (`extend_edges.py`).

## Witnesses (S lists; arcs i -> i+s mod N)

See `witnesses.txt` (format `N n m : S`). The headline ones:

```
k(4,4) > 39 : N=39  S = 3 5 6 7 10 11 12 14 20 22 24
k(3,5) > 42 : N=42  S = 1 3 7 8 9 11 14 16 30 32 36 37 38 40
k(6,3) > 28 : N=28  S = 3 8 10 12 17   (superseded by the 29-vertex non-circulant graph)
k(7,3) > 36 : N=36  S = 6 8 10 11 13 15
```

## How it was checked

- Found by `bt_circ.c` / `bt_circ128.c` (exhaustive backtracking over circulants).
- Re-checked by two programs written separately from the search code:
  - `check/verify.py`: plain brute force over every n-subset and every m-subset, using no symmetry.
  - `check/verify_nx.py`: networkx exact maximum clique of the complement (independence number)
    plus enumeration of all m-cliques with an acyclicity test.
- `check/verify_independent.c`: a third checker, written in a separate AI session without reference to the search code or the two checkers above. It confirmed every line of `witnesses.txt` and the 29-vertex graph. Usage: `gcc -O2 -o chk check/verify_independent.c`, then feed `N A T` followed by one arc `u v` per line on standard input.
- `python3 verify_all.py` re-runs both on every witness (brute force skipped when it would take
  too long, i.e. for (8,3), (9,3), (6,4), (3,6)). Output: `verify_all.out`.

## Things that did NOT work (so the bounds above may be far from the truth)

- Exhaustive circulant search finds no witnesses at: (4,4) N=34-36, 38, 40-49; (3,5) N=43-54;
  (6,3) N=29-32; (7,3) N=37-44; (8,3) N=46-64; (9,3) N=61-65; (5,4) N=61-64.
- Exhaustive Cayley-digraph search on ~250 non-cyclic groups found nothing better.
- No maximal circulant can be extended by a single extra vertex (SAT check, `extend2.py`).
- A plain SAT attack on k(6,3) did not finish; even N=33, which is impossible by the known
  counting bound, was out of reach in 10 minutes. So no upper bound was improved.
- General simulated annealing on 30 vertices for (6,3) got down to 4 bad sets but never 0.
- Exhaustive search over bi-circulants (Z_15 x {0,1}) on 30 vertices for (6,3): none exist (`bt_bicirc.c`, completed).
- General SA does much worse than circulants for (4,4), (3,5), (7,3).

## Reproduce

```
gcc -O2 -o bt_circ bt_circ.c && ./bt_circ 39 4 4          # prints a witness
python3 check/verify.py 39 4 4 3 5 6 7 10 11 12 14 20 22 24
python3 check/verify_nx.py 39 4 4 3 5 6 7 10 11 12 14 20 22 24
python3 verify_all.py
```
