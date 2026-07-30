# Hard pitch feasibility

Hard pitch constraints are finite rational affine equalities and non-strict
inequalities over symbolic pitch expressions:

\[
Hx=h,\qquad Lx\leq l.
\]

This layer decides whether the hard region is empty. It does not choose a
preferred point and does not implement soft tiers.

Every equation and inequality has a complete, unique stable identity. Identity
is the machine reference used by contradiction and active-set traces;
provenance remains the human evidence description.

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

## Witness

Each row set is retained immediately before its variable is eliminated. After
the constant rows pass, variables are restored in reverse order. Substituting
already restored variables turns every applicable row into a lower or upper
bound. The witness chooses the greatest lower bound when one exists, otherwise
the least upper bound, otherwise exact zero.

All bounds are closed, finite sets have exact extrema under symbolic pitch
ordering, and the chosen point is checked against every original equality and
inequality before it is returned.

## Resource boundary

Projection may grow exponentially. A declared row budget is checked before
each cross product. A separate retained-stage budget covers reverse
substitution storage, including empty-stage headers. Rational overflow, row
exhaustion, retained-stage exhaustion, and pitch-order proof exhaustion are
distinct typed failures. None is reported as either feasible or infeasible.

## Current boundary

- inequalities are closed and non-strict;
- variables range over the exact symbolic pitch-expression space;
- feasible underdetermined hard regions return a validated exact witness;
- the system solver uses the same witness path for equalities and inequalities;
- lexicographic soft-tier optimization remains separate.

## Downstream breadth

Exact witnesses unlock active-set KKT validation for contextual tendency tiers,
legal trajectory envelopes, register checks, and concrete realizations of
partially specified pitch fields. The next sequence consumes this solver in
the KKT equations for one soft tier rather than adding a separate approximate
constraint engine.

## Laws

- equality is equivalent to two opposing inequalities;
- a closed exact boundary is feasible;
- projection detects contradictions spanning multiple variables;
- symbolic ratios and rational cents retain their exact order;
- declared-variable validation is strict;
- incomplete and duplicate constraint identities are rejected;
- contradiction traces retain stable constraint identities and provenance;
- row, retained-stage, and proof budgets fail explicitly;
- reverse substitution satisfies underdetermined equations and one- or
  two-sided bounds;
- free variables receive a deterministic exact witness;
- an equality solution cannot bypass an incompatible inequality.
