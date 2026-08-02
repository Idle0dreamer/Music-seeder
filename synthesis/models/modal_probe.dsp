import("stdfaust.lib");

pm = library("physmodels.lib");

declare name "music_seed_modal_probe";
declare version "0.1";
declare description "Generic modal resonator probe; not a named instrument model";

fundamental = hslider("fundamental_hz", 220, 20, 2000, 0.01);
strike = button("strike");
excitation = pm.impulseExcitation(strike);

// This is a build-time probe for the physical-modeling boundary. The
// inharmonic ratios are resonator parameters, not a maqam tuning authority.
process = excitation : pm.modalModel(
    3,
    (fundamental, fundamental * 2.01, fundamental * 3.02),
    (1.4, 0.9, 0.55),
    (0.9, 0.45, 0.2));
