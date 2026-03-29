#include "SWAEngine/Tilemap/TilePropertyManager/PropertyManager.h"

namespace SWAEngine::Tilemap::TilePropertyManager
{
	void PropertyManager::TryEraseData(void* data, Math::Int3 tileLocation, bool shouldRelease)
	{
		if (data == nullptr)
			return;

		// Try remove from shared
		if (DataContainer.at(data).contains(tileLocation))
			DataContainer.at(data).erase(tileLocation);

		// If no tiles are referencing this property, release its memory
		if (!shouldRelease || DataContainer.at(data).size() > 0)
			return;
		DataContainer.erase(data);
		delete(data);



		//// Release if not shared
		//if (rSharedTiles.size() <= 1)
		//{
		//	delete(data);
		//	DataContainer.erase(data);
		//	return;
		//}

		//// Else it is shared, so try to resolve
 	//	if (!rSharedTiles.contains(tileLocation))
		//	return;
		//// Try resolve
		//rSharedTiles.erase(tileLocation); // This shared tile is trying to release this property
		//if (rSharedTiles.size() > 1) // If still shared by others
		//	return;
		//// Release memory
		//delete(data);
		//DataContainer.erase(data);
	}
}