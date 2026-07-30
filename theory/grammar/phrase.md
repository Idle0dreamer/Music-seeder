# Phrase and cadence grammar

Phrase structure is hierarchical and lexicalized. A practical schema is:

\[
\begin{aligned}
\text{Performance} &\Rightarrow \text{Section}^{+}\\
\text{Section} &\Rightarrow \text{PhraseGroup}^{+}\\
\text{PhraseGroup} &\Rightarrow
  \text{Phrase}\;[\text{Response}]\;[\text{Expansion}]\\
\text{Phrase} &\Rightarrow
  \text{Preparation}\;\text{Core}\;[\text{Extension}]\;\text{Boundary}.
\end{aligned}
\]

Square brackets indicate licensed optional constituents, not a fixed universal
order. Productions are conditioned on profile, maqam, section function,
tonal state, and discourse state.

## Phrase functions

A phrase carries one principal function and optional secondary functions:

- expose;
- prepare;
- confirm;
- vary;
- expand;
- sequence;
- suspend;
- contrast;
- re-expose;
- return;
- conclude.

Function constrains motif choice, direction, register, emphasis, tonal evidence,
timing, and admissible boundary.

## Phrase relations

Relations are evaluated over complete phrases:

- **question–response**: response recalls material and increases or redirects
  closure;
- **repetition**: preserves identity-bearing anchors;
- **variation**: preserves motif identity while changing licensed dimensions;
- **sequence**: moves a recognized cell or motif through licensed tonal roles;
- **expansion**: elaborates a prior span without losing its anchors;
- **contrast**: weakens similarity while maintaining discourse continuity;
- **recall**: restores earlier tonal or motivic material after intervening space.

Question and answer are not assigned from final-note height. Their status is
inferred from similarity, boundary strength, tonal direction, timing, and the
next phrase’s behavior.

## Cadence object

A cadence is

\[
K=(s,\tau,p,a,f,e,d,\sigma)
\]

- \(s\): scope — cell, phrase, section, or performance;
- \(\tau\): target tonal center;
- \(p\): preparation conditions;
- \(a\): approach-family identity;
- \(f\): closure strength;
- \(e\): tonicization evidence contributed;
- \(d\): duration, repetition, and silence behavior;
- \(\sigma\): profile-specific surface realizations.

Closure strength is continuous internally but labeled for annotation:

- none;
- weak continuation;
- local;
- sectional;
- final.

A final cadence requires a licensed return path, root-jins re-establishment,
adequate motivic recall or conclusion, and enough temporal space for the
resolution to be heard.

## Weighted attribute grammar

For derivation \(Y\), latent tonal path \(Z\), and context \(C\):

\[
P(Y,Z\mid C) =
\frac{
\mathbf{1}_{\mathrm{legal}}(Y,Z,C)
\;\exp\left(\sum_k \lambda_k f_k(Y,Z,C)\right)}
{\mathcal{Z}(C)}.
\]

Features include:

- production and lexical likelihood;
- motif recognition and transformation cost;
- cadence fit;
- tonal-evidence consistency;
- sayr progress;
- phrase-length and boundary fit;
- register and climax preparation;
- performer and repertoire priors;
- repetition, novelty, and delayed recall;
- predicted perceptual clarity.

Generation uses constrained beam or particle search over phrase-level
derivations. Greedy note-by-note sampling is prohibited.
