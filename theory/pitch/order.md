# Exact pitch ordering

Pitch inequalities require a total order over symbolic pitch expressions before
they can be admitted as hard constraints. Binary floating point is not an
ordering authority.

## Normal form

For expressions \(a\) and \(b\), form \(d=a-b\). Fold the rational cents offset
\(c\) into the exponent of prime two:

\[
d=1200\log_2\left(\prod_p p^{q_p}\right),\qquad
q_2\mathrel{+}=\frac{c}{1200}.
\]

All \(q_p\) are reduced rationals. Zero exponents are removed. The expressions
are equal exactly when the resulting map is empty.

## Integer certificate

Let \(L\) be the least common multiple of all exponent denominators and
\(n_p=Lq_p\). Then

\[
\operatorname{sign}(d)=
\operatorname{sign}\left(
\prod_{n_p>0}p^{n_p}
-
\prod_{n_p<0}p^{-n_p}
\right).
\]

Both products are non-negative arbitrary-precision integers. The comparison is
exact. Unique prime factorization proves that a non-empty normalized exponent
map cannot compare equal.

A certificate contains the normalized rational exponents, \(L\), both integer
products, and the resulting relation. Verification reconstructs the products
from the normalized map. Prime-basis validation uses a deterministic
Miller--Rabin witness set covering the signed 64-bit input domain.

## Resource boundary

Exactness does not imply unbounded resource use. LCM overflow, an exponent above
the declared limit, or a product above the declared bit limit returns a typed
proof-budget error. It never returns an approximate relation or silently calls
the floating-point realization method.

Certified interval logarithms may later extend the practical range. They are an
additional proof backend, not permission to guess when an interval contains
zero.

## Downstream breadth

This order turns symbolic pitch expressions into lawful comparison operands. It
unlocks hard register and course bounds, pitch-role ordering, monotone
trajectory guards, inequality feasibility, and the primal/dual sign checks
needed by exact constrained optimization.

## Laws

- antisymmetry;
- transitivity;
- equality after ratio refactoring;
- invariance under adding a common pitch expression;
- reversal under argument exchange;
- correct signs for deliberately close rational intervals;
- certificate reconstruction;
- explicit proof-budget failure.
