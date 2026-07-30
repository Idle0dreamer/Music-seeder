# Lexicographic tendency sequence

Tendency tiers are ordered strongest to weakest. A lower tier may choose among
the minimizers left by stronger tiers; it may not trade away any stronger
optimum.

## Prediction preservation

For tier \(k\), collect its affine predictions:

\[
y_k=A_kx.
\]

The tier objective is a positive weighted squared distance from \(y_k\) to its
target vector. The attainable prediction set is convex, and positive weighted
squared distance is strictly convex in \(y_k\). Therefore every structural
solution minimizing the tier has the same prediction vector \(y_k^*\), even
when its pitch solution \(x\) is not unique.

After solving tier \(k\), add one exact equality per target:

\[
a_{ki}^{\top}x=y_{ki}^*.
\]

These equalities describe exactly the stronger tier's remaining minimizer set.
Tier \(k+1\) is then solved by the existing one-tier KKT operator over the
narrowed hard region.

## Identity and trace

Every tier and target has a complete stable identity. Target identities are
unique across the whole sequence. Synthetic preservation equalities use a
reserved namespace derived from the target identity, never an array position.

The result records, for every tier:

- tier identity;
- exact target predictions;
- stable identities of the active hard inequalities.

The final structural solution is returned separately.

## Resource boundary

A declared tier-count limit rejects oversized sequences before solving. Each
tier independently inherits the active-set and hard-solver proof budgets.
Failure never drops a stronger preservation equality or substitutes an
approximate objective.

## Downstream breadth

Lexicographic tiers unlock explicit precedence among structural convention,
regional preference, repertoire tendency, performer tendency, and bounded
surface variation without averaging their authority. The next consumer is the
pitch-field assembly layer, which assigns sourced terms to named tiers rather
than one undifferentiated cost.

## Laws

- one tier agrees with the one-tier KKT operator;
- a conflicting weaker target cannot change a stronger prediction;
- a weaker tier may resolve a stronger tier's null space;
- every earlier prediction remains exact in the final solution;
- target storage order does not change a tier result;
- tier order is semantic and is not normalized away;
- incomplete or repeated identities and tier-count exhaustion fail explicitly.
