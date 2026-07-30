# Operational semantics

A grammar expression denotes a finite state relation:

\[
\llbracket g\rrbracket_\Gamma :
\Sigma\rightarrow
\mathcal{P}_{\!f}(\Sigma\times \operatorname{Trace}\times\operatorname{Cost}).
\]

The result is a finite set of legal outcomes. Illegal branches return no
outcome plus a diagnostic; they are not assigned a small probability.

## Primitive execution

For primitive operator \(o\):

\[
\frac{
\operatorname{pre}_\Gamma(o,\Sigma)
\qquad
\Sigma'=\operatorname{effect}_\Gamma(o,\Sigma)
}{
\langle o,\Sigma\rangle\Downarrow
\langle\Sigma',[o],\operatorname{cost}_\Gamma(o,\Sigma)\rangle
}.
\]

If the precondition is false, execution yields a typed violation containing
the rule identity and provenance that rejected it.

## Sequence

\[
\llbracket g_1;g_2\rrbracket(\Sigma)=
\bigcup_{(\Sigma_1,t_1,c_1)\in\llbracket g_1\rrbracket(\Sigma)}
\{(\Sigma_2,t_1\!\cdot t_2,c_1\!\oplus c_2):
(\Sigma_2,t_2,c_2)\in\llbracket g_2\rrbracket(\Sigma_1)\}.
\]

## Choice and seed

Costs are lexicographic vectors:

\[
C=(v_\text{hard},v_\text{idiom},v_\text{form},
v_\text{surface},u).
\]

Hard violations must equal zero. Profile-declared conventions determine the
remaining integer or rational penalties. The final component \(u\) is a stable
hash:

\[
u=H(\text{seed},\text{expression identity},\text{scope path},
\text{alternative identity}).
\]

The resolver first minimizes all declared cost tiers. The seed chooses only
among alternatives that remain eligible under the profile's choice-band rule.
It never converts a forbidden expression into a legal one.

## Evidence

Tonicization evidence is an exact vector, not a neural latent state:

\[
E=(e_\text{recurrence},e_\text{dwell},e_\text{emphasis},
e_\text{cell},e_\text{baggage},e_\text{cadence},e_\text{register}).
\]

Each operator contributes a profile-declared rational vector. A level
\(\ell\) is established when:

\[
A_{\Gamma,\ell}E\geq b_{\Gamma,\ell}
\]

and residence, path, phrase, and cadence guards are true. This allows different
regional practices to define different sufficient evidence without changing
the meaning of tonicization.

## Termination

Every repetition is bounded. Every recursive production must decrease a
well-founded budget:

\[
B=(\text{remaining sections},\text{phrases},\text{cells},\text{time})
\in\mathbb{N}^4.
\]

Consequently, evaluation and seeded choice terminate.

