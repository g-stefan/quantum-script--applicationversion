# API reference

## Script

Available after `Script.requireExtension("ApplicationVersion")`.

### `VersionCompare`

| Symbol | Value | Parts compared |
|--------|-------|----------------|
| `VersionCompare.Major` | `1` | major |
| `VersionCompare.Minor` | `2` | minor |
| `VersionCompare.Patch` | `4` | patch |
| `VersionCompare.Build` | `8` | build |
| `VersionCompare.MajorMinor` | `3` | major, minor |
| `VersionCompare.MajorMinorPatch` | `7` | major, minor, patch (default) |
| `VersionCompare.All` | `15` | all four parts |

### `ApplicationVersion`

| Symbol | Returns | Notes |
|--------|---------|-------|
| `ApplicationVersion.compare(versionA, versionB, type)` | `-1`, `0`, `1` or `undefined` | `-1`: A older, `0`: equal, `1`: A newer, `undefined`: a version could not be parsed; `type` defaults to `VersionCompare.MajorMinorPatch` |

### Version format

| Rule | Example |
|------|---------|
| `major[.minor[.patch[.build]]]`, decimal integers | `"1.2.3.4"` |
| missing parts are `0` | `"7.10"` = `"7.10.0.0"` |
| compared numerically | `"1.10"` > `"1.9"` |
| leading zeros, leading whitespace, `+` ignored | `"01.02"` = `"1.2"` |
| reading stops at the first unexpected character | `"1.2.3-rc1"` = `"1.2.3"`, `"1..3"` = `"1"` |
| must start with a number | `"v1.2"`, `""` → `undefined` |
| arguments converted with `toString` | `2` → `"2"` |
| fractional / non 32-bit integer numbers rejected | `1.10`, `NaN`, `4294967296` → `undefined` |

### `type` conversion

| `type` | Used as |
|--------|---------|
| missing / `undefined` | `7` |
| number | truncated (`1.9` → `1`) |
| numeric string | converted (`"15"` → `15`) |
| `true` | `1` |
| `null`, `false`, `NaN`, negative, `±Infinity` | `0`: nothing compared, always `0` |

### Errors

| Message | Cause |
|---------|-------|
| `Unable to open "ApplicationVersion"` | the extension library was not found and no internal one is registered |

`compare` itself never throws: invalid versions give `undefined`.

## C++

Namespace `XYO::QuantumScript::Extension::ApplicationVersion`, umbrella
header `<XYO/QuantumScript.Extension/ApplicationVersion.hpp>`.

### Library (`ApplicationVersion/Library.hpp`)

| Symbol | Notes |
|--------|-------|
| `void registerInternalExtension(Executive *executive)` | register `"ApplicationVersion"` as an internal extension |
| `void initExecutive(Executive *executive, void *extensionId)` | extension init, run by the engine: sets name, info, version, public flag; defines `VersionCompare` and `ApplicationVersion.compare` |
| `extern "C" void quantumScriptExtension(Executive *, void *)` | DLL entry point (only when `XYO_PLATFORM_COMPILE_DYNAMIC_LIBRARY` and not `XYO_QUANTUMSCRIPT_EXTENSION_APPLICATIONVERSION_LIBRARY`) |

### Underlying implementation (`xyo-system`, `<XYO/System/ApplicationVersion.hpp>`)

| Symbol | Notes |
|--------|-------|
| `bool XYO::System::ApplicationVersion::getVersion(const char *version, int &major, int &minor, int &patch, int &build)` | parse; `false` if no major number |
| `String XYO::System::ApplicationVersion::setVersion(int major, int minor, int patch, int build)` | `"major.minor.patch.build"` |
| `bool XYO::System::ApplicationVersion::compare(const char *versionA, const char *versionB, int &result, int type = CompareMajorMinorPatch)` | `result` = `-1` / `0` / `1`; `false` if a version is invalid |
| `CompareMajor`, `CompareMinor`, `ComparePatch`, `CompareBuild`, `CompareMajorMinor`, `CompareMajorMinorPatch`, `CompareAll` | `1`, `2`, `4`, `8`, `3`, `7`, `15` |

### Metadata

| Symbol | Notes |
|--------|-------|
| `Version::version()`, `Version::build()`, `Version::versionWithBuild()`, `Version::datetime()` | from `version.json` |
| `Copyright::copyright()`, `Copyright::publisher()`, `Copyright::company()`, `Copyright::contact()` | |
| `License::license()`, `License::shortLicense()` | MIT text |

`Version`, `Copyright` and `License` exist in every XYO library: qualify them
(`Extension::ApplicationVersion::Version::versionWithBuild()`).

### Build configuration

| Name | Meaning |
|------|---------|
| `quantum-script--applicationversion` | fabricare project, `dll-or-lib`: DLL / shared library on dynamic platforms, static library on `*.static` platforms |
| `XYO_QUANTUMSCRIPT_EXTENSION_APPLICATIONVERSION_EXPORT` | export / import macro |
| `XYO_QUANTUMSCRIPT_EXTENSION_APPLICATIONVERSION_INTERNAL` | defined while building the DLL (from `QUANTUM_SCRIPT__APPLICATIONVERSION_INTERNAL`) |
| `XYO_QUANTUMSCRIPT_EXTENSION_APPLICATIONVERSION_LIBRARY` | empty export macro, no DLL entry point |
