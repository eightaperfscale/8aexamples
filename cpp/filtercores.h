// Example source code to filter out physical cores.
//
// The source code is included as-is with no warranty.
//
void FilterCores (std::vector<int> &cores)
{
	// Initialize
	hwloc_topology_t topology;
	hwloc_topology_init(&topology);
	hwloc_topology_load(topology);

	// Load the topology e.g. sockets, cores etc.
	auto depth = hwloc_get_type_depth(topology, HWLOC_OBJ_CORE);
   	auto phyCores = hwloc_get_nbobjs_by_depth(topology, depth);

	//	Loop through the physical cores to filter out threads
	for (auto i = 0; i < phyCores; i ++)
	{
		// Get the information for this core
		auto core = hwloc_get_obj_by_depth(topology, depth, i);

		// Each physical core can have multiple processing units (PU). 
		// We just need the first
		auto first = hwloc_get_next_child(topology, core, nullptr);
		if (first && first->type == HWLOC_OBJ_PU)
		{
			// Set this core in the list
			cores.push_back(first->os_index);
		}
	}

	// Clean up
	hwloc_topology_destroy(topology);
}
