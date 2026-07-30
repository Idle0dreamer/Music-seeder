# Modulation and tonicization

Modulation is modeled as constrained inference over a labeled graph, with
context-dependent edges and duration.

## Graph

\[
\mathcal{G}_{p,m}=(N,E)
\]

is defined for profile \(p\) and parent maqam \(m\).

A node is:

\[
n=(J,\tau,R,B)
\]

where \(J\) is a jins family, \(\tau\) a local tonic role, \(R\) a register
condition, and \(B\) required jins baggage.

An edge is:

\[
e=(n_i,n_j,\ell,\pi,\gamma,\rho,\delta,w)
\]

- \(\ell\): maximum tonicization level;
- \(\pi\): pivot or approach conditions;
- \(\gamma\): admissible gesture and cadence families;
- \(\rho\): phrase and sayr-stage conditions;
- \(\delta\): minimum/typical residence;
- \(w\): corpus-estimated weight.

The graph may contain path-conditioned edges. An edge can be legal only if a
specific intermediate state was established earlier. Therefore the actual
model is a graph with finite history, or equivalently a constrained automaton,
not an interchangeable collection of tetrads.

## Tonicization inference

Let \(z_t\) be the active tonal hypothesis and \(x_t\) the observed or generated
musical evidence. A semi-Markov update is:

\[
P(z_t,d_t\mid x_{1:t},C)\propto
\;p(x_t\mid z_t,C)
\times
\sum_{z_{t-1},d_{t-1}}
p(z_t,d_t\mid z_{t-1},d_{t-1},C)
P(z_{t-1},d_{t-1}\mid x_{1:t-1},C).
\]

The emission model uses:

- tonic recurrence and dwell;
- emphasis and register;
- idiomatic cell and motif likelihood;
- baggage-tone compatibility;
- approach and cadence likelihood;
- direction and phrase function;
- neighboring tonal hypotheses.

The duration variable \(d_t\) distinguishes fleeting color from established
tonicization.

## Generation rule

The grammar first proposes a modulation intent, path, target strength, and
cadence plan. It then generates sufficient supporting evidence. It does not
choose a foreign pitch and relabel it afterward.

The modulation is accepted only when:

\[
\Pr(\ell\geq \ell_{\mathrm{planned}}\mid x_{1:t})\geq\theta_{\ell,p,m}
\]

and all graph, residence, phrase, and return conditions are satisfied.

## Bayati is a test case, not a universal graph

Published pedagogical analysis recognizes internal pathways among Bayati,
Rast, Nahawand, Hijaz, and Ajam at particular roles and registers, while also
showing that plausible-looking direct moves are not automatically idiomatic.
The exact graph must be encoded as sourced profile data, with negative edges
and required intermediate paths, rather than inferred from shared pitch
collections alone.
