// Quantum Script Extension ApplicationVersion
// Copyright (c) 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_EXTENSION_APPLICATIONVERSION_COPYRIGHT_HPP
#define XYO_QUANTUMSCRIPT_EXTENSION_APPLICATIONVERSION_COPYRIGHT_HPP

#ifndef XYO_QUANTUMSCRIPT_EXTENSION_APPLICATIONVERSION_DEPENDENCY_HPP
#	include <XYO/QuantumScript.Extension/ApplicationVersion/Dependency.hpp>
#endif

namespace XYO::QuantumScript::Extension::ApplicationVersion::Copyright {
	XYO_QUANTUMSCRIPT_EXTENSION_APPLICATIONVERSION_EXPORT std::string copyright();
	XYO_QUANTUMSCRIPT_EXTENSION_APPLICATIONVERSION_EXPORT std::string publisher();
	XYO_QUANTUMSCRIPT_EXTENSION_APPLICATIONVERSION_EXPORT std::string company();
	XYO_QUANTUMSCRIPT_EXTENSION_APPLICATIONVERSION_EXPORT std::string contact();
};

#endif
