# Quantum Script Extension ApplicationVersion

Quantum Script extension
- Compare version strings `major.minor.patch.build` numerically, part by
part (`1.10` is newer than `1.9`, `1.2` equals `1.2.0.0`).
- Choose the parts compared with the `VersionCompare` bit mask; the default
ignores the build number.
- Check minimum versions, version ranges, update availability, sort
releases.

```javascript
Script.requireExtension("ApplicationVersion");

VersionCompare.Major;             // 1
VersionCompare.Minor;             // 2
VersionCompare.Patch;             // 4
VersionCompare.Build;             // 8
VersionCompare.MajorMinor;        // 3
VersionCompare.MajorMinorPatch;   // 7, default
VersionCompare.All;               // 15

ApplicationVersion.compare(strA,strB,type);   // -1 older, 0 equal, 1 newer, undefined invalid
```

Built on `quantum-script` and `xyo-system`, part of the XYO C++ SDK.

## Documentation

- [Overview](docs/README.md) - purpose and design
- [Getting started](docs/getting-started.md) - build, load from a script, register in a C++ host, static builds
- [Version format](docs/version-format.md) - how version strings are parsed
- [Script API](docs/script-api.md) - `compare`, the `type` mask, recipes
- [API reference](docs/reference.md)

A Claude Code skill for this extension is in
[.claude/skills/quantum-script--applicationversion](.claude/skills/quantum-script--applicationversion/SKILL.md).

## License

Copyright (c) 2016-2026 Grigore Stefan
Licensed under the [MIT](LICENSE) license.
