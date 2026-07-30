# Musical state

The generator operates on musical state, not on a current scale and note.
At decision time \(t\), the state is

\[
S_t=(H_t,Z_t,M_t,P_t,Q_t,T_t,A_t,D_t)
\]

where:

- \(H_t\): hierarchical position: section, phrase, motif, cell, event;
- \(Z_t\): active tonal hypothesis;
- \(M_t\): motif memory and transformation history;
- \(P_t\): phrase-function and cadence state;
- \(Q_t\): sayr obligations, permissions, and completed landmarks;
- \(T_t\): timing, breath, and optional iqa state;
- \(A_t\): acoustic state of the instrument and sounding courses;
- \(D_t\): discourse state: question, response, contrast, recall, closure.

## Tonal hypothesis

\(Z_t\) is a distribution, not a categorical switch:

\[
Z_t=(J_t,\tau_t,\ell_t,m_t,c_t,r_t)
\]

- \(J_t\): candidate jins interpretation;
- \(\tau_t\): candidate local tonic;
- \(\ell_t\): tonicization level;
- \(m_t\): parent maqam hypothesis;
- \(c_t\): confidence;
- \(r_t\): expected minimum residence and cadence evidence.

The tonicization levels are:

1. **color** — a brief sonority or gesture without a stable new center;
2. **internal** — another jins is established while the maqam identity remains;
3. **maqam** — a new principal center and maqam identity are established.

A single pitch cannot prove a modulation. Evidence accumulates from recurrence,
dwell, emphasis, register, compatible baggage, idiomatic cells, and cadence.

## State invariants

These are hard constraints unless a profile explicitly supplies an attested
exception:

1. Every event belongs to a cell, every cell to a motif occurrence or connective
   gesture, and every phrase to a section.
2. A claimed internal or maqam-level modulation must have a legal graph path and
   sufficient tonicization evidence.
3. Pitch realization is requested only after a structural pitch role is chosen.
4. Ornament cannot create a structural tonicization that its carrier tone lacks.
5. Phrase-final closure cannot exceed the closure licensed by its section state.
6. The acoustic renderer may alter audibility, not the intended musical role.
7. Region and repertoire profiles may narrow permissions but may not silently
   borrow rules from another profile.

## History

Generation is non-Markovian at the note level. The minimum retained history is:

- structural pitch roles and their realized trajectories;
- phrase boundaries and cadence strengths;
- motif identities and transformations;
- tonicization posterior over time;
- completed and pending sayr obligations;
- registers visited, climax candidates, and return paths;
- sounding-course energy and masking estimates.

The history may be compressed into sufficient statistics only after tests show
that the compression preserves phrase and modulation judgments.

## Constraint layers

The model distinguishes:

- **hard legality**: impossible or outside the selected profile;
- **strong convention**: rare violations require corpus evidence;
- **soft preference**: weighted alternatives;
- **performer prior**: individual tendency;
- **stochastic surface choice**: variation that does not alter grammar.

Random seeds select among licensed alternatives. They never legalize an
otherwise forbidden transition.

