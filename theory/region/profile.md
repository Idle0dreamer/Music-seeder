# Tradition and repertoire profiles

The uniform component is the typed operator algebra and constraint machinery.
Musical permissions, costs, formulas, and constants live in profiles.

## Profile

\[
\mathcal{P}=(g,v,s,c,i,o,f,a,r,e)
\]

- \(g\): jins and modulation graph data;
- \(v\): cell and motif vocabulary;
- \(s\): sayr policies;
- \(c\): phrase and cadence grammar weights;
- \(i\): contextual intonation model;
- \(o\): ornament and articulation inventory;
- \(f\): form and timing conventions;
- \(a\): instrument realization practices;
- \(r\): repertoire, era, and performer priors;
- \(e\): evidence, uncertainty, and coverage.

## No generic regional offset

Egyptian, Levantine, Iraqi, Gulf, Turkish, North African, and other related
practices cannot be represented by a cents offset. A profile can change the
graph, vocabulary, cadence behavior, intonation, phrase timing, ornament,
formal expectations, and instrument technique.

## Reconstruction

Profiles may inherit shared claims only when evidence supports the sharing:

\[
\text{shared semantics}\rightarrow\text{tradition}\rightarrow
\text{region}\rightarrow\text{school}\rightarrow
\text{repertoire}\rightarrow\text{performer}.
\]

Each step is a declarative patch. Compatible refinements compose; conflicting
parent rules require an explicit child resolution. Claims that conflict across
sources remain named variants. The system does not average them into a
fictitious universal style.

The complete patch algebra is defined in
[regional reconstruction](../algebra/reconstruct.md).

## Coverage

Each profile publishes a coverage matrix for:

- maqamat and ajnas;
- transitions, including known forbidden or unsupported paths;
- motif and cadence vocabulary;
- intonation contexts;
- forms and iqa contexts;
- ornaments and instruments;
- repertoire periods and performer representation.

Unsupported cells in this matrix are errors if selected for generation, not
opportunities for unconstrained fallback.
