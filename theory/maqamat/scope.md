# Maqāmāt implementation scope

The finished Arabic maqām module targets the named repertoire in the
[MaqamWorld maqām index](https://maqamworld.com/en/maqam.php). This is a
declared repertoire boundary, not a claim that one index exhausts Arabic,
Ottoman, Persian, Iraqi, Gulf, North-African, historical, or performer
practice.

The index presents eight families:

- ʿAjam;
- Bayati;
- Hijaz;
- Kurd;
- Nahawand;
- Nikriz;
- Rast;
- Sikah.

It also presents independent maqāmāt that do not belong to those families:

- Jiharkah;
- Lami;
- Saba;
- Saba Zamzam;
- Sikah Baladi.

The named entries in the declared index boundary are:

```text
ʿAjam, ʿAjam ʿUshayran, Athar Kurd, Awj ʿIraq, Bastanikar,
Bayati, Bayati Shuri, Dalanshin, Farahfaza, Hijaz, Hijazkar,
Hijazkar Kurd, Husayni, Huzam, ʿIraq, Jiharkah, Kirdan, Kurd,
Lami, Mahur, Muhayyar, Mustaʿar, Nahawand, Nahawand Murassaʿ,
Nairuz, Nawa Athar, Nikriz, Rahat al-Arwah, Rast, Saba,
Saba Zamzam, Sazkar, Shadd ʿAraban, Shahnaz, Shawq Afza, Sikah,
Sikah Baladi, Suzdalara, Suzidil, Suznak, ʿUshaq Masri, Yakah, Zanjaran.
```

The source family pages define the following package groups:

| Family | Entries |
| --- | --- |
| ʿAjam | ʿAjam, ʿAjam ʿUshayran, Shawq Afza |
| Bayati | Bayati, Bayati Shuri, Husayni, Muhayyar |
| Hijaz | Hijaz, Hijazkar, Shadd ʿAraban, Shahnaz, Suzidil, Zanjaran |
| Kurd | Kurd, Hijazkar Kurd |
| Nahawand | Nahawand, Farahfaza, Nahawand Murassaʿ, ʿUshaq Masri |
| Nikriz | Nikriz, Nawa Athar, Athar Kurd |
| Rast | Rast, Kirdan, Sazkar, Suznak, Nairuz, Yakah, Dalanshin, Suzdalara, Mahur |
| Sikah | Sikah, Huzam, Rahat al-Arwah, ʿIraq, Awj ʿIraq, Bastanikar, Mustaʿar |
| Independent | Jiharkah, Lami, Saba, Saba Zamzam, Sikah Baladi |

This list is a product scope, not executable data. Every entry must receive
its own profile package or an explicitly sourced branch package. A branch may
share universal laws and genuinely shared source claims, but it may not be
implemented as a name alias, scale-array substitution, or neutral fallback.

## Package completion contract

An entry is complete only when its package supplies:

- root and secondary jins descriptors with typed roles and register context;
- entry, establishment, development, climax, return, and closure cells;
- sourced positive and negative tonicization/modulation constraints;
- a dependency-checked sayr route set;
- phrase functions, cadence families, motif identity and allowed variations;
- contextual pitch authorities and trajectory conditions;
- timing, articulation, ornament, and release intents;
- provenance, source scope, competing interpretations, and confidence;
- executable law tests, complete-candidate generation, and the shared audio
  boundary test.

“Provisional” is a development state and research debt marker. It is never a
completion status. A package that lacks evidence is marked incomplete and
cannot be selected by a finished-repertoire catalog.

## Historical and tuning variants

DiArMaqAr provides a larger machine-readable archive of maqāmāt, ajnās,
suyūr, intiqālāt, tuning systems, and bibliographic provenance. Its data is a
separate documentary/profile source and must be imported only with its license
and source boundaries preserved. Historical tuning systems and regional
variants become separate profile identities; they do not overwrite the
canonical repertoire package.

## Current implementation state

Rast, Nahawand, Hijaz, Kurd, Ajam, and Nikriz are the first canonical package
implementations under this contract. Bayati and Sikah currently have
executable vertical routes but remain incomplete under their case-study gates.
The remaining entries are not silently executable. The catalog, package
coverage matrix, and tests must advance together until every declared entry
reaches the contract above.
