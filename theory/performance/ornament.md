# Ornament and articulation

An ornament is a grammatical operation with intent, placement conditions, and
an instrument-neutral performance-plan realization.

## Abstract operation

\[
O=(q,h,\pi,\Delta,\chi)
\]

- \(q\): function — approach, emphasis, delay, connection, release, or color;
- \(h\): host structural event;
- \(\pi\): melodic and phrase preconditions;
- \(\Delta\): added or redistributed time;
- \(\chi\): pitch-role trajectory.

The abstract inventory includes:

- slides and stepped connections;
- turns;
- mordent-like neighbor figures;
- anticipation;
- delayed arrival;
- vibrato or pitch fluctuation behavior;
- repeated-note articulation;
- tremolo-like reiteration;
- grace attacks and after-notes.

Names do not imply Western execution. Each profile supplies its own gesture
families and evidence.

## Placement

Ornament admissibility and cost are:

\[
\operatorname{cost}(O\mid h,C)=
\begin{cases}
\infty,&\neg\operatorname{legal}(O,h,C)\\
K_{\Gamma}(O,h,C),&\operatorname{legal}(O,h,C).
\end{cases}
\]

\(K_\Gamma\) is an explicit tiered profile formula over host role, direction,
motif identity, cadence position, duration budget, register, prior ornament
density, and articulation context.

## Structural conservation

The underlying host and its tonal evidence remain identifiable. An ornament may
delay, approach, reiterate, or color a host, but cannot silently replace the
phrase grammar.

## Adapter boundary

The language emits the gesture's melodic, temporal, and articulation intent.
It does not select strings, keys, mallets, samples, or excitation models. A
synthesis adapter must either map the intent explicitly or reject it.

## Interaction limits

- Dense ornaments are constrained by phrase clarity and duration.
- Repeated attacks carry explicit continuation or rearticulation intent.
- Pitch-fluctuation intent is described without assuming a physical mechanism.
- Ornament timing is generated jointly with the carrier gesture.
