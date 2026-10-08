---
name: quantum-script--applicationversion
description: >-
  How to use the Quantum Script ApplicationVersion extension
  (quantum-script--applicationversion), the version comparison extension
  loaded with Script.requireExtension("ApplicationVersion"):
  ApplicationVersion.compare(versionA, versionB, type) returning -1 (A
  older) / 0 (equal) / 1 (A newer) / undefined (unparsable version),
  numeric per-part comparison of major.minor.patch.build ("1.10" > "1.9",
  "1.2" == "1.2.0.0", suffixes like "-rc1" ignored, "v1.2" invalid), the
  VersionCompare bit mask constants (Major 1, Minor 2, Patch 4, Build 8,
  MajorMinor 3, MajorMinorPatch 7 = default, All 15), the type pitfalls
  (null / 0 compare nothing and always return 0), recipes (minimum version,
  ranges, same major, sorting, newest); the C++ side
  (registerInternalExtension, XYO::System::ApplicationVersion::compare).
  Use when writing or reviewing Quantum Script or fabricare .js code that
  compares or checks versions, C++ code that includes
  <XYO/QuantumScript.Extension/ApplicationVersion.hpp>, a fabricare.json
  depending on "quantum-script--applicationversion", or when working inside
  the quantum-script--applicationversion repository.
---

# quantum-script--applicationversion

Version comparison extension of Quantum Script (see the `quantum-script`
skill for the language and its differences from JavaScript, and the
`xyo-system` skill for the underlying `XYO::System::ApplicationVersion`;
their rules apply). Purpose: **compare version strings numerically, part
by part**, so scripts can check "installed >= required", "is an update
available", sort releases — things string comparison gets wrong
(`"1.10" < "1.9"` as text).

Full documentation: `docs/` in the quantum-script--applicationversion
repository
(`X:\Storage\XYO\Gitea\CPP\quantum-script--applicationversion\docs` on this
machine): README, getting-started, **version-format** (exact parsing
rules), **script-api** (`compare`, the `type` mask, recipes), reference.
The whole extension is `source/XYO/QuantumScript.Extension/ApplicationVersion/Library.cpp`
(~80 lines); the logic is `xyo-system/source/XYO/System/ApplicationVersion.cpp`.

## Script API

```javascript
Script.requireExtension("ApplicationVersion");   // defines globals VersionCompare and ApplicationVersion

ApplicationVersion.compare(a, b);                        // -1 a older, 0 equal, 1 a newer, undefined = invalid
ApplicationVersion.compare(a, b, VersionCompare.All);    // include the build number

VersionCompare.Major            // 1
VersionCompare.Minor            // 2
VersionCompare.Patch            // 4
VersionCompare.Build            // 8
VersionCompare.MajorMinor       // 3
VersionCompare.MajorMinorPatch  // 7  default (build ignored)
VersionCompare.All              // 15
```

```javascript
ApplicationVersion.compare("1.2.3", "1.2.4");                       // -1
ApplicationVersion.compare("1.10", "1.9");                          // 1   numeric, not text
ApplicationVersion.compare("1.2", "1.2.0.0");                       // 0   missing parts are 0
ApplicationVersion.compare("1.2.3.4", "1.2.3.9");                   // 0   build ignored by default
ApplicationVersion.compare("1.2.3.4", "1.2.3.9", VersionCompare.All);   // -1
ApplicationVersion.compare("2.0", "1.9", VersionCompare.Minor);     // -1  only minor: 0 < 9
ApplicationVersion.compare("v1.2", "1.2");                          // undefined
```

## Hard rules

1. **Result is the sign of `a - b`.** `compare(installed, required) >= 0`
   means "installed is at least required". Argument order matters.
2. **`undefined` means a version did not parse**, and it is falsy like `0`;
   `undefined < 0` and `undefined >= 0` are both `false`, `undefined == 0`
   is `false`. Test `Script.isUndefined(r)` when invalid input must be
   reported. Never use `if (compare(a, b))` to mean "different".
3. **Parsing is `sscanf("%d.%d.%d.%d")`**: 1 to 4 integers, missing parts
   `0`, leading zeros / whitespace / `+` ignored, reading stops at the first
   unexpected character. Valid as soon as the major number is read.
   - `"1.2.3-rc1"` == `"1.2.3"` (suffix ignored, no pre-release ordering).
   - `"1..3"` is `1.0.0.0` (stops at the second dot).
   - `"v1.2"`, `""`, a missing argument → `undefined`. Strip prefixes first.
   - Parts beyond the fourth are ignored; keep parts within 32-bit int.
