# Phrase spans and cadence state

A phrase span groups stable structural events under one profile-defined
function. It does not infer phrase identity from final-note height or silence.

## Function and boundary

A phrase function is a typed stable identity supplied by a selected profile:

\[
\phi\in\Phi_\Gamma.
\]

Profiles may define functions such as question, response, suspension, return,
or conclusion. These names are not a closed universal enum and neutral fixtures
do not assert regional authenticity.

A boundary has one universal structural kind:

\[
b\in\{\operatorname{Open},\operatorname{Closed}\}.
\]

`Open` means later grammar remains responsible for closure. `Closed` means the
span must carry at least one declared cadence.

## Active frame

`Begin(i,\phi)` opens:

\[
F=(i,\phi,k,[])
\]

where \(i\) is a unique phrase identity and \(k\) is the current event-history
length. Nested active phrases are rejected at this layer; hierarchy is supplied
by grammar scopes and later phrase groups.

While \(F\) is active, `Cadence(a,e,f)` both contributes exact tonicization
evidence \(e\) and appends:

\[
\kappa=(a,e_i,f)
\]

to the frame, where \(a\) is a profile-permitted cadence-family identity,
\(e_i\) is the current structural-event identity, and \(f>0\) is exact rational
closure strength. One event carries at most one cadence interpretation within a
legal branch; analytical alternatives remain separate branches.

## Completion

`End(i,b)` succeeds only when:

- the active phrase identity is \(i\);
- at least one structural event was placed since `Begin`;
- \(i\) has not already completed;
- a `Closed` boundary has a cadence on its final event.

It produces:

\[
P=(i,\phi,e_k,e_n,b,[\kappa_1,\ldots,\kappa_m])
\]

using stable first and last event identities, clears the active frame, and
appends the span exactly once.

## Scope

Phrase state is an explicit scope component. A non-exporting scope restores the
active frame and completed spans together. Operation trace remains append-only
evaluation evidence.

## Pitch-field projection

Typed readers expose, for the current structural event:

- phrase-function identity;
- latest cadence-family identity when present;
- completed open or closed boundary through explicit mapped identities.

An active frame may supply function and cadence but has no completed boundary.
A completed span is applicable when its last event is the current event.
Required readers fail when no applicable state exists.

Cadence strength remains exact rational phrase state. It is not squeezed into
the categorical field-fact representation. The current pitch seam consumes
function, family, and boundary; a later numeric-context seam must consume
strength explicitly rather than encoding numbers as identity strings.

## Downstream breadth

Stable spans unlock question-response and recall relations, phrase-length and
boundary timing, cadence-conditioned pitch fields, motif occurrence ownership,
section aggregation, closure validation, and event-stage completion. The
immediate consumer is the staged generator: it resolves exact pitch only after
the current event has received its phrase and cadence annotations. Phrase
relations remain the next discourse-level consumer.

## Laws

- phrase identities are complete and unique;
- nested or empty phrase spans fail;
- profile-disallowed functions and cadence families fail;
- closed spans without cadence fail;
- exact cadence evidence and strength remain distinct;
- closed spans require final-event cadence;
- first and last event identities match event history;
- phrase state obeys scope restoration;
- function, cadence, and boundary readers are typed and storage-order stable;
- a completed phrase context selects an exact pitch rule end to end.
