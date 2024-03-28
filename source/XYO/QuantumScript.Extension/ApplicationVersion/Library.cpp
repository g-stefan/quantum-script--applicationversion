// Quantum Script Extension ApplicationVersion
// Copyright (c) 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <XYO/QuantumScript.Extension/ApplicationVersion/Library.hpp>
#include <XYO/QuantumScript.Extension/ApplicationVersion/Copyright.hpp>
#include <XYO/QuantumScript.Extension/ApplicationVersion/License.hpp>
#include <XYO/QuantumScript.Extension/ApplicationVersion/Version.hpp>

namespace XYO::QuantumScript::Extension::ApplicationVersion {

	static TPointer<Variable> applicationVersionCompare(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUMSCRIPT_DEBUG_RUNTIME
		printf("- application-version-compare\n");
#endif

		int result;
		int type = System::ApplicationVersion::CompareMajorMinorPatch;

		if (!TIsTypeExact<VariableUndefined>(arguments->index(2))) {
			type = (arguments->index(2))->toIndex();
		};

		if (System::ApplicationVersion::compare((arguments->index(0))->toString(), (arguments->index(1))->toString(), result, type)) {
			return VariableNumber::newVariable(result);
		};

		return Context::getValueUndefined();
	};

	void registerInternalExtension(Executive *executive) {
		executive->registerInternalExtension("ApplicationVersion", initExecutive);
	};

	void initExecutive(Executive *executive, void *extensionId) {

		String info = "ApplicationVersion\r\n";
		info << ApplicationVersion::License::shortLicense().c_str();

		executive->setExtensionName(extensionId, "ApplicationVersion");
		executive->setExtensionInfo(extensionId, info);
		executive->setExtensionVersion(extensionId, Extension::ApplicationVersion::Version::versionWithBuild());
		executive->setExtensionPublic(extensionId, true);

		//
		char buffer[128];
		//
		executive->compileStringX("var VersionCompare={};");
		sprintf(buffer, "VersionCompare.Major=%d;", System::ApplicationVersion::CompareMajor);
		executive->compileStringX(buffer);
		sprintf(buffer, "VersionCompare.Minor=%d;", System::ApplicationVersion::CompareMinor);
		executive->compileStringX(buffer);
		sprintf(buffer, "VersionCompare.Patch=%d;", System::ApplicationVersion::ComparePatch);
		executive->compileStringX(buffer);
		sprintf(buffer, "VersionCompare.Build=%d;", System::ApplicationVersion::CompareBuild);
		executive->compileStringX(buffer);
		sprintf(buffer, "VersionCompare.MajorMinor=%d;", System::ApplicationVersion::CompareMajorMinor);
		executive->compileStringX(buffer);
		sprintf(buffer, "VersionCompare.MajorMinorPatch=%d;", System::ApplicationVersion::CompareMajorMinorPatch);
		executive->compileStringX(buffer);
		sprintf(buffer, "VersionCompare.All=%d;", System::ApplicationVersion::CompareAll);
		executive->compileStringX(buffer);

		//
		executive->compileStringX("var ApplicationVersion={};");
		executive->setFunction2("ApplicationVersion.compare(strA,strB,type)", applicationVersionCompare);
	};

};

#ifdef XYO_PLATFORM_COMPILE_DYNAMIC_LIBRARY
extern "C" XYO_QUANTUMSCRIPT_EXTENSION_APPLICATIONVERSION_EXPORT void quantumScriptExtension(XYO::QuantumScript::Executive *executive, void *extensionId) {
	XYO::QuantumScript::Extension::ApplicationVersion::initExecutive(executive, extensionId);
};
#endif
