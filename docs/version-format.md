# Version format

A version is up to four decimal integers separated by dots:

```
major[.minor[.patch[.build]]]
```

Both arguments of `ApplicationVersion.compare` are converted to strings and
parsed the same way, by `XYO::System::ApplicationVersion::getVersion`
(`sscanf(version, "%d.%d.%d.%d", ...)`):

1. All four parts start as `0`.
2. Numbers are read from the left, one per part, separated by a single `.`.
3. Reading stops at the first character that does not fit the pattern; the
   parts read so far are kept, the rest stay `0`.
4. The version is **valid if at least `major` was read**. Otherwise
   `compare` returns `undefined`.

## Examples

| Version string | Parsed as | Notes |
|----------------|-----------|-------|
| `"1.2.3.4"` | 1 . 2 . 3 . 4 | all parts |
| `"1.2.3"` | 1 . 2 . 3 . 0 | missing build is 0 |
| `"7.10"` | 7 . 10 . 0 . 0 | `"7.10"` equals `"7.10.0.0"` |
| `"2"` | 2 . 0 . 0 . 0 | a single number is a valid version |
| `"1.10.0"` vs `"1.9.0"` | 10 > 9 | numeric, not text: 1.10 is newer |
| `"01.02"` | 1 . 2 . 0 . 0 | leading zeros ignored, equals `"1.2"` |
| `" 1.2"`, `"1. 2"` | 1 . 2 . 0 . 0 | whitespace before a number is skipped |
| `"+1"` | 1 . 0 . 0 . 0 | a sign is accepted |
| `"-1"` | -1 . 0 . 0 . 0 | negative numbers are read as such; `-1` is older than `0` |
| `"1.2.3-rc1"` | 1 . 2 . 3 . 0 | suffix ignored: **equal to `"1.2.3"`** |
| `"1.2-beta"` | 1 . 2 . 0 . 0 | suffix ignored |
| `"1.x"` | 1 . 0 . 0 . 0 | stops at `x` |
| `"1..3"` | 1 . 0 . 0 . 0 | stops at the second `.`; the `3` is **not** read |
| `"1.2.3.4.5"` | 1 . 2 . 3 . 4 | a fifth part is ignored |
| `"v1.2"` | — | does not start with a number: `undefined` |
| `""` | — | `undefined` |
| `undefined` (missing argument) | — | converted to `"undefined"`: `undefined` |

## Numbers instead of strings

A number argument is accepted **only if it is an integer** in 32-bit
signed range; it is then used as the major version. A fractional number is
rejected and `compare` returns `undefined`:

| Argument | Result |
|----------|--------|
| `2` | `"2"`, parsed as 2 . 0 . 0 . 0 |
| `-1` | `"-1"`, parsed as -1 . 0 . 0 . 0 |
| `1.5`, `1.10` | `undefined` — fractional |
| `NaN`, `Infinity`, `4294967296` | `undefined` — not a 32-bit integer |

Fractions are rejected because the number has already lost the version's
spelling before the call: `1.10`, `1.1` and `1.100` are the same number, so
`compare(1.10, "1.9")` could only guess (and would answer "older"). Pass
versions as strings: `compare("1.10", "1.9")` is `1`.

## What is not supported

| Not supported | What happens | Do instead |
|---------------|--------------|------------|
| Prefixes (`v1.2`, `release-1.2`) | `undefined` | strip the prefix first: `version.substring(1)` |
| Pre-release ordering (`1.0.0-alpha < 1.0.0`) | suffix ignored, versions compare equal | compare the suffixes yourself after `compare` returns `0` |
| Build metadata (`1.0.0+20260101`) | ignored | — |
| More than 4 parts | parts 5+ ignored | — |
| Numbers above 2147483647 | out of range for `%d`, result not defined | keep each part within 32-bit signed range |
| Separators other than `.` (`1-2-3`, `1_2`) | only the first number is read | replace them with `.` first |

The parsing is deliberately lenient: a version string read from a file
name, a tool's `--version` output or a tag usually compares correctly as
long as it starts with the number. The flip side is that `compare` reports
an error **only** when the first character is not part of a number, so
validate input yourself when a malformed version must be rejected.
