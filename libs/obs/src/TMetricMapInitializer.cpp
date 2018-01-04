/* +------------------------------------------------------------------------+
   |                     Mobile Robot Programming Toolkit (MRPT)            |
   |                          http://www.mrpt.org/                          |
   |                                                                        |
   | Copyright (c) 2005-2017, Individual contributors, see AUTHORS file     |
   | See: http://www.mrpt.org/Authors - All rights reserved.                |
   | Released under BSD License. See details in http://www.mrpt.org/License |
   +------------------------------------------------------------------------+ */

#include "obs-precomp.h"  // Precomp header

#include <mrpt/maps/TMetricMapInitializer.h>
#include <mrpt/config/CConfigFileBase.h>
#include <mrpt/serialization/CArchive.h>

using namespace std;
using namespace mrpt;
using namespace mrpt::maps;

MRPT_TODO("look git history!");

/** Looks up in the registry of known map types and call the corresponding
 * `<metric_map_class>::MapDefinition()`. */
TMetricMapInitializer* TMetricMapInitializer::factory(
	const std::string& mapClassName)
{
	using internal::TMetricMapTypesRegistry;
	TMetricMapTypesRegistry& mmr = TMetricMapTypesRegistry::Instance();
	return mmr.factoryMapDefinition(mapClassName);
}

TMetricMapInitializer::TMetricMapInitializer(
	const mrpt::rtti::TRuntimeClassId* classID)
	: metricMapClassType(classID)
{
}

/** Load all params from a config file/source. For examples and format, read the
 * docs of mrpt::maps::CMultiMetricMap */
void TMetricMapInitializer::loadFromConfigFile(
	const mrpt::config::CConfigFileBase& source,
	const std::string& sectionNamePrefix)
{
	// Common:
	const std::string sctCreat =
		sectionNamePrefix + std::string("_creationOpts");
	this->genericMapParams.loadFromConfigFile(source, sctCreat);

	// Class-specific:
	this->loadFromConfigFile_map_specific(source, sectionNamePrefix);
}

void TMetricMapInitializer::dumpToTextStream(std::ostream& out) const
{
	out << "-----------------TMetricMapInitializer --------------------\n";
	out << "================ C++ Class: '"
		<< this->metricMapClassType->className << "'\n";
	this->genericMapParams.dumpToTextStream(out);
	// Class-specific:
	this->dumpToTextStream_map_specific(out);
}

void TSetOfMetricMapInitializers::dumpToTextStream(std::ostream& out) const
{
	MRPT_START
	out << "===============================================================\n\n"
		   "      Set of internal maps for 'CMultiMetricMap' object\n\n"
		   "=================================================================\n"
		   // Show each map:
		   "Showing next the "
		<< this->size() << " internal maps:\n\n";

	int i = 0;
	for (auto it = begin(); it != end(); ++it, i++)
	{
		out << mrpt::format(
			"------- Internal map %u out of %u:\n", i + 1, (int)size());
		(*it)->dumpToTextStream(out);
	}  // for "it"
	MRPT_END
}