4. **Pass strings, not numbers.** The number `1.10` is `1.1` before the
   call, so fractional numbers are rejected: `compare(1.10, "1.9")` is
   `undefined`. Integer numbers (32-bit) are accepted as a major version
   (`compare(2, 10)` is `-1`); `NaN` / `Infinity` give `undefined`.
5. **Default `type` is `MajorMinorPatch` (7): the build number is ignored.**
   Pass `VersionCompare.All` when the build counter matters (e.g. versions
   from `Script.getExtensionList()`, which look like `"5.9.0.6"`).
6. **`type` is a bit mask, checked in the fixed order major → minor → patch
   → build**; the first selected part that differs decides. Combine with
   `|` (`VersionCompare.Major | VersionCompare.Build` = 9).
7. **A `type` with no valid bits always returns `0`**: `null`, `false`,
   `0`, `NaN`, negative, `16`, `32`. Only `undefined` / a missing argument
   selects the default. Never pass `null` for "default".
8. `compare` never throws and has no state; each thread requires the
   extension itself.
9. Only `compare` is exposed. To get the parts in a script:
   `version.split(".")` + `Convert.toNumber`. `getVersion` / `setVersion`
   exist only in C++ (`XYO::System::ApplicationVersion`).
10. Not built into `fabricare` (it does not register this extension); the
    DLL is loaded from the SDK `bin` folder. `quantum-script--magnet` hosts
    register it as internal. Either way, scripts call
    `Script.requireExtension("ApplicationVersion")`.

## Recipes

```javascript
function isAtLeast(v, min) { var r = ApplicationVersion.compare(v, min); return (!Script.isUndefined(r)) && (r >= 0); };
function inRange(v, min, limit) { return (ApplicationVersion.compare(v, min) >= 0) && (ApplicationVersion.compare(v, limit) < 0); };   // invalid -> false
function isCompatible(v, req) {   // same major, not older
	return (ApplicationVersion.compare(v, req, VersionCompare.Major) == 0) && (ApplicationVersion.compare(v, req) >= 0);
};
var sorted = list.sort(function(a, b) { return ApplicationVersion.compare(a, b); });   // sort returns a NEW array; all entries must be valid
```

## C++

```cpp
#include <XYO/QuantumScript.Extension/ApplicationVersion.hpp>
using namespace XYO::QuantumScript;

Extension::ApplicationVersion::registerInternalExtension(executive);   // host init callback; scripts still requireExtension

// native code: call xyo-system directly, no script needed
int result;
if (XYO::System::ApplicationVersion::compare("1.10", "1.9", result, XYO::System::ApplicationVersion::CompareAll)) {
	// result: -1 / 0 / 1
};
```

- `fabricare.json` dependency: `"quantum-script--applicationversion"`
  (`dll-or-lib`: DLL on dynamic platforms, static lib on `*.static`
  platforms; there is no separate `.static` project). Static hosts must
  register it as internal.
- `initExecutive` builds the `VersionCompare` object with `compileStringX`
  from the `XYO::System::ApplicationVersion::Compare*` enum, then
  `setFunction2("ApplicationVersion.compare(strA,strB,type)", ...)`.

## Working in this repository

- Build: `fabricare make`, `fabricare test` (runs `test/test.01`, which
  registers Console and ApplicationVersion as internal extensions and runs
  `test/test.01.js`; run `make` first), `fabricare install` (see the
  `fabricare` skill). `quantum-script` and `quantum-script--console` must
  be installed first.
- Quick manual check against the installed DLL:
  `quantum-script file.js` with
  `Script.requireExtension("ApplicationVersion"); Console.writeLn(ApplicationVersion.compare("1.10", "1.9"));`.
- Behavior changes belong in `xyo-system` (`ApplicationVersion.cpp`); this
  repository only binds it. When the script API changes, update
  `README.md`, `docs/script-api.md`, `docs/reference.md` and this skill.
- Code style: tabs (width 8), `.clang-format`, CRLF, statements and blocks
  end with `};`, camelCase. SPDX header: MIT for `source/` and `docs/`,
  Unlicense for `test/` and `.claude/` (see `.reuse/dep5`).
