# Jins descriptor

A descriptor is a validated melodic-unit capability graph. It says which
structural roles and behaviors can establish or extend one jins occurrence. It
does not enumerate scale degrees or assign fixed cents values.

## Structure

The executable descriptor is:

\[
J=(j,\tau,G,R,R^\star,F,B,L)
\]

- \(j\): stable descriptor identity;
- \(\tau\): one tonic-role identity;
- \(G\): one or more ghammaz-role identities;
- \(R\): allowed relational register-region identities;
- \(R^\star\subseteq R\): characteristic register regions;
- \(F\): roles whose emphasis contributes jins-establishment evidence;
- \(B\): baggage capabilities;
- \(L\): idiomatic gesture definitions.

The complete role domain \(D_J\) is retained independently. Every role in
\(\{\tau\}\cup G\cup F\) belongs to \(D_J\).

No member of this tuple contains an ordered pitch array. Roles become variables
in the calculated pitch field. Profile- and context-guarded exact equations,
inequalities, and tendencies determine their pitch relations.

## Relational register

A register region is a stable relational identity such as root region,
ghammaz region, or extended upper region. The descriptor declares membership,
not an absolute frequency interval:

\[
r\in R.
\]

Absolute range belongs to the later performance-plan and performer boundary.
An event in \(R^\star\) contributes exact register evidence; an allowed but
non-characteristic region does not.

## Baggage capability

A baggage capability is:

\[
b=(i_b,\rho_b,R_b,D_b,L_b)
\]

- \(i_b\): stable baggage identity;
- \(\rho_b\in D_J\): required structural role;
- \(R_b\subseteq R\): permitted register regions;
- \(D_b\): permitted intended motion directions;
- \(L_b\subseteq L\): gesture families that can carry the baggage.

An empty \(L_b\) means the capability is not restricted to a named gesture.
Declaring baggage on an event contributes baggage evidence only after the full
capability matches. A role outside baggage remains a core or contextual role;
the engine does not infer baggage from pitch height.

## Gesture definition

An idiomatic gesture is not an unconstrained label. It is a finite ordered
relation:

\[
\lambda=(i_\lambda,[s_1,\ldots,s_n])
\]

where each step is:

\[
s_k=(D_k,R_k,M_k,b_k?)
\]

- \(D_k\subseteq D_J\): allowed roles;
- \(R_k\subseteq R\): allowed register regions;
- \(M_k\): allowed intended motion directions;
- \(b_k?\): an optional required baggage capability.

A gesture occurrence has a stable occurrence identity, family identity,
starting event offset, and next expected step. `Begin` opens step one. Each
placed structural event must match the next step and advances exactly once.
`End` succeeds only after every step matched, then records stable first and
last event identities.

Gesture alternatives remain separate grammar branches. A broad step domain is
an explicit profile claim, not an invitation for unrestricted note choice.

## Event conformance

For event

\[
e=(i,\rho,r,d,b?,\lambda?)
\]

under active descriptor \(J\), legality requires:

1. \(\rho\in D_J\);
2. \(r\in R\);
3. when \(b\) exists, its role, register, direction, and gesture conditions
   all match;
4. when a gesture is active, \(e\) matches exactly its next step;
5. the effective profile permits the role, register, baggage, and gesture;
6. structural identity and motion-history laws still hold.

Global profile permission cannot widen the descriptor. Regional or repertoire
profiles may narrow a descriptor capability; widening requires a separately
validated descriptor or sourced exception.

## Catalog invariants

Catalog admission fails when:

- any identity is incomplete or repeated;
- the tonic, ghammaz, emphasis, or baggage role is outside \(D_J\);
- a characteristic or baggage register is outside \(R\);
- a baggage gesture references an absent gesture;
- a gesture is empty;
- a gesture step has no role, register, or direction;
- a step references an absent baggage capability;
- a step's required baggage cannot match any of its role, register, or motion
  alternatives.

Conflict is rejected rather than merged. Descriptor reconstruction and
evidence provenance remain a later profile-data layer; the current catalog
uses neutral fixtures only.

## Downstream breadth

This descriptor unlocks:

- event legality that depends on an actual melodic unit;
- exact register and baggage evidence;
- gesture-progress state and completed gesture spans;
- typed register, baggage, and gesture pitch-field context;
- multi-event generated candidates whose legality changes by profile;
- sayr landmarks stated over completed gestures and characteristic register.

The immediate consumer is staged generation: the neutral travel candidate must
complete one descriptor-owned gesture, calculate every role through contextual
pitch constraints, and disappear when its baggage capability is narrowed by an
effective profile.

## Laws

- descriptor storage order cannot change membership or conformance;
- malformed cross-references fail catalog admission;
- entering an unknown descriptor fails without fallback;
- roles and registers outside the active descriptor fail;
- invalid baggage combinations fail before evidence changes;
- gesture steps advance exactly once and cannot end early;
- completed gesture spans use exact event identities;
- characteristic register and baggage evidence are exact and scoped;
- profile narrowing removes an otherwise descriptor-legal generated candidate;
- exact targets remain field calculations, never descriptor array lookup.
