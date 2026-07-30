# Sorts and identities

The unified system is a many-sorted algebra. An expression that joins
incompatible musical kinds is ill-formed before generation begins.

## Scalar sorts

- `Count`: non-negative integer.
- `Ratio`: reduced rational number.
- `Cents`: rational logarithmic pitch displacement.
- `Time`: positive rational score-time duration.
- `Rate`: positive rational multiplier.
- `Priority`: ordered integer tier; lower is stronger.
- `Seed`: unsigned integer used only by the choice resolver.

Floating-point values are a rendering format, not the authority for grammar or
profile composition.

## Identity sorts

Every identity is scoped and stable:

\[
\operatorname{Id}=(\text{namespace},\text{local name},\text{revision}).
\]

Distinct sorts exist for:

- tone role;
- tonal center;
- register;
- jins definition and jins placement;
- cell and motif family;
- cadence family;
- ornament family;
- phrase and section function;
- iqa and beat role;
- instrument and technique;
- evidence source;
- regional, school, repertoire, and performer profile.

An integer “scale degree” cannot substitute for a tone role. The same notated
degree may have different functions under different active ajnas.

## Product sorts

Core immutable values are:

\[
\begin{aligned}
\text{PlacedJins} &=
  \text{Jins}\times\text{Center}\times\text{Register},\\
\text{PitchRole} &=
  \text{ToneRole}\times\text{PlacedJins}\times\text{Function},\\
\text{Boundary} &=
  \text{Scope}\times\text{Closure}\times\text{Target},\\
\text{Context} &=
  \text{Profile}\times\text{Form}\times\text{Instrument}.
\end{aligned}
\]

## State

\[
\Sigma=(h,z,m,q,d,\tau,a,\Gamma)
\]

- \(h\): formal cursor and scope stack;
- \(z\): active tonal interpretations and tonicization evidence;
- \(m\): motif memory;
- \(q\): sayr obligations;
- \(d\): discourse and cadence state;
- \(\tau\): metric or free-time state;
- \(a\): instrument/acoustic state;
- \(\Gamma\): immutable selected profile and rule environment.

State changes are produced only by typed operators. Direct field mutation is
outside the language.

