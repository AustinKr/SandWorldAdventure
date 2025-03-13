#include "HeaderFiles/Game/GameInstance.h"
#include "HeaderFiles/MasterWindow.h"

namespace SandboxEngine::Game
{
	std::vector<Layer> GameInstance::Layers;
	Render::Camera GameInstance::MainCamera;
	Time GameInstance::TimeInfo;

	void GameInstance::RegisterGameObject(int layer, std::string layerName, const GameObject::IGameObject* pGameObject, std::string objectName)
	{
		Layers.insert(Layers.cbegin() + layer, Layer(layerName))->Objects.insert(std::make_pair(objectName, (GameObject::IGameObject*)pGameObject));
	}
	void GameInstance::RegisterGameObject(std::vector<Layer>::iterator layerIterator, const GameObject::IGameObject* pGameObject, std::string objectName)
	{
		layerIterator->Objects.insert(std::make_pair(objectName, (GameObject::IGameObject*)pGameObject));
	}

	void GameInstance::UpdateObjects()
	{
		WhereAllObjects([](std::vector<Layer>::reverse_iterator iter, std::unordered_map<std::string, GameObject::IGameObject*>::const_iterator objIter)
		{
			objIter->second->Update(TimeInfo);
		});
	}
	void GameInstance::CompositeLayers()
	{
		WhereAllObjects([](std::vector<Layer>::reverse_iterator iter, std::unordered_map<std::string, GameObject::IGameObject*>::const_iterator objIter)
		{
			objIter->second->CopyScreen(&MainCamera.MainScreen);
		});
		
		//// TODO: could remove this and use only the camera screen
		//Vector2 camPos = Renderer::FromWorldToScreen(&MasterWindow::ScreenState, MainCamera.Position);
		//Vector2 camScale = Renderer::FromWorldToScreen(&MasterWindow::ScreenState, MainCamera.Scale, false);
		//Renderer::CopyState(&MainCamera.MainScreen, floor(camPos.X), floor(camPos.Y), &MasterWindow::ScreenState, 0, 0, NULL_CAMERA_COLOR);
	}
	void GameInstance::Release()
	{
		WhereAllObjects([](std::vector<Layer>::reverse_iterator iter, std::unordered_map<std::string, GameObject::IGameObject*>::const_iterator objIter)
		{
			GameObject::IGameObject* pObj = objIter->second;
			pObj->Release(); 
			delete(pObj);
		});
	}

	void GameInstance::WhereAllObjects(void(*func)(std::vector<Layer>::reverse_iterator layerIter, std::unordered_map<std::string, GameObject::IGameObject*>::const_iterator objIter))
	{
		for (auto iterator = Layers.rbegin(); iterator != Layers.rend(); iterator++)
		{
			for (auto objIterator = iterator->Objects.cbegin(); objIterator != iterator->Objects.cend(); objIterator++)
			{
				func(iterator, objIterator);
			}
		}
	}
}