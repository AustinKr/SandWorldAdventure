#include "HeaderFiles/Game/GameInstance.h"
#include "HeaderFiles/MasterWindow.h"

namespace SandboxEngine::Game
{
	std::vector<std::pair<std::string, GameObject::IGameObject*>> GameInstance::GameObjectsRegistry;
	GraphicsPipeline::GraphicsPipeline2D GameInstance::Pipeline;
	Time GameInstance::TimeInfo;

	void GameInstance::RegisterGameObject(std::string objectName, const GameObject::IGameObject* pGameObject)
	{
		GameObjectsRegistry.push_back(std::make_pair(objectName, (GameObject::IGameObject*)pGameObject));
	}

	void GameInstance::UpdateObjects()
	{
		WhereAllObjects([](std::vector<std::pair<std::string, GameObject::IGameObject*>>::const_iterator objIter)
		{
			objIter->second->Update(TimeInfo);
		});
	}
	void GameInstance::Release()
	{
		WhereAllObjects([](std::vector<std::pair<std::string, GameObject::IGameObject*>>::const_iterator objIter)
		{
			GameObject::IGameObject* pObj = objIter->second;
			pObj->Release(); 
			delete(pObj);
		});

		Pipeline.Release();
	}

	void GameInstance::WhereAllObjects(void(*func)(std::vector<std::pair<std::string, GameObject::IGameObject*>>::const_iterator objIter))
	{
		for (auto objIterator = GameObjectsRegistry.cbegin(); objIterator != GameObjectsRegistry.cend(); objIterator++)
		{
			func(objIterator);
		}
	}
}