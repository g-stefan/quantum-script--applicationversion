//
// Quantum Script Extension Version
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "quantum-script-extension-version-license.hpp"
#include "quantum-script-extension-version.hpp"
#ifndef QUANTUM_SCRIPT_EXTENSION_VERSION_NO_VERSION
#include "quantum-script-extension-version-version.hpp"
#endif

#include "quantum-script-variablestring.hpp"
#include "quantum-script-variablenumber.hpp"
//

//#define QUANTUM_SCRIPT_VM_DEBUG_RUNTIME

namespace Quantum {
	namespace Script {
		namespace Extension {
			namespace Version {

				using namespace XYO;
				using namespace Quantum::Script;

				static TPointer<Variable> versionCompare(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_DEBUG_RUNTIME
					printf("- version-compare\n");
#endif

					int result;
					int type = Application::Version::CompareMajorMinorPatch;

					if(!TIsTypeExact<VariableUndefined>(arguments->index(2))) {
						type = (arguments->index(2))->toIndex();
					};

					if(Application::Version::compare((arguments->index(0))->toString(), (arguments->index(1))->toString(), result, type)) {
						return VariableNumber::newVariable(result);
					};

					return Context::getValueUndefined();
				};

				void registerInternalExtension(Executive *executive) {
					executive->registerInternalExtension("Version", initExecutive);
				};

				void initExecutive(Executive *executive, void *extensionId) {

					String info = "Version\r\n";
					info << License::shortContent();

					executive->setExtensionName(extensionId, "Version");
					executive->setExtensionInfo(extensionId, info);
#ifndef QUANTUM_SCRIPT_EXTENSION_VERSION_NO_VERSION
					executive->setExtensionVersion(extensionId, Extension::Version::Version::versionWithBuild());
#endif
					executive->setExtensionPublic(extensionId, true);


					//
					char buffer[128];
					//
					executive->compileStringX("var VersionCompare={};");
					sprintf(buffer, "VersionCompare.Major=%d;", Application::Version::CompareMajor);
					executive->compileStringX(buffer);
					sprintf(buffer, "VersionCompare.Minor=%d;", Application::Version::CompareMinor);
					executive->compileStringX(buffer);
					sprintf(buffer, "VersionCompare.Patch=%d;", Application::Version::ComparePatch);
					executive->compileStringX(buffer);
					sprintf(buffer, "VersionCompare.Build=%d;", Application::Version::CompareBuild);
					executive->compileStringX(buffer);
					sprintf(buffer, "VersionCompare.MajorMinor=%d;", Application::Version::CompareMajorMinor);
					executive->compileStringX(buffer);
					sprintf(buffer, "VersionCompare.MajorMinorPatch=%d;", Application::Version::CompareMajorMinorPatch);
					executive->compileStringX(buffer);
					sprintf(buffer, "VersionCompare.All=%d;", Application::Version::CompareAll);
					executive->compileStringX(buffer);

					//
					executive->compileStringX("var Version={};");
					executive->setFunction2("Version.compare(strA,strB,type)", versionCompare);
				};

			};
		};
	};
};

#ifdef XYO_COMPILE_DYNAMIC_LIBRARY
extern "C" QUANTUM_SCRIPT_EXTENSION_VERSION_EXPORT void quantumScriptExtension(Quantum::Script::Executive *executive, void *extensionId) {
	Quantum::Script::Extension::Version::initExecutive(executive, extensionId);
};
#endif

