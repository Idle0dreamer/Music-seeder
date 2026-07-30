# Regional reconstruction

Regional variants are reconstructed by composing declarative refinements over
shared operator semantics.

## Lineage

\[
P_\text{effective}=
N(P_0\triangleleft\Delta_\text{tradition}
\triangleleft\Delta_\text{region}
\triangleleft\Delta_\text{school}
\triangleleft\Delta_\text{repertoire}
\triangleleft\Delta_\text{performer}
\triangleleft\Delta_\text{instrument}).
\]

\(P_0\) defines types and operator meanings, not musical defaults. Each
\(\Delta\) contains sourced rule operations.

## Patch primitives

- `Define(key,value)` — introduce a previously absent rule.
- `Refine(key,domain)` — narrow the legal domain.
- `Prefer(key,orderedAlternatives)` — set an explicit choice order or costs.
- `Parameterize(key,formula/constants)` — provide declared numeric behavior.
- `Forbid(key,predicate)` — add a negative constraint.
- `AddException(key,value,scope)` — explicitly widen within a named scope.
- `Resolve(key,parentChoice)` — settle a multiple-parent conflict.

Blind replacement is not part of the language.

## Multiple parents

Profile lineage is a directed acyclic graph. Parent profiles are normalized
first. For each rule key:

1. identical normalized values merge;
2. compatible refinements intersect;
3. disjoint additions unite;
4. incompatible values produce a conflict;
5. only a child `Resolve` operation can settle that conflict.

The topological order therefore cannot silently decide musical truth.

## Reconstruction result

Completion produces:

\[
\operatorname{Complete}(P)=
(R^+,R^-,F,K,V,C).
\]

- \(R^+\): permitted rules;
- \(R^-\): explicit prohibitions;
- \(F\): formulas and constants;
- \(K\): choice costs and tiers;
- \(V\): provenance and evidence status;
- \(C\): coverage and unresolved requirements.

A request is accepted only if all rule keys it can reach are complete. Missing
regional information has no generic fallback.

## What remains common

Regions share:

- sorts and identities;
- primitive operator meanings;
- combinator semantics;
- profile-patch operations;
- exact constraint and conflict rules;
- validation laws;
- deterministic seed semantics.

They may differ in every musically substantive domain: jins placement,
intonation formulas, baggage, paths, evidence thresholds, cell vocabulary,
cadences, sayr, timing, ornament, form, and instrument technique.

