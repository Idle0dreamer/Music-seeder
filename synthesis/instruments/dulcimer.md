# Hammered dulcimer acoustic model

This is one prospective synthesis adapter. It is not part of the maqam theory,
grammar, profile language, or kernel completion claim.

The renderer models a struck, coupled string–bridge–soundboard system. A bank of
long generic resonators is rejected because it can let one band mask later
notes.

## Instrument profile

Required measurements or identified parameters include:

- course layout, lengths, gauges, materials, tensions, and paired strings;
- bridge locations, impedances, and coupling;
- soundboard geometry, modal frequencies, damping, and radiation;
- hammer mass, hardness, contact law, strike velocity, angle, and position;
- muting, hand damping, restrike technique, and stereo microphone geometry.

## Stiff string

For string displacement \(y(x,t)\):

\[
\mu y_{tt}+2\mu\sigma_0y_t
-Ty_{xx}+EIy_{xxxx}
=F_h(t)\delta(x-x_h)+F_b(t)\delta(x-x_b).
\]

- \(\mu\): linear density;
- \(T\): tension;
- \(EI\): bending stiffness;
- \(\sigma_0\): loss;
- \(x_h,x_b\): strike and bridge positions.

Frequency-dependent loss is added in the modal domain. Inharmonic modal
frequencies follow from the stiff-string eigenproblem, not exact integer
partials.

## Hammer contact

\[
F_h(t)=K[\max(0,z_h(t)-y(x_h,t))]^p
\eta_h\,\frac{d}{dt}[\max(0,z_h-y)] ,
\]

while hammer motion obeys

\[
m_h\ddot z_h=-F_h(t).
\]

Contact ends when compression returns to zero. \(K,p,\eta_h\) depend on hammer
material and effective contact.

## Coupling

Course and bridge forces are related by a frequency-dependent mobility matrix:

\[
\mathbf v_b(\omega)=\mathbf Y_b(\omega)\mathbf F_b(\omega).
\]

Soundboard modal coordinates obey:

\[
\ddot q_m+2\zeta_m\omega_m\dot q_m+\omega_m^2q_m
=\phi_m(x_b)F_b(t).
\]

The radiation signal is a spatially weighted sum of modal velocities and direct
string/bridge radiation. Closely tuned strings in one course retain phase and
detuning, producing time-varying beating.

## Restrike and damping

The state of every sounding course persists between events. A new strike begins
from current displacement and velocity, not from silence. Hand damping and
muting are time-varying boundary losses. Energy is never normalized per note
after mixing.

## Crispness and masking

“Crisp” is operationalized against reference recordings:

\[
\mathrm{ELR}_{80} =
10\log_{10}
\frac{\int_0^{80\text{ms}}x^2(t)\,dt}
     {\int_{80\text{ms}}^{2\text{s}}x^2(t)\,dt}.
\]

Additional measurements:

- attack time;
- spectral-centroid trajectory;
- bandwise \(T_{20}\) and \(T_{60}\);
- early-to-late energy by pitch and strike strength;
- partial and soundboard-band persistence;
- masking of a new note by all earlier notes;
- dynamic range under repeated attacks.

The model is rejected if a narrow soundboard band remains perceptually dominant,
if decay is nearly identical across pitches, or if later attacks lose their
expected clarity.

## Calibration order

1. isolated courses across pitch and dynamics;
2. repeated strikes on one course;
3. adjacent and widely separated course pairs;
4. fast melodic cells;
5. ornament patterns;
6. complete phrases.

No full performance render is accepted before isolated-note decay and overlap
tests match profile-specific reference distributions within declared tolerance.
