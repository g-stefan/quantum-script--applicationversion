# Getting started

## 1. Build and install

The extension is built with [fabricare](https://github.com/g-stefan/fabricare),
the build tool used by all XYO C++ projects. `quantum-script` (and everything
below it: `xyo-system`, `xyo-encoding`, ...) and `quantum-script--console`
must be installed to the SDK first. From the repository root:

```bash
fabricare make       # build into output/
fabricare test       # build and run test/test.01 (run make first)
fabricare install    # copy output/{bin,include,lib} to ~/.fabricare/<platform>
fabricare clean      # remove output/ and temp/
```

The project `quantum-script--applicationversion` is of kind `dll-or-lib`:

| Platform | Result | Use it when |
|----------|--------|-------------|
| dynamic (`win64-msvc-2026`, `ubuntu-*`) | `quantum-script--applicationversion.dll` / `libquantum-script--applicationversion.so` + import library | scripts run by `quantum-script`, or a host using the engine DLL |
| static (`win64-msvc-2026.static`) | `quantum-script--applicationversion.lib`, static CRT | self-contained hosts built with the static engine |

After `fabricare install`, the DLL (Windows) / shared library (Linux) sits in
the SDK `bin` folder next to `quantum-script.exe`, which is where
`Script.requireExtension("ApplicationVersion")` finds it.

## 2. Use it from a script

`check-version.js`:

```javascript
Script.requireExtension("Console");
Script.requireExtension("ApplicationVersion");

var installed = "1.10.2";
var required = "1.9";

var r = ApplicationVersion.compare(installed, required);
if (Script.isUndefined(r)) {
	Console.writeLn("invalid version");
} else if (r < 0) {
	Console.writeLn("version " + required + " or newer is required, found " + installed);
} else {
	Console.writeLn("ok");      // printed: 1.10 is newer than 1.9
};
```

Run it with:

```bash
quantum-script check-version.js
```

`Script.requireExtension("ApplicationVersion")` looks for an external
`quantum-script--applicationversion` library first (the file as named, then
every include path folder: next to the interpreter, next to the script),
then for an internal extension registered by the host. Loading twice does
nothing. A missing extension throws `Unable to open "ApplicationVersion"`.

The extension creates two global objects, `VersionCompare` (the `type`
constants) and `ApplicationVersion` (the `compare` function). Do not define
your own globals with these names.

## 3. fabricare build scripts

`fabricare` does **not** register `ApplicationVersion` as an internal
extension. A build script that needs it requires it like any script; the
library is loaded from the SDK `bin` folder (installed in step 1):

```javascript
Script.requireExtension("Console");
Script.requireExtension("Shell");
Script.requireExtension("JSON");
Script.requireExtension("ApplicationVersion");

var json = JSON.decode(Shell.fileGetContents("version.json"));
var current = json["quantum-script--applicationversion"].version;
if (ApplicationVersion.compare(current, "5.0.0") < 0) {
	Console.writeLn("version 5.0.0 or newer expected, found " + current);
	Script.exit(1);
};
```

Hosts that bundle the `Magnet` extension (`quantum-script--magnet`) get
`ApplicationVersion` registered as an internal extension; scripts still
call `Script.requireExtension("ApplicationVersion")`.

## 4. Register it in a C++ host

A host that embeds Quantum Script makes `ApplicationVersion` available as an
internal extension by registering it in the init callback (this is what
`test/test.01.cpp` does):

```cpp
#include <XYO/QuantumScript.hpp>
#include <XYO/QuantumScript.Extension/Console.hpp>
#include <XYO/QuantumScript.Extension/ApplicationVersion.hpp>

using namespace XYO::QuantumScript;

void initExecutive(Executive *executive) {
	Extension::Console::registerInternalExtension(executive);
	Extension::ApplicationVersion::registerInternalExtension(executive);
};

int main(int cmdN, char *cmdS[]) {
	if (ExecutiveX::initExecutive(cmdN, cmdS, initExecutive)) {
		if (!ExecutiveX::executeString(
		        "Script.requireExtension(\"Console\");"
		        "Script.requireExtension(\"ApplicationVersion\");"
		        "Console.writeLn(ApplicationVersion.compare(\"1.10\", \"1.9\"));")) {
			printf("%s\n", (ExecutiveX::getError()).value());
			printf("%s", (ExecutiveX::getStackTrace()).value());
		};
		ExecutiveX::endProcessing();
	};
	return 0;
};
```

Registering only makes the extension *available*: scripts still call
`Script.requireExtension("ApplicationVersion")`. With the DLL build of the
engine an external `quantum-script--applicationversion.dll` found on the
include path wins over the internal one for `requireExtension`; use
`Script.requireInternalExtension("ApplicationVersion")` to force the
internal one.

In the host's `fabricare.json`:

```json
{
	"name": "my-host",
	"make": "exe",
	"sourcePath": "XYO/MyHost",
	"dependency": [
		"quantum-script--applicationversion"
	]
}
```

C++ code that only needs to compare versions does not need this extension:
call `XYO::System::ApplicationVersion::compare` from `xyo-system` directly
(see [Script API](script-api.md#the-c-side)).

## 5. Static builds

Build the host on the static platform (`win64-msvc-2026.static`): the same
`quantum-script--applicationversion` dependency then links the static
library. A static host can not load external DLLs, so it must register the
extension with `registerInternalExtension` (section 4). In static builds
the `quantumScriptExtension` DLL entry point is not compiled
(`XYO_PLATFORM_COMPILE_DYNAMIC_LIBRARY` is not defined).

## 6. Threads

Each thread that runs scripts has its own engine, so every thread loads the
extension itself with `Script.requireExtension("ApplicationVersion")`.
`compare` has no state: it is safe to call from any number of threads.
