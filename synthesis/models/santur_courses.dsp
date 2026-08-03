import("stdfaust.lib");

pm = library("physmodels.lib");

declare name "music_seed_santur_courses";
declare version "0.1";
declare description "Three-course hammered-dulcimer resonator with live strike control";

fundamental = hslider("fundamental_hz", 220, 20, 2000, 0.01);
intensity = hslider("intensity", 0.8, 0, 1, 0.001);
articulation = hslider("articulation", 0, 0, 2, 1);
releaseSeconds = hslider("release_seconds", 0.5, 0.001, 10.0, 0.001);
strike = button("strike");
strikePosition = hslider("strike_position", 0.42, 0.05, 0.95, 0.001);
strikeSharpness = hslider("strike_sharpness", 0.72, 0.05, 1.5, 0.001);
sustainSeconds = hslider("sustain_seconds", 3.2, 0.25, 12.0, 0.01);
// Neutral, connected, and detached are distinct attack-force mappings at
// this boundary. Their named-instrument calibration remains future work.
attackGain = 1.0 - 0.10 * articulation;
// A dulcimer hammer is a short filtered-noise contact, not a clean unit
// impulse. Keep the excitation live so every incoming strike starts a new
// string state while the modal state continues between render blocks.
excitation = intensity * attackGain * pm.strike(
    strikePosition, strikeSharpness, 1.0, strike);
decayScale = (sustainSeconds + releaseSeconds) / 3.2;

course1 = excitation : pm.modalModel(
    10,
    (fundamental * 0.9982, fundamental * 1.9964,
     fundamental * 3.0, fundamental * 4.01,
     fundamental * 5.03, fundamental * 6.06,
     fundamental * 7.11, fundamental * 8.18,
     fundamental * 9.27, fundamental * 10.38),
    (1.0, 0.46, 0.24, 0.14, 0.09, 0.05, 0.032, 0.021, 0.014, 0.009),
    (0.8 * decayScale, 0.44 * decayScale, 0.24 * decayScale,
     0.13 * decayScale, 0.08 * decayScale, 0.04 * decayScale,
     0.026 * decayScale, 0.018 * decayScale, 0.012 * decayScale,
     0.008 * decayScale));
course2 = excitation : pm.modalModel(
    10,
    (fundamental, fundamental * 2.0,
     fundamental * 3.002, fundamental * 4.008,
     fundamental * 5.02, fundamental * 6.04,
     fundamental * 7.08, fundamental * 8.14,
     fundamental * 9.22, fundamental * 10.32),
    (1.0, 0.46, 0.24, 0.14, 0.09, 0.05, 0.032, 0.021, 0.014, 0.009),
    (0.8 * decayScale, 0.44 * decayScale, 0.24 * decayScale,
     0.13 * decayScale, 0.08 * decayScale, 0.04 * decayScale,
     0.026 * decayScale, 0.018 * decayScale, 0.012 * decayScale,
     0.008 * decayScale));
course3 = excitation : pm.modalModel(
    10,
    (fundamental * 1.0021, fundamental * 2.0042,
     fundamental * 3.006, fundamental * 4.016,
     fundamental * 5.025, fundamental * 6.05,
     fundamental * 7.13, fundamental * 8.2,
     fundamental * 9.29, fundamental * 10.4),
    (1.0, 0.46, 0.24, 0.14, 0.09, 0.05, 0.032, 0.021, 0.014, 0.009),
    (0.8 * decayScale, 0.44 * decayScale, 0.24 * decayScale,
     0.13 * decayScale, 0.08 * decayScale, 0.04 * decayScale,
     0.026 * decayScale, 0.018 * decayScale, 0.012 * decayScale,
     0.008 * decayScale));

// The bridge filter is the shared coupling/radiation boundary. The final
// instrument model must replace this reduced modal coupling with measured
// bridge and soundboard state.
process = (course1, course2, course3) :> _ : pm.bridgeFilter(0.62, 0.38);
