// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

Script.requireExtension("Console");
Script.requireExtension("ApplicationVersion");

function check(name, value, expected) {
	if (value !== expected) {
		throw "Test failed: " + name + " => " + value + ", expected " + expected;
	};
};

check("1.2.3 < 1.2.4", ApplicationVersion.compare("1.2.3", "1.2.4"), -1);
check("1.10 > 1.9", ApplicationVersion.compare("1.10", "1.9"), 1);
check("1.2 == 1.2.0.0", ApplicationVersion.compare("1.2", "1.2.0.0"), 0);
check("build ignored by default", ApplicationVersion.compare("1.2.3.4", "1.2.3.9"), 0);
check("build with All", ApplicationVersion.compare("1.2.3.4", "1.2.3.9", VersionCompare.All), -1);
check("Minor only", ApplicationVersion.compare("2.0", "1.9", VersionCompare.Minor), -1);
check("invalid version", ApplicationVersion.compare("v1.2", "1.2"), undefined);
check("missing version", ApplicationVersion.compare("1.2"), undefined);

// numbers: integers are versions, fractions are ambiguous (1.10 is the number 1.1)
check("integer numbers", ApplicationVersion.compare(2, 10), -1);
check("integer number vs string", ApplicationVersion.compare(2, "2.0.0.0", VersionCompare.All), 0);
check("fractional number A", ApplicationVersion.compare(1.10, "1.9"), undefined);
check("fractional number B", ApplicationVersion.compare("1.9", 1.5), undefined);
check("NaN", ApplicationVersion.compare(NaN, "1"), undefined);
check("Infinity", ApplicationVersion.compare(Infinity, "1"), undefined);
check("out of int range", ApplicationVersion.compare(4294967296, "1"), undefined);

Console.writeLn("ApplicationVersion: ok");
