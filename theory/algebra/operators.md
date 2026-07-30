# Primitive operators

Operators express musical actions. A regional profile controls their legal
domains, preconditions, costs, and realizations; it does not redefine their
semantics.

## Tonal operators

\[
\begin{array}{ll}
\operatorname{Anchor}(c) & \text{declare or restore a center}\\
\operatorname{Enter}(j) & \text{enter a placed jins}\\
\operatorname{Depart}(r) & \text{weaken the current center through role }r\\
\operatorname{Approach}(r,k) & \text{approach role }r\text{ by family }k\\
\operatorname{Emphasize}(r,n) & \text{add }n\text{ units of emphasis evidence}\\
\operatorname{Dwell}(r,t) & \text{occupy role }r\text{ for structural time }t\\
\operatorname{Color}(j) & \text{use a secondary color without new center}\\
\operatorname{Tonicize}(j,\ell) & \text{establish jins }j\text{ to level }\ell\\
\operatorname{Modulate}(p,\ell) & \text{follow a licensed path }p\\
\operatorname{Return}(p) & \text{restore a prior center through path }p.
\end{array}
\]

`Tonicize` is a checked summary operator. It succeeds only when the preceding
trace contains the required evidence. It cannot manufacture that evidence.

## Vocabulary operators

\[
\begin{array}{ll}
\operatorname{Emit}(c) & \text{realize a cell}\\
\operatorname{Expose}(m) & \text{first identity-bearing motif occurrence}\\
\operatorname{Recall}(m) & \text{retrieve an established motif}\\
\operatorname{Transform}(m,x) & \text{apply a licensed transformation}\\
\operatorname{Connect}(a,b,k) & \text{use connector family }k.
\end{array}
\]

Transformations include sequence, register transfer, temporal dilation,
extension, truncation, neighbor elaboration, and changed endings. A
transformation is unavailable unless the active profile defines its domain.

## Phrase and form operators

- `Begin(scope,function)` and `End(scope,boundary)`;
- `Prepare(target)`;
- `Suspend(target)`;
- `Answer(reference,closureDelta)`;
- `Cadence(family,target,strength)`;
- `Fulfil(obligation)`;
- `ActivateIqa(iqa)` and `DeactivateIqa()`.

Iqa operators alter rhythmic context, never maqam identity.

## Performance operators

- `Place(event,role,direction)` records structural melodic intent;
- `Intone(role,context)` solves a pitch field and trajectory;
- `Time(event,context)` realizes structural time;
- `Ornament(host,family)` elaborates a host without replacing it;
- `Articulate(host,intent)` declares instrument-neutral execution intent;
- `Release(host,intent)` declares continuation or release intent.

Instrument action and excitation are not language operators. A synthesis
adapter maps supported performance intents after derivation.

## Combinators

The grammar itself uses a small closed set:

\[
\begin{aligned}
\operatorname{Id} &: G\\
\operatorname{Fail}(e) &: G\\
\operatorname{Seq}(g_1,g_2) &: G\\
\operatorname{Alt}(g_1,\ldots,g_n) &: G\\
\operatorname{Guard}(p,g) &: G\\
\operatorname{Repeat}_{[m,n]}(g) &: G\\
\operatorname{Scope}(s,g) &: G\\
\operatorname{Bind}(x,g) &: G.
\end{aligned}
\]

There is no unrestricted note-choice operator.
