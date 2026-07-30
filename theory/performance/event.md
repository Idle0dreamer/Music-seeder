# Structural pitch events

A structural event is the smallest identity-bearing melodic intention that may
request pitch. It is not a MIDI note and it contains no instrument action.

## Intent

The \(n\)-th event is:

\[
e_n=(i_n,\rho_n,d_n)
\]

where \(i_n\) is a unique stable event identity, \(\rho_n\) is a structural
pitch-role identity, and:

\[
d_n\in\{\operatorname{Start},\operatorname{Same},
\operatorname{Rise},\operatorname{Fall}\}.
\]

Direction is intended contour relative to the preceding realized structural
event. `Start` is valid exactly when there is no preceding event. Later events
may not use `Start`.

## State transition

Musical state carries:

\[
U_n=(e_n,[e_1,\ldots,e_n]).
\]

For `Place(i,r,d)` to succeed:

- \(i\) and \(r\) have complete identities;
- the selected profile permits role \(r\);
- \(i\) has not appeared in the event history;
- current event and history are internally consistent;
- \(d=\operatorname{Start}\) iff the history was empty.

The successful transition makes the event current and appends it exactly once.
The operation trace remains evaluation evidence; it is not the event history.

## Scope

Melodic event state is one explicit scope component. A scope that does not
export it restores both current event and event history. Trace and decision
metadata remain append-only, so discarded local exploration is observable
without becoming musical state.

## Context projection

Two typed readers extend pitch-field projection:

- current structural role;
- current intended direction through an explicit identity mapping.

Missing current event state follows the same required-or-optional rule as other
projection sources. Reader storage order remains immaterial.

## Exact pitch request

A pitch request consumes:

- one completed grammar snapshot;
- a typed projection plan;
- one contextual field schema;
- an instrument-neutral plan containing the already calculated prefix.

The prefix must correspond exactly to the event-history prefix. Field assembly
and lexicographic solving produce the current role's exact structural center
\(c_n\). For \(n>1\), exact symbolic ordering must prove:

\[
\begin{array}{ll}
d_n=\operatorname{Same} &\Rightarrow c_n=c_{n-1},\\
d_n=\operatorname{Rise} &\Rightarrow c_n>c_{n-1},\\
d_n=\operatorname{Fall} &\Rightarrow c_n<c_{n-1}.
\end{array}
\]

The order certificate is retained with the request result. A missing prefix,
missing role solution, proof-budget exhaustion, or direction mismatch fails
explicitly.

The output target contains event intent and exact structural center. It does
not yet claim a trajectory, duration, articulation, or acoustic realization.
An explicit history limit bounds prefix validation before any field or order
proof work begins.

## Downstream breadth

Stable structural events unlock exact neighbor relationships, direction-aware
intonation, phrase spans, motif occurrence boundaries, timing hosts, ornament
hosts, and instrument-neutral plan identity. The next sequence consumes this
leverage by defining phrase-function and cadence state over event spans and
projecting it into the pitch field. Synthesis remains a separate later
consumer.

## Laws

- event identities are unique and append exactly once;
- only the first event may use `Start`;
- non-exported event state cannot leak through grammar scope;
- role and direction projection are storage-order invariant;
- a calculated request extends exactly the matching plan prefix;
- `Same`, `Rise`, and `Fall` require matching exact order certificates;
- contradictory intended and calculated motion fails;
- the performance target contains no instrument or waveform state.
