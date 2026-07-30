# Exact contextual tendency tier

A contextual tendency target is a stable identity, a rational affine pitch
form \(a_i^\top x\), an exact symbolic target \(b_i\), a positive rational
weight \(w_i\), and provenance. One tier minimizes

\[
F(x)=\sum_i w_i(a_i^\top x-b_i)^2
\]

over the exact hard region.

The implementation never evaluates this expression through binary floating
point and does not multiply two symbolic pitch expressions. Convex optimality
is decided through linear Karush--Kuhn--Tucker conditions.

## Active set

For each candidate subset \(A\) of hard inequalities, constraints in \(A\) are
also imposed as equalities. Equality multipliers are free. Active inequality
multipliers satisfy \(\mu\geq0\). Inactive multipliers are exactly zero.

Stationarity for structural variable \(x_k\) is

\[
2\sum_i w_i a_{ik}(a_i^\top x-b_i)
+\sum_j E_{jk}\lambda_j
+\sum_{r\in A}L_{rk}\mu_r=0.
\]

After expansion this is an affine equation with rational coefficients and a
symbolic pitch-expression right side. The hard witness engine decides the
combined primal, active, stationarity, and dual constraints exactly.

The hard region is polyhedral and the objective is convex, so a feasible KKT
system is sufficient for global optimality. Candidate active sets are ordered
by cardinality and then stable inequality identity, never input position.

## Output

The result contains:

- the exact structural pitch solution;
- the stable identities of active hard inequalities;
- the exact predicted affine value for every tendency identity.

Predictions, rather than a floating objective value, are the authority later
tiers preserve.

## Resource boundary

Active-set enumeration is finite but exponential. A declared set budget limits
the number of KKT candidates. Every candidate also carries the hard solver's
row, retained-stage, rational-arithmetic, and order-proof budgets. Exhaustion
is a typed failure, not an approximate optimum.

## Downstream breadth

One exact tier unlocks contextual center selection inside legal pitch fields,
auditable attraction targets, hard registral interaction, and the
prediction equalities needed for lexicographic later tiers. The next sequence
freezes all stronger-tier predictions before invoking this same tier operator
again.

## Current boundary

- targets are finite weighted affine residuals with strictly positive rational
  weights;
- hard inequalities are closed;
- one tier is solved at a time;
- active-set identity normalization is implemented before broader
  storage-order invariance is claimed;
- no regional tendency values are asserted by neutral tests.
