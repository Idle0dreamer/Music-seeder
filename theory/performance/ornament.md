# Ornament and articulation

An ornament is a grammatical operation with intent, placement conditions, and
an instrument-specific realization.

## Abstract operation

\[
O=(q,h,\pi,\Delta,\chi,\nu)
\]

- \(q\): function — approach, emphasis, delay, connection, release, or color;
- \(h\): host structural event;
- \(\pi\): melodic and phrase preconditions;
- \(\Delta\): added or redistributed time;
- \(\chi\): pitch-role trajectory;
- \(\nu\): realization map for an instrument and technique.

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
density, and instrument state.

## Structural conservation

The underlying host and its tonal evidence remain identifiable. An ornament may
delay, approach, reiterate, or color a host, but cannot silently replace the
phrase grammar.

## Instrument mapping

A continuous vocal or bowed slide may be physically unavailable on struck
dulcimer. A profile may realize its connecting intent as a rapid stepped sweep,
alternating courses, a grace-note approach, or omit it. This mapping is declared
and documented; it is not a generic MIDI effect.

## Interaction limits

- Dense ornaments are penalized when resonant masking is high.
- Repeated attacks interact with remaining string displacement and damping.
- Vibrato-like modulation is allowed only where the instrument technique
  supports it.
- Ornament timing is generated jointly with the carrier gesture.
