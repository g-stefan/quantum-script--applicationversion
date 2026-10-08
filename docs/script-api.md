# Script API

Everything the extension defines after
`Script.requireExtension("ApplicationVersion")`: two global objects,
`VersionCompare` and `ApplicationVersion`.

## `VersionCompare`

Bit flags that select which parts of a version are compared:

| Constant | Value | Parts compared |
|----------|-------|----------------|
| `VersionCompare.Major` | `1` | major |
| `VersionCompare.Minor` | `2` | minor |
| `VersionCompare.Patch` | `4` | patch |
| `VersionCompare.Build` | `8` | build |
| `VersionCompare.MajorMinor` | `3` | major, minor |
| `VersionCompare.MajorMinorPatch` | `7` | major, minor, patch — **the default** |
| `VersionCompare.All` | `15` | major, minor, patch, build |

The constants are plain numbers on a plain object; combine them with `|`
(`VersionCompare.Major | VersionCompare.Build` is `9`). The object is not
read-only, but do not change it.

## `ApplicationVersion.compare(versionA, versionB, type)`

Compares two versions part by part, as integers. Returns:

| Result | Meaning |
|--------|---------|
| `-1` | `versionA` is **older** than `versionB` |
| `0` | equal in every compared part |
| `1` | `versionA` is **newer** than `versionB` |
| `undefined` | `versionA` or `versionB` could not be parsed (see [Version format](version-format.md)) |

```javascript
ApplicationVersion.compare("1.2.3", "1.2.4");   // -1
ApplicationVersion.compare("1.10", "1.9");      // 1
ApplicationVersion.compare("1.2", "1.2.0.0");   // 0
ApplicationVersion.compare("v1.2", "1.2");      // undefined
```

The order of the arguments matters: `compare(a, b)` is `-compare(b, a)`.
Read it as "the sign of `a - b`": `compare(installed, required) >= 0` means
"installed is at least required".

### Arguments

- `versionA`, `versionB`: converted with `toString`, then parsed (see
  [Version format](version-format.md)). Pass strings. An integer number
  is accepted as a major version (`2` is `"2"`); a fractional number
  (`1.10`, which is the number `1.1`) is ambiguous and gives `undefined`.
  A missing argument becomes `"undefined"` and the result is `undefined`.
- `type` (optional): bit mask of `VersionCompare` flags.
  - Missing or `undefined`: `VersionCompare.MajorMinorPatch` (`7`) — the
    **build number is ignored** by default.
  - Anything else is converted with the engine's index conversion: to a
    number, fraction truncated; `NaN`, `±Infinity` and negative values
    become `0`. Numeric strings work (`"15"`), `true` is `1`.

### How `type` is applied

The parts are always checked in the order **major, minor, patch, build**.
For each part whose bit is set in `type`, the two values are compared; the
first difference decides the result. Parts whose bit is not set are
skipped. If no selected part differs, the result is `0`.

```javascript
var a = "2.0.0.1", b = "1.9.0.7";
ApplicationVersion.compare(a, b);                                            // 1  (major 2 > 1)
ApplicationVersion.compare(a, b, VersionCompare.Minor);                      // -1 (minor 0 < 9, major ignored)
ApplicationVersion.compare(a, b, VersionCompare.Build);                      // -1 (build 1 < 7)
ApplicationVersion.compare("1.5.0.1", "1.3.0.2", VersionCompare.Major);      // 0  (same major)
ApplicationVersion.compare("1.2.3.4", "1.2.3.9");                            // 0  (build ignored by default)
ApplicationVersion.compare("1.2.3.4", "1.2.3.9", VersionCompare.All);        // -1
```

A `type` with no valid bits compares nothing and **always returns `0`**:

| `type` | Becomes | Result |
|--------|---------|--------|
| `undefined` / missing | `7` | normal comparison |
| `null` | `0` | always `0` |
| `0`, `NaN`, `"abc"`, negative | `0` | always `0` |
| `16` or more with no low bits (`16`, `32`) | no part selected | always `0` |
| `1.9` | `1` | major only |

Pass a `VersionCompare` constant, or leave `type` out; never pass `null`
to mean "default".

### Testing the result

`undefined` (invalid version) and `0` (equal) are both falsy, and
`undefined < 0` / `undefined >= 0` are both `false` in Quantum Script
comparisons. Decide what an invalid version means for your script and test
for it explicitly:

