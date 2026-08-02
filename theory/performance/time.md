# Hierarchical timing

Timing is generated from phrase intention down to attacks.

## Time map

Let score time be \(s\) and performed time be

\[
t(s)=\int_0^s \exp\!\bigl(r(u)\bigr)\,du
+\sum_j \mathbf{1}_{s\geq b_j}p_j ,
\]

where \(r(u)\) is a smooth log-rate field and \(p_j\) are boundary pauses.

The rate field is decomposed:

\[
r(s)=r_\text{section}(s)+r_\text{phrase}(s)+
r_\text{gesture}(s)+r_\text{performer}(s).
\]

This guarantees positive time while allowing correlated acceleration and
deceleration.

## Duration model

Cell durations are relative and explicitly composed:

\[
\log d_i=\alpha_C+
\delta_{\text{role}}+\delta_{\text{cadence}}+
\delta_{\text{phrase}}+\delta_{\text{performer}}+\epsilon_s.
\]

Each \(\delta\) is a declared profile formula. \(\epsilon_s\) is bounded seeded
surface variation. Inputs include emphasis, approach, suspension, cadence
strength, repetition, register, ornament, and articulation intent.

## Boundaries

Boundary evidence is computed jointly from:

- silence and final lengthening;
- cadence evidence;
- contour and tonal closure;
- motif completion;
- following-phrase reset;
- performer breathing or physical gesture when available.

Silence is an intentional event with function and uncertainty, not an empty
random interval.

## Repeated attacks

Repeated-note timing has its own articulation state. Its microtiming may become
locally regular even in a free-rhythm performance. It is conditioned on
emphasis, continuation, ornament, and release intent.

## Iqa alignment

When iqa is active, the continuous time map is constrained by cycle position:

\[
t_i=\tau(k_i,b_i)+\Delta_{\text{role},i},
\]

where \(k_i\) is cycle index, \(b_i\) beat role, and \(\Delta\) a declared
profile microtiming formula or bounded table. Phrase grammar remains responsible
for melodic syntax; the iqa layer supplies cyclic rhythmic affordances.

## Current kernel boundary

The first executable seam is intentionally narrower than this model. A
monophonic `performance::Plan` stores contiguous exact-rational event spans;
the pitch request derives each onset from the prior duration and requires an
explicit direction-conditioned `performance::Timing` profile. The current
profile resolves phrase start/body/end rates, bounded seeded duration and
intensity variation, explicit release intents, and intentional boundary pauses.
The checked-in engineering profile is input data, not a maqam default. Smooth
rate fields, ornament/carrier time allocation, breath and cycle alignment,
performer distributions, and measured free-time evidence remain required for
the full performance model.
