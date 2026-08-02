# Structural pitch events

A structural event is the smallest identity-bearing melodic intention that may
request pitch. It is not a MIDI note and it contains no instrument action.

## Intent

The \(n\)-th event is:

\[
e_n=(i_n,\rho_n,d_n,r_n,b_n?,\lambda_n?)
\]

where \(i_n\) is a unique stable event identity, \(\rho_n\) is a structural
pitch-role identity, \(r_n\) is a relational register-region identity,
\(b_n?\) is an optional validated baggage capability, \(\lambda_n?\) is the
active gesture occurrence and family, and:

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

For `Place(i,role,direction,region,baggage?)` to succeed:

- every supplied identity is complete;
- an active catalog descriptor exists;
- role and register belong to that descriptor;
- the selected profile permits the role and register;
- optional baggage matches its descriptor role, register, direction, and
  gesture restrictions, and the profile permits it;
- an active gesture advances only when the event matches its next ordered step;
- \(i\) has not appeared in the event history;
- current event and history are internally consistent;
- direction is `Start` iff the history was empty.

The successful transition makes the event current and appends it exactly once.
It contributes exact characteristic-register and baggage evidence after full
conformance. A gesture-bearing event retains occurrence and family identity.
The operation trace remains evaluation evidence; it is not the event history.

## Scope

Melodic event state is one explicit scope component. A scope that does not
export it restores both current event and event history. Trace and decision
metadata remain append-only, so discarded local exploration is observable
without becoming musical state.

## Context projection

Five typed readers expose event context:

- current structural role;
- current intended direction through an explicit identity mapping;
- current relational register region;
- optional baggage capability;
- optional gesture family.

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

The structural `Target` contains event intent and exact structural center. The
kernel now wraps it in `performance::TimedEvent`, carrying exact rational
onset and duration, nonnegative intensity, explicit articulation and release
intent, and a typed strand. `performance::Plan` also carries intentional pause
spans. Event timing is resolved from an external profile using phrase context
and a seeded bounded performer-variation field; it is not a maqam-owned fixed
duration table. Smooth rate fields, measured/free-time realization, and joint
ornament allocation remain required downstream. An explicit history limit
bounds prefix validation before any field or order proof work begins.

## Downstream breadth

Stable descriptor-conformant events now support exact neighbor relationships,
direction-aware intonation, phrase spans, completed gesture spans, baggage and
register evidence, motif boundaries, timing hosts, ornament hosts, and
instrument-neutral plan identity. The next sequence consumes completed phrases,
gestures, and path history as sayr landmarks. Synthesis remains a separate
later consumer.

## Laws

- event identities are unique and append exactly once;
- only the first event may use `Start`;
- descriptor and profile permissions intersect; neither silently widens the
  other;
- invalid baggage and gesture steps fail before state or evidence changes;
- non-exported event state cannot leak through grammar scope;
- role, direction, register, baggage, and gesture projection are
  storage-order invariant;
- a calculated request extends exactly the matching plan prefix;
- `Same`, `Rise`, and `Fall` require matching exact order certificates;
- contradictory intended and calculated motion fails;
- the performance target contains no instrument or waveform state.
