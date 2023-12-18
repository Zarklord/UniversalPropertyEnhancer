/****************************************************************************
* Copyright (C) 2018, 2020, 2022 Zarklord
*
* This file is part of UniversalPropertyEnhancer.
*
* UniversalPropertyEnhancer is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with UniversalPropertyEnhancer.  If not, see <http://www.gnu.org/licenses/>.
****************************************************************************/

#include "stdafx.h"
#include "VerifyUPE.h"

#include "UniversalProperty/Duplicator.h"
#include "UniversalProperty/PostInit.h"
#include "UniversalProperty/Replacement.h"

void VerifyUPE::ParseLine(const ArgScript::Line& line) {
	string error_string;
	const bool upr_results = GetUniversalPropertyReplacer().GetTestResults(error_string);
	App::ConsolePrintF("UniversalPropertyReplacer Test Results -> %s", upr_results ? "Passed" : "Failed");
	if (!upr_results)
		App::ConsolePrintF("UniversalPropertyReplacer Errors:\n%s", error_string);

	const bool pld_results = GetPropertyListDuplicator().GetTestResults(error_string);
	App::ConsolePrintF("PropertyListDuplicator Test Results -> %s", pld_results ? "Passed" : "Failed");
	if (!pld_results)
		App::ConsolePrintF("PropertyListDuplicator Errors:\n%s", error_string);

	const bool plpi_results = GetPropertyListPostInitializer().GetTestResults(error_string);
	App::ConsolePrintF("PropertyListPostInitializer Test Results -> %s", plpi_results ? "Passed" : "Failed");
	if (!plpi_results)
		App::ConsolePrintF("PropertyListPostInitializer Errors:\n%s", error_string);
}