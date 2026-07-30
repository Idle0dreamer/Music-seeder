# State projection into pitch context

Pitch-field context is derived from evaluated musical state. It is not copied
from arbitrary caller strings and it is not inferred from the audio layer.

## Projection

Let \(S\) be a completed grammar outcome and let each typed reader be:

\[
p_i:S\rightarrow V_i\cup\{\bot\}.
\]

Each reader owns one stable field-context key \(k_i\). Its emitted fact is:

\[
(k_i,p_i(S))
\]

when a value exists. A required reader fails on \(\bot\); an optional reader
emits nothing. Two readers may not own the same key.

The implemented readers are deliberately limited to state that currently
exists:

- root or local center from the nonempty center stack;
- active jins;
- tonicization level through an explicit identity mapping;
- completion state for one named modulation path.

The projection does not pretend to know direction, phrase function, cadence
function, register, neighboring role, or performer context. Those require
typed state components and operations before they can become readers.

## Scope

Projection occurs after a grammar term has completed. It reads the resulting
snapshot, after lexical scope restoration. A center, jins, tonicization, or
path change that a scope does not export therefore cannot leak into pitch
context.

Trace events are evidence of evaluation, not authority for current context.
Projection never reconstructs discarded state by replaying the trace.

## Identity and ordering

Reader keys, mapped values, and named paths have complete identities. Reader
storage order is normalized by key. The output is the same finite partial
function used by field assembly, so duplicate keys are rejected before a
field is requested.

Projection cardinality has an explicit budget. Exhaustion and missing required
state fail rather than omitting facts silently.

## Downstream breadth

This seam lets complete grammar branches request exact context-dependent pitch
fields using their established center, jins, tonicization, and path history.
The next sequence must add typed structural pitch-event and motion state, then
consume it immediately by projecting direction and role into an
instrument-neutral performance request. Synthesis remains a separate consumer.

## Laws

- reader storage order does not change the context;
- root and local center selection are distinct;
- missing required state fails and missing optional state is omitted;
- explicit tonicization mappings preserve typed levels;
- completed-path projection does not depend on trace replay;
- non-exported scoped state cannot leak into the projected context;
- the projected context selects and solves a neutral field end to end.
