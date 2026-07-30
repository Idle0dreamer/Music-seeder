# Calculated pitch fields

Pitch is calculated from constraints attached to roles and contexts. The input
is not a scale array.

## Variables

For active structural pitch roles \(r_1,\ldots,r_n\), let:

\[
x=(x_1,\ldots,x_n)^\top
\]

be rational cents relative to a scoped reference.

## Constraints

Hard constraints are rational affine equalities and inequalities:

\[
H x=h,\qquad Lx\leq l.
\]

Examples include anchoring, pitch ordering, course range, and an interval
identity declared exact within a selected tuning theory.

Contextual tendencies are ordered soft tiers:

\[
\min_{\text{lex}}
\left(
\sum_i w_{1i}(a_{1i}^{\top}x-b_{1i})^2,\ldots,
\sum_i w_{ki}(a_{ki}^{\top}x-b_{ki})^2
\right)
\]

subject to the hard constraints. All coefficients and weights are rational.
A lower tier cannot trade away a stronger tier.

## Conditional terms

The target formula for role \(r\) is assembled from explicit profile terms:

\[
x_r=
\mu(r,J,M)
\delta_d(r)
\delta_n(r,n_-,n_+)
\delta_k(r,\text{cell},\text{cadence})
\delta_g(r)
\delta_p(r)
\delta_\nu(r)
\epsilon.
\]

Each \(\delta\) is a declared formula or reviewed measurement table with
provenance. \(\epsilon\) is an optional bounded, seeded performance deviation,
not a learned residual.

## Trajectory

After solving structural centers, a continuous trajectory uses basis functions:

\[
c_r(t)=x_r+
\sum_j \alpha_j B_j(t),\qquad t\in[0,1].
\]

Basis families describe approach, delayed arrival, attraction, release, and
instrument-specific connection. Boundary conditions ensure the trajectory
reaches or intentionally delays the structural target.

## Exactness boundary

Grammar, profile composition, and rational constraints use exact arithmetic.
Conversion to frequency:

\[
f=f_0\,2^{c/1200}
\]

and waveform integration necessarily use controlled floating-point
approximations. Tests declare error bounds at that boundary.

