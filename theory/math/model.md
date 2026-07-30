# Unified constraint model

The engine combines a typed term grammar, exact regional profile composition,
constraint-based pitch realization, explicit timing formulas, and a physical
instrument model. It is not a neural or backpropagation system.

## Objects

Let:

- \(G\): normalized grammar term;
- \(\Gamma\): completed regional/repertoire/performer profile;
- \(\Sigma_0\): initial typed state;
- \(R\): request and finite resource budget;
- \(s\): seed;
- \(D\): derivation trace;
- \(X\): solved performance parameters;
- \(W\): rendered waveform.

## Derivation

\[
\mathcal{D}
=\llbracket G\rrbracket_\Gamma(\Sigma_0,R)
=\{(\Sigma_i,D_i,C_i)\}_{i=1}^{n}.
\]

Only legal finite derivations appear in \(\mathcal{D}\). The resolver selects:

\[
i^*=
\operatorname*{arg\,min}^{\text{seeded}}_{i}
\ C_i,
\]

using the lexicographic cost and stable seed rules in
[operational semantics](../algebra/semantics.md).

## Constraint realization

The selected trace emits typed constraint systems:

\[
\begin{aligned}
\mathcal C_p(D)& &&\text{pitch and trajectory},\\
\mathcal C_t(D)& &&\text{structural and performed time},\\
\mathcal C_o(D)& &&\text{ornament placement and realization},\\
\mathcal C_a(D)& &&\text{instrument actions and acoustic state}.
\end{aligned}
\]

Realization is:

\[
X=
\operatorname{Solve}_\Gamma
(\mathcal C_p,\mathcal C_t,\mathcal C_o,\mathcal C_a).
\]

Contradictory hard constraints fail. Ordered soft tiers are solved
lexicographically and cannot trade away stronger conventions for surface
variation.

## Regional reconstruction

\[
\Gamma=
\operatorname{Complete}\circ\operatorname{Normalize}
(P_0\triangleleft\Delta_1\triangleleft\cdots\triangleleft\Delta_k).
\]

Composition is exact and provenance-preserving. Missing rules, unresolved
multiple-parent conflicts, unsupported widening, and unreachable return paths
are errors before generation.

## Rendering

\[
W=\operatorname{Render}_{\mathcal I}(X)
\]

is the only stage where floating-point approximation is inherently required.
The pitch-to-frequency exponential, differential-equation integration, and
audio sampling publish numerical tolerances.

## Seed partition

\[
s_j=H(s,\text{module},\text{scope path},\text{choice identity}).
\]

This prevents adding an unrelated choice in one module from perturbing all
other choices. Seeds vary legal realizations; they do not create rules,
profiles, scales, or intonation facts.

## Calibration without grammar learning

Source analysis can produce:

- observed ranges and exact counts;
- reviewed constants and bounded tables;
- competing named variants;
- evidence supporting or rejecting a rule;
- acoustic parameter identification.

Those results enter profiles through explicit, reviewable patch operations.
There is no gradient descent, neural network, latent embedding, or automated
replacement of the operator grammar.

