# Contextual pitch-field assembly

A pitch field is assembled from guarded affine rules. It is not selected from
a scale table and it does not use a hidden fallback.

## Context

A context is a finite partial function:

\[
C:K\rightharpoonup V.
\]

Keys may represent direction, jins role, neighboring role, phrase function,
region, repertoire, performer, or another typed distinction. A key has at most
one value in a context. Both key and value have complete stable identities.

A guard is another finite partial function \(G\). It applies exactly when:

\[
G\subseteq C.
\]

An empty guard is unconditional. Guard order and unrelated context facts have
no effect. Missing facts do not act as wildcards.

## Rules

Every rule has a stable identity, provenance, a guard, and exactly one affine
effect:

- hard equality \(a^\top x=b\);
- hard upper bound \(a^\top x\leq b\);
- positive-weight tendency \((t,w,a,b)\) in named tier \(t\).

The right side \(b\) is an exact symbolic pitch expression. The coefficients
and tendency weight are exact rationals. Every variable is a declared tonal
role, not an array index.

Profile reconstruction occurs before field assembly. Assembly has no
"most-specific rule wins" convention. If two reconstructed rules both apply,
both effects are present. An incompatible pair therefore produces an explicit
hard contradiction or an explicit same-tier compromise instead of a hidden
override.

## Authority

The schema declares a tier sequence:

\[
T=(t_1,\ldots,t_m)
\]

from strongest to weakest. Active tendency rules are grouped by tier. Empty
tiers are removed without changing the relative order of active tiers.

Ordering has four distinct semantics:

1. guard applicability is set-like and storage-order invariant;
2. hard effects delimit the legal region before preference;
3. tier order is semantic and lexicographic;
4. target order inside a tier is immaterial because the tier is one weighted
   sum.

Seeded choice is not part of field assembly. It may act only after legality and
authority have produced alternatives.

## Coverage

Every coverage obligation names a nonempty set of rule identities. Assembly
succeeds only if at least one named rule is active:

\[
\forall N_j,\quad N_j\cap A(C)\neq\varnothing.
\]

This makes an intentionally optional rule different from a missing regional,
directional, or functional definition. Failure is explicit; the engine does
not substitute a generic tuning, equal temperament, or nearest available rule.

## Assembly

For validated schema \(P\) and context \(C\), assembly selects:

\[
A(P,C)=\{r\in P.rules\mid guard(r)\subseteq C\}.
\]

The selected rules are identity-normalized, partitioned into equalities,
inequalities, and ordered tendency tiers, and sent unchanged to the exact hard
and lexicographic solvers. The result records the active rule identities and
each tier's exact prediction trace.

Explicit budgets bound context facts, rules, coverage obligations, and solver
proof work. Exhaustion fails rather than truncating the field.

## Downstream breadth

This layer lets one exact solver consume reconstructed rules for direction,
neighbor influence, phrase role, modulation state, region, repertoire, and
performer without hard-coding a scale per combination. Typed state projection
now produces the implemented center, jins, tonicization, and path context.
Instrument mapping remains outside the theory kernel.

## Laws

- context and rule storage order do not change assembly;
- an unrelated context fact does not change assembly;
- a missing guard fact never matches;
- every active rule appears exactly once;
- uncovered obligations fail explicitly;
- duplicate context keys and incomplete identities fail explicitly;
- every target refers to a declared authority tier;
- weaker tiers preserve all stronger predictions;
- a neutral symbolic-ratio fixture is calculated end to end without a scale
  array.
