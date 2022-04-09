//
// Quantum Script Extension Version
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_EXTENSION_VERSION_HPP
#define QUANTUM_SCRIPT_EXTENSION_VERSION_HPP

#ifndef QUANTUM_SCRIPT_HPP
#	include "quantum-script.hpp"
#endif

#ifndef QUANTUM_SCRIPT_EXTENSION_VERSION__EXPORT_HPP
#	include "quantum-script-extension-version--export.hpp"
#endif

#ifndef QUANTUM_SCRIPT_EXTENSION_VERSION_COPYRIGHT_HPP
#	include "quantum-script-extension-version-copyright.hpp"
#endif

#ifndef QUANTUM_SCRIPT_EXTENSION_VERSION_LICENSE_HPP
#	include "quantum-script-extension-version-license.hpp"
#endif

#ifndef QUANTUM_SCRIPT_EXTENSION_VERSION_VERSION_HPP
#	include "quantum-script-extension-version-version.hpp"
#endif

namespace Quantum {
	namespace Script {
		namespace Extension {
			namespace Version {

				using namespace Quantum::Script;

				QUANTUM_SCRIPT_EXTENSION_VERSION_EXPORT void initExecutive(Executive *executive, void *extensionId);
				QUANTUM_SCRIPT_EXTENSION_VERSION_EXPORT void registerInternalExtension(Executive *executive);

			};
		};
	};
};

#endif
