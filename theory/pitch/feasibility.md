# Hard pitch feasibility

Hard pitch constraints are finite rational affine equalities and non-strict
inequalities over symbolic pitch expressions:

\[
Hx=h,\qquad Lx\leq l.
\]

This layer decides whether the hard region is empty. It does not choose a
preferred point and does not implement soft tiers.

## Projection

Each equality is represented by its two opposing inequalities. For a variable
\(x\), partition the current rows into positive, negative, and zero
coefficients. Given

\[
p x+u\leq a,\qquad n x+v\leq b,
\]

where \(p>0\) and \(n<0\), the exact projected row is

\[
(-n)u+pv\leq(-n)a+pb.
\]

Every positive row is paired with every negative row. One-sided rows involving
\(x\) disappear because \(x\) is unbounded in the required direction. Rows
without \(x\) remain. Repeating this for every declared variable is
Fourier--Motzkin elimination.

## Constant proof

After projection, every remaining row has the form \(0\leq c\). The symbolic
pitch ordering layer proves each comparison exactly. A false row returns an
infeasibility report containing the order certificate and the merged
provenance of the constraints that produced it.

## Resource boundary

Projection may grow exponentially. A declared row budget is checked before
each cross product. Rational overflow, row exhaustion, and pitch-order proof
exhaustion are distinct typed failures. None is reported as either feasible or
infeasible.

## Current boundary

- inequalities are closed and non-strict;
- variables range over the exact symbolic pitch-expression space;
- feasibility is general, but witness construction is not yet implemented;
- the equality solver returns a value only when its equality subsystem is
  fully determined and rejects any value that violates an inequality;
- lexicographic soft-tier optimization remains separate.

## Downstream breadth

Feasibility unlocks exact witnesses for underdetermined hard regions, active-set
KKT validation for contextual tendency tiers, legal trajectory envelopes, and
instrument-range checks. The next sequence consumes the retained elimination
stages to construct a witness; soft-tier optimization follows that witness
rather than adding a separate approximate solver.

## Laws

- equality is equivalent to two opposing inequalities;
- a closed exact boundary is feasible;
- projection detects contradictions spanning multiple variables;
- symbolic ratios and rational cents retain their exact order;
- declared-variable validation is strict;
- row and proof budgets fail explicitly;
- an equality solution cannot bypass an incompatible inequality.
