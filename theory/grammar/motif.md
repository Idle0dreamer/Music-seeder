# Cells, motifs, and melodic vocabulary

The smallest generative musical vocabulary is not a scale degree. It is a
profile-specific inventory of melodic cells and motif families.

## Cell

A cell is

\[
C=(\rho,\alpha,\kappa,\delta,\eta,\omega)
\]

- \(\rho\): relative structural pitch roles;
- \(\alpha\): anchor and emphasis pattern;
- \(\kappa\): contour and directional constraints;
- \(\delta\): elastic duration relationships;
- \(\eta\): admissible ornaments and articulations;
- \(\omega\): entry and exit conditions.

Pitch roles are relational, such as:

- jins tonic;
- ghammaz or modulation pivot;
- approach from below;
- upper-neighbor suspension;
- emphasized baggage tone;
- cadential penultimate;
- connective passing tone.

A cell may leave some surface pitches unspecified. Those pitches are sampled
only after the cell is placed in a tonal, directional, and phrase context.

## Motif family

A motif family is a recognizable ordered composition of cells:

\[
M=(C_1,\ldots,C_n;\ a,\chi,\Pi,\mathcal{T})
\]

- \(a\): identity-bearing anchors;
- \(\chi\): characteristic contour and timing relations;
- \(\Pi\): placement preconditions;
- \(\mathcal{T}\): attested transformation set.

Transformations are not assumed to form a universal musical group. A profile
must license each operation:

- diatonic or jins-relative sequence;
- register transfer;
- rhythmic dilation or compression;
- prefix or suffix extension;
- truncation;
- neighbor elaboration;
- ornament substitution;
- changed ending for cadence or continuation.

Inversion and retrograde are excluded by default. They enter a profile only
with repertoire evidence.

## Recognition constraint

For a transformed occurrence \(M'\), identity is retained when

\[
d_w(M,M') =
w_a d_a + w_c d_c + w_r d_r + w_t d_t + w_e d_e
\leq \theta_{M,\text{profile}} .
\]

The distances compare anchors, contour, pitch-role relations, timing ratios,
and entry/exit function. Weights and threshold are motif- and profile-specific.
This prevents “variation” from becoming unrelated random material.

## Motif memory

For each motif family the state stores:

- first exposure;
- most recent occurrence;
- cumulative transformations;
- recognition confidence;
- unresolved continuation;
- phrase and section functions already served.

The grammar rewards recall after sufficient separation, penalizes mechanical
immediate repetition, and limits repeated transformation in one direction.
The exact costs are declared in the profile and may be calibrated from reviewed
profile-matched phrase counts.

## Vocabulary construction

Cells and motifs are not hand-entered as interval arrays. They are obtained
from:

1. expert annotation of phrase segments and pitch roles;
2. alignment of recurring melodic material;
3. clustering conditioned on maqam, jins, function, direction, and performer;
4. expert acceptance, splitting, or rejection of candidate families;
5. explicit source and confidence records.

Software may propose recurring candidates for expert review, but only explicitly
accepted families enter a profile. It must not invent tradition-wide claims
from one recording.
