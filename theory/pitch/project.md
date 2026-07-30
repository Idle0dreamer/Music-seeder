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

The implemented readers are limited to typed state that currently exists:

- root or local center from the nonempty center stack;
- active jins;
- tonicization level through an explicit identity mapping;
- completion state for one named modulation path;
- current structural pitch role;
- current intended motion through an explicit identity mapping;
- current relational register region;
- optional baggage capability and idiomatic gesture family;
- active or current completed phrase function;
- current-event cadence family;
- current completed phrase boundary.

There is still no reader for neighboring-role history, sayr obligation,
performer, instrument, or acoustic context. Each requires its own typed state
and laws before projection can expose it.

## Scope

Projection reads an evaluated snapshot after a grammar term or committed event
stage has completed. A center, jins, tonicization, path, phrase, melody, or
gesture change that a scope does not export therefore cannot leak into pitch
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

This seam lets each committed stage request an exact context-dependent pitch
field using center, jins, tonicization, path, event, phrase, register, baggage,
and gesture state. The descriptor-driven generator consumes those facts across
an ordered multi-event gesture. The next projection expansion is reserved for
typed sayr landmarks; synthesis remains a separate consumer.

## Laws

- reader storage order does not change the context;
- root and local center selection are distinct;
- missing required state fails and missing optional state is omitted;
- explicit tonicization mappings preserve typed levels;
- completed-path projection does not depend on trace replay;
- register, baggage, and gesture facts come from the current structural event;
- phrase cadence and boundary facts remain tied to the current event span;
- non-exported scoped state cannot leak into the projected context;
- the projected context selects and solves a neutral field end to end.
