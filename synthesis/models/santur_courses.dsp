import("stdfaust.lib");

pm = library("physmodels.lib");

declare name "music_seed_santur_courses";
declare version "0.1";
declare description "Three-course struck resonator; parameters remain provisional";

fundamental = hslider("fundamental_hz", 220, 20, 2000, 0.01);
intensity = hslider("intensity", 0.8, 0, 1, 0.001);
strike = button("strike");
excitation = intensity * pm.impulseExcitation(strike);

course1 = excitation : pm.modalModel(
    6,
    (fundamental * 0.9982, fundamental * 1.9964,
     fundamental * 3.0, fundamental * 4.01,
     fundamental * 5.03, fundamental * 6.06),
    (1.0, 0.46, 0.24, 0.14, 0.09, 0.05),
    (0.8, 0.44, 0.24, 0.13, 0.08, 0.04));
course2 = excitation : pm.modalModel(
    6,
    (fundamental, fundamental * 2.0,
     fundamental * 3.002, fundamental * 4.008,
     fundamental * 5.02, fundamental * 6.04),
    (1.0, 0.46, 0.24, 0.14, 0.09, 0.05),
    (0.8, 0.44, 0.24, 0.13, 0.08, 0.04));
course3 = excitation : pm.modalModel(
    6,
    (fundamental * 1.0021, fundamental * 2.0042,
     fundamental * 3.006, fundamental * 4.016,
     fundamental * 5.025, fundamental * 6.05),
    (1.0, 0.46, 0.24, 0.14, 0.09, 0.05),
    (0.8, 0.44, 0.24, 0.13, 0.08, 0.04));

// The bridge filter is the shared coupling/radiation boundary. The final
// instrument model must replace this reduced modal coupling with measured
// bridge and soundboard state.
process = (course1, course2, course3) :> _ : pm.bridgeFilter(0.62, 0.38);