```javascript
var r = ApplicationVersion.compare(found, required);
if (Script.isUndefined(r)) {
	throw "invalid version: " + found;
};
if (r < 0) {
	throw "version " + required + " or newer is required, found " + found;
};
```

Never write `if (ApplicationVersion.compare(a, b))` to mean "different":
it is also `false` for invalid input.

## Recipes

### Minimum version

```javascript
function isAtLeast(version, minimum) {
	var r = ApplicationVersion.compare(version, minimum);
	return (!Script.isUndefined(r)) && (r >= 0);
};

isAtLeast("2.1.0", "2.0");   // true
isAtLeast("1.9.9", "2.0");   // false
isAtLeast("v2.1", "2.0");    // false (invalid)
```

### Version range

```javascript
// minimum <= version < limit
function inRange(version, minimum, limit) {
	return (ApplicationVersion.compare(version, minimum) >= 0) &&
	       (ApplicationVersion.compare(version, limit) < 0);
};

inRange("1.4.2", "1.2", "2.0");   // true
inRange("2.0.0", "1.2", "2.0");   // false
```

An invalid `version` gives `false` here, because both comparisons with
`undefined` are `false`.

### Same major version (semantic versioning compatibility)

```javascript
function isCompatible(version, required) {
	return (ApplicationVersion.compare(version, required, VersionCompare.Major) == 0) &&
	       (ApplicationVersion.compare(version, required) >= 0);
};

isCompatible("3.4.1", "3.2.0");   // true
isCompatible("4.0.0", "3.2.0");   // false: new major version
```

### Sort versions

`compare` has the shape of a sort callback. Array `sort` returns a new
array in Quantum Script:

```javascript
var list = ["1.10.0", "1.2.0", "1.9.1", "0.9"];
var sorted = list.sort(function(a, b) {
	return ApplicationVersion.compare(a, b);
});
// ["0.9", "1.2.0", "1.9.1", "1.10.0"]
```

All entries must be valid versions; an `undefined` result breaks the sort
order. Pass `VersionCompare.All` inside the callback to order by build
number as well.

### Newest of a list

```javascript
function newest(list) {
	var best = undefined;
	var k;
	for (k = 0; k < list.length; ++k) {
		if (Script.isUndefined(best) || ApplicationVersion.compare(list[k], best, VersionCompare.All) > 0) {
			best = list[k];
		};
	};
	return best;
};

newest(["5.9.0.6", "5.10.0.1", "5.9.0.12"]);   // "5.10.0.1"
```

### Check the version of a loaded extension

`Script.getExtensionList()` reports each loaded extension with a `version`
string such as `"5.9.0.6"` (version and build):

```javascript
Script.requireExtension("ApplicationVersion");

function extensionVersion(name) {
	var list = Script.getExtensionList();
	var k;
	for (k in list) {
		if (list[k].name == name) {
			return list[k].version;
		};
	};
	return undefined;
};

if (ApplicationVersion.compare(extensionVersion("ApplicationVersion"), "5.9.0.6", VersionCompare.All) < 0) {
	throw "ApplicationVersion 5.9.0.6 or newer required";
};
```

## The C++ side

The script function is a thin wrapper over
`XYO::System::ApplicationVersion` (in `xyo-system`):

```cpp
namespace XYO::System::ApplicationVersion {
	bool getVersion(const char *version, int &major, int &minor, int &patch, int &build);
	String setVersion(int major, int minor, int patch, int build);   // "1.2.3.4"
	enum {
		CompareMajor = 1, CompareMinor = 2, ComparePatch = 4, CompareBuild = 8,
		CompareMajorMinor = 3, CompareMajorMinorPatch = 7, CompareAll = 15
	};
	bool compare(const char *versionA, const char *versionB, int &result, int type = CompareMajorMinorPatch);
};
```

`compare` returns `false` when a version can not be parsed (the script
function then returns `undefined`); otherwise it sets `result` to
`-1`, `0` or `1`. The `VersionCompare` constants are generated from this
enum when the extension is loaded, so script and C++ always agree.

Native code that only needs to compare versions should call this function
directly instead of going through a script. `getVersion` and `setVersion`
are not exposed to scripts; split a version with
`version.split(".")` and `Convert.toNumber` if a script needs the parts.
