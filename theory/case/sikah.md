# Sikah vertical route

This package is an executable ordered-station vertical slice, not a completed
Sikah repertoire implementation. It is kept outside the finished catalog
until the package completion contract in `theory/maqamat/scope.md` is met.

The structural source boundary is:

- [MaqamWorld Sikah](https://www.maqamworld.com/en/maqam/sikah.php);
- [MaqamWorld Jins Sikah](https://www.maqamworld.com/en/jins/sikah.php);
- [MaqamWorld Jins Rast](https://www.maqamworld.com/en/jins/rast.php).

The current route is declarative rather than name-dispatched:

```text
root Jins Sikah
→ upper Jins Rast station
→ Rast station at the ordered secondary center
→ root return
```

Its branch records carry source center, target center, tonic role, endpoint
role, and transition direction. The shared family builder uses those records
to construct descriptors, path rules, sayr obligations, pitch rules, and a
timed plan. The route is exercised directly by `sikah_case` and by the named
kernel CLI.

Still required before catalog completion:

- sourced phrase cells, motif identity, cadence behavior, and negative route
  constraints;
- contextual intonation and performer variation evidence;
- profile-matched casebook observations with independent annotation review;
- full pause, release, ornament, and phrase-rate validation;
- shared-boundary audio validation through the Faust renderer.
