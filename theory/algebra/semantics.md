# Operational semantics

A grammar expression denotes a finite state relation. Evaluation also carries a
lexical environment \(\Lambda=(\text{scope path},\text{bindings})\):

\[
\llbracket g\rrbracket_\Gamma :
(\Sigma,\Lambda)\rightarrow
\mathcal{P}_{\!f}
(\Sigma\times\Lambda\times\operatorname{Trace}\times\operatorname{Cost}
\times\operatorname{Decisions}).
\]

The result is a finite set of legal outcomes. Illegal branches return no
outcome plus a diagnostic; they are not assigned a small probability.

`Decisions` is an ordered trace of stable expression, scope-path, and
alternative identities. It is generation metadata, not musical state.

## Closed combinator semantics

`Id` returns its input with zero cost and no trace addition. `Fail(e)` returns no
outcome and the diagnostic \(e\).

`Guard(p,g)` evaluates \(g\) only when the typed predicate \(p\) holds over
\((\Sigma,\Lambda,\Gamma)\). A false guard returns a diagnostic, never a costly
outcome.

`Alt(a_1,\ldots,a_n)` evaluates every alternative to completion:

\[
\llbracket\operatorname{Alt}(a_1,\ldots,a_n)\rrbracket(s)=
\bigcup_i
\{\,r\oplus(c_i,d_i):r\in\llbracket a_i\rrbracket(s)\,\},
\]

where \(c_i\) is the declared branch cost and \(d_i\) its stable decision
identity. Seeded resolution occurs only over these complete legal outcomes.
Diagnostics from rejected alternatives remain available even when another
alternative succeeds.

Bounded repetition is a finite union:

\[
\llbracket\operatorname{Repeat}_{[m,n]}(g)\rrbracket=
\bigcup_{k=m}^{n}\llbracket g^k\rrbracket,\qquad
g^0=\operatorname{Id}.
\]

Each count is a distinct stable decision. A failed iteration removes only the
affected count and its continuations.

`Bind(x,g)` adds the identity \(x\) to the lexical binding set while evaluating
\(g\), then restores the prior set. Bindings are presence-typed grammar
capabilities; they do not mutate musical state. Guards may require a binding.

`Scope(s,E,g)` pushes \(s\) onto the lexical scope path, evaluates \(g\), and
then restores the parent path and bindings. For each musical component not in
the explicit export set \(E\), the output projection is restored from the
input. Trace and decision metadata are append-only evidence and are never
scope-local musical components.

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

For a complete derivation, the final stable component is hashed from its ordered
decision trace. This keeps draws dependent on expression and scope identities,
not container iteration order.

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
