# Quantum Script Extension ApplicationVersion — Documentation

`quantum-script--applicationversion` is the **version comparison extension
of Quantum Script**. Loaded with
`Script.requireExtension("ApplicationVersion")`, it lets a script compare two
version strings of the form `major.minor.patch.build` **numerically, part by
part**, and choose which parts take part in the comparison.

Comparing versions as text is wrong as soon as a part has two digits:
`"1.10.0" < "1.9.0"` is `true` for strings, but 1.10 is the newer release.
`ApplicationVersion.compare` reads each part as an integer, so `1.10` is
newer than `1.9`, `1.2` equals `1.2.0.0`, and `01.02` equals `1.2`.

- **One function.** `ApplicationVersion.compare(versionA, versionB, type)`
  returns `-1` (A is older), `0` (equal) or `1` (A is newer), or
  `undefined` when a version can not be parsed.
- **Selectable parts.** `type` is a bit mask built from the `VersionCompare`
  constants (`Major`, `Minor`, `Patch`, `Build`, and the combinations
  `MajorMinor`, `MajorMinorPatch`, `All`). The default,
  `MajorMinorPatch`, ignores the build number.
- **Lenient parsing.** 1 to 4 numeric parts, missing parts are `0`, anything
  after the last number is ignored (`"1.2.3-rc1"` is `1.2.3.0`). A version
  must start with a number: `"v1.2"` and `""` give `undefined`.

```
scripts: fabricare build scripts, installers, update checks, quantum-script .js
quantum-script--applicationversion   <-- this extension: VersionCompare, ApplicationVersion.compare
quantum-script                       (Executive, Variable, Context)
xyo-system                           (XYO::System::ApplicationVersion::compare, the implementation)
xyo-encoding, xyo-multithreading, xyo-data-structures, xyo-managed-memory, xyo-platform
```

## Why it exists

| Need | What the extension gives |
|------|--------------------------|
| "Is the installed version at least the required one?" | `ApplicationVersion.compare(installed, required) >= 0` |
| "Is a newer release available?" | `ApplicationVersion.compare(latest, current) > 0` |
| Compatibility by major version only (semantic versioning) | `type = VersionCompare.Major` |
| Ignore or include the build counter | default ignores it, `VersionCompare.All` includes it |
| Sort a list of versions | `list.sort(function(a, b) { return ApplicationVersion.compare(a, b); })` |
| The same rules as the C++ side | it calls `XYO::System::ApplicationVersion::compare` directly |

## Concepts at a glance

| Need | Use | Notes |
|------|-----|-------|
| Load the extension | `Script.requireExtension("ApplicationVersion");` | once per script, defines `VersionCompare` and `ApplicationVersion` |
| Compare two versions | `ApplicationVersion.compare("1.10", "1.9")` | `1`; result is `-1`, `0`, `1` or `undefined` |
| Include the build number | `ApplicationVersion.compare(a, b, VersionCompare.All)` | default is `VersionCompare.MajorMinorPatch` |
| Compare only some parts | `VersionCompare.Major \| VersionCompare.Build` | any bit mask of `1, 2, 4, 8`; parts are always checked major first |
| Detect a bad version | `Script.isUndefined(r)` / `r === undefined` | `undefined` is falsy, like `0`: never test the result with `if (r)` |
| Version format | `major[.minor[.patch[.build]]]` | decimal integers, missing parts `0`, trailing text ignored |

## Contents

| Document | What it covers |
|----------|----------------|
| [Getting started](getting-started.md) | Build and install, load the extension from a script, register it in a C++ host, static builds |
| [Version format](version-format.md) | How a version string is parsed: parts, missing parts, leading zeros, suffixes, invalid input |
| [Script API](script-api.md) | `VersionCompare`, `ApplicationVersion.compare`: arguments, the `type` mask, results, recipes |
| [API reference](reference.md) | Every script and C++ symbol on one page |

Quantum Script itself (the language, `Script.requireExtension`, embedding,
writing extensions) is documented in the `quantum-script` repository,
`docs/`; the underlying `XYO::System::ApplicationVersion` in the
`xyo-system` repository, `docs/application.md`.

## Source map

```
source/XYO/QuantumScript.Extension/ApplicationVersion.hpp            umbrella header, include this from C++
source/XYO/QuantumScript.Extension/ApplicationVersion.Amalgam.cpp    the whole extension in one translation unit
source/XYO/QuantumScript.Extension/ApplicationVersion/
    Dependency.hpp                                                   <XYO/QuantumScript.hpp>, export macro
    Library[.hpp/.cpp]                                               initExecutive, registerInternalExtension,
                                                                     VersionCompare constants, compare()
    Library.rc / Library.rh                                          Windows version resource of the DLL
    Copyright / License / Version                                    library metadata
test/test.01.cpp                                                     C++ host registering Console and ApplicationVersion as internal
test/test.01.js                                                      compare() checks: ordering, type mask, invalid input, numbers
```

## AI assistant skill

A Claude Code skill describing how to use this extension lives in
[`.claude/skills/quantum-script--applicationversion/`](../.claude/skills/quantum-script--applicationversion/SKILL.md).
It is picked up automatically inside this repository; copy the folder to
`~/.claude/skills/` to have it available in the projects that compare
versions (fabricare scripts, Quantum Script tools, other extensions).
