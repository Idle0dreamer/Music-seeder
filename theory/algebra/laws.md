# Algebraic laws

These laws are normative and must have executable property tests.

## Grammar laws

\[
\begin{aligned}
(a;b);c &= a;(b;c) && \text{sequence associativity}\\
\operatorname{Id};a &= a = a;\operatorname{Id}
  && \text{identity}\\
\operatorname{Fail};a &= \operatorname{Fail}
  && \text{left annihilation}\\
\operatorname{Guard}(p,\operatorname{Guard}(q,a))
&=\operatorname{Guard}(p\land q,a)
  && \text{guard conjunction}.
\end{aligned}
\]

`Alt` is associative after alternative identities are normalized. It is not
silently idempotent: two identical-looking alternatives with different
provenance remain distinguishable until normalization explicitly merges them.

## Scope laws

For state components not exported by scope \(s\):

\[
\pi_{\neg s}(\operatorname{Scope}(s,g)(\Sigma))=\pi_{\neg s}(\Sigma).
\]

A phrase-local suspension, temporary rhythmic context, or ornament host cannot
leak into its parent scope.

Generated output obeys the same explicit rule. A non-exporting scope may retain
append-only trace evidence, but its actions and event stages cannot enter a
candidate. Phrase export additionally requires melody export so span identities
cannot dangle. Gesture export has the same melody dependency.

## Generation laws

- Every sealed stage contains exactly one structural event.
- Every candidate action belongs to exactly one stage.
- Candidate and stage identities are unique within a generation request.
- All grammar outcomes and candidate programs are evaluated before seeding.
- Profile-illegal candidates retain diagnostics and never enter the draw.
- Same profile, grammar, seed, and request select the same candidate regardless
  of branch or candidate storage order.

## Tonal laws

- Entering, emphasizing, dwelling, and placing require a validated active jins
  descriptor; no missing-catalog fallback exists.
- Descriptor role, register, baggage, and gesture domains intersect effective
  profile permissions.
- Ordered gesture steps advance once, cannot end early, and retain exact event
  span identities.
- Color does not change the principal center.
- Tonicization is monotone in accumulated positive evidence within a scope.
- A stronger tonicization level implies all lower evidence thresholds, but not
  necessarily the same cadence label.
- Return restores a center already on the center stack.
- Modulation path concatenation is legal only when endpoints and
  path-conditioned prerequisites match.

## Seed laws

1. Same normalized profile, grammar, seed, and request produce the same trace.
2. Reordering alternatives in storage does not change the selected identity.
3. Adding an ineligible alternative does not change the result.
4. A module's draw identity is independent of unrelated module evaluation.

## Profile laws

- Applying the empty patch is identity.
- Patch order is associative only for disjoint keys or explicitly commuting
  refinements.
- Conflicting writes without an explicit resolver fail.
- A refinement may narrow a legal domain but may not widen it.
- A sourced exception may widen a domain only through `AddException`.
- Removing a parent rule requires an explicit prohibition with provenance.
- Normalization is idempotent:

\[
N(N(P))=N(P).
\]

## Pitch laws

- Translating every pitch variable and anchor by \(k\) translates the solution
  by \(k\).
- Interval constraints are invariant under common translation.
- A connected hard-constraint component with an anchor has at most one exact
  solution.
- Contradictory hard constraints fail with a minimal conflicting subset where
  practical; they are never averaged.
