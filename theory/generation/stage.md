# Staged generation

Generation is an executable boundary between a theory vocabulary and an
instrument-neutral performance plan. It is not audio rendering and it is not a
random walk over pitches.

## Event stage

An event stage is:

\[
G_i=(g_i,[o_1,\ldots,o_n])
\]

where \(g_i\) is a stable identity and the ordered actions contain exactly one
structural `Place`. Actions before it establish context; actions after it may
attach evidence, cadence, phrase boundary, or other vocabulary-defined state.
The stage resolves pitch only after every action succeeds.

Exactly one placed event prevents two invalid shortcuts:

- calculating pitch before its phrase or cadence context exists;
- allowing one stage to commit an unobservable bundle of events.

## Candidate

A candidate is:

\[
C=(c,w,[G_1,\ldots,G_m])
\]

with stable identity \(c\), lexicographic cost \(w\), and one or more stages.
Every stage is evaluated under the selected effective profile. Each committed
event must extend the event history by exactly one and extend the exact
performance-plan prefix by exactly one target.

An unfinished active phrase makes the candidate illegal. Open phrase boundaries
are legal only after the phrase itself has ended.

## Complete evaluation before seed

All candidates are evaluated to either:

- a complete legal state and exact pitch plan; or
- a retained diagnostic naming candidate, stage, and semantic or pitch failure.

Only then may the seed choose among the lowest-cost legal candidates. A cheaper
illegal candidate never participates in the draw. Candidate storage order does
not affect the selected identity.

## Profile reconstruction

Profile patches do not become credible merely because their maps reconstruct.
The same candidate set must be executable under each effective profile. A
prohibition or prerequisite must remove the affected candidate and preserve its
diagnostic; a parameter or preference must cause an inspectable state, target,
or ranking difference.

This supplies the first answer to regional validation: profile knowledge and
generation are developed together. The profile algebra constructs scoped
authority; the generator exposes its musical consequences.

## Theory-language boundary

The staging algorithm does not define maqām, harmony, counterpoint, rhythm, or
another theory. Its current action type is the first compiled vocabulary used
by the maqām module. Before a second theory module is admitted, action
composition must move behind an explicit vocabulary boundary so new theory
operators do not enlarge one monolithic universal variant.

Shared grammar operators are consolidated only when their laws are genuinely
shared. Similar names across theories do not prove identical semantics.

## Typed grammar output

The finite grammar adds two packaging forms without duplicating its algebra:

- `Stage(i,g)` seals the ordered primitive actions emitted by \(g\);
- `Candidate(i,g)` assigns one complete outcome a stable candidate identity.

`Seq`, `Alt`, `Guard`, `Repeat`, `Scope`, and `Bind` keep their existing
semantics. Consequently, branch costs, path guards, lexical bindings, profile
failures, and diagnostics are resolved by one grammar. The generator compiles
only complete sealed outcomes and independently replays them to construct exact
performance plans.

## Downstream breadth

Staged generation unlocks:

- executable profile comparison;
- phrase-complete exact event targets;
- seed-stable full-candidate choice;
- later grammar production expansion into candidates;
- event-by-event sayr and motif obligation checks;
- a complete instrument-neutral plan for timing and gesture layers.

Typed grammar derivation now consumes stages directly. The next musical
sequence must use this executable path for a full jins melodic-unit descriptor,
so tonic, ghammaz, register, emphasis, baggage, and gesture permissions affect
generated event legality rather than remaining inert catalog data.

## Laws

- candidate and stage identities are complete and unique;
- each stage contains exactly one structural event;
- every legal stage extends state and target history by one;
- incomplete phrases reject the candidate;
- all candidates are evaluated before seeded choice;
- illegal candidates cannot influence the seed;
- candidate storage order cannot influence the seed;
- budgets fail explicitly;
- effective profile differences change executable legal outcomes.
