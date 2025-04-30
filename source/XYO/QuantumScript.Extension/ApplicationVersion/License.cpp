// Quantum Script Extension ApplicationVersion
// Copyright (c) 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <XYO/QuantumScript.Extension/ApplicationVersion/License.hpp>
#include <XYO/QuantumScript.Extension/ApplicationVersion/Copyright.hpp>

namespace XYO::QuantumScript::Extension::ApplicationVersion::License {

	std::string license() {
		std::string retV;
		retV += Platform::License::licenseMITHeader();
		retV += Copyright::copyright();
		retV += "\r\n";
		retV += Platform::License::licenseMITContent();
		return retV;
	};

	std::string shortLicense() {
		std::string retV;
		retV += Copyright::copyright();
		retV += "\r\n";
		retV += Platform::License::licenseMITShort();
		return retV;
	};

};
