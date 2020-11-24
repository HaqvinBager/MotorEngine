#include "stdafx.h"
#include "Scene.h"
#include "EnvironmentLight.h"
#include "ModelComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "Camera.h"
#include "CollisionManager.h"
#include "PointLight.h"
#include "ParticleInstance.h"
#include "VFXInstance.h"
#include "LineInstance.h"
#include "SpriteInstance.h"
#include "AnimatedUIElement.h"
#include "Component.h"
#include "Debug.h"
#include <algorithm>
#include "CameraComponent.h"
#include "NavmeshLoader.h"
#include "LineFactory.h"

CScene* CScene::ourInstance = nullptr;

CScene* CScene::GetInstance()
{
	return ourInstance;
}

CScene::CScene()
{
	myIsReadyToRender = false;
	ourInstance = this;
	myMainCamera = nullptr;
	myCollisionManager = new CCollisionManager();
	myModelToOutline = nullptr;
	myEnvironmentLight = nullptr;
}

CScene::~CScene()
{
	ourInstance = nullptr;
	myMainCamera = nullptr;
	delete myCollisionManager;
	myCollisionManager = nullptr;
}


bool CScene::Init()
{
	return true;
}

bool CScene::InitNavMesh(std::string aPath)
{
	CNavmeshLoader* loader = new CNavmeshLoader();
	myNavMesh = loader->LoadNavmesh(aPath);
	
	if (!myNavMesh) {
		return false;
	}

	std::vector<DirectX::SimpleMath::Vector3> positions;
	positions.resize(myNavMesh->myTriangles.size() * 6);

	for (UINT i = 0, j = 0; i < positions.size() && j < myNavMesh->myTriangles.size(); i += 6, j++)
	{
		positions[i + 0] = myNavMesh->myTriangles[j]->myVertexPositions[0];
		positions[i + 1] = myNavMesh->myTriangles[j]->myVertexPositions[1];
		positions[i + 2] = myNavMesh->myTriangles[j]->myVertexPositions[2];
		positions[i + 3] = myNavMesh->myTriangles[j]->myVertexPositions[0];
		positions[i + 4] = myNavMesh->myTriangles[j]->myVertexPositions[1];
		positions[i + 5] = myNavMesh->myTriangles[j]->myVertexPositions[2];
	}

	myNavMeshGrid = new CLineInstance();
	myNavMeshGrid->Init(CLineFactory::GetInstance()->CreatePolygon(positions));
	this->AddInstance(myNavMeshGrid);

	delete loader;
	return true;
}


void CScene::SetMainCamera(CCameraComponent* aCamera)
{
	myMainCamera = aCamera;
}

CCameraComponent* CScene::GetMainCamera()
{
	return myMainCamera;
}

CEnvironmentLight* CScene::GetEnvironmentLight()
{
	return myEnvironmentLight;
}

SNavMesh* CScene::GetNavMesh()
{
	return myNavMesh;
}

std::vector<CGameObject*> CScene::CullGameObjects(CCameraComponent* aMainCamera)
{
	using namespace DirectX::SimpleMath;
	Vector3 cameraPosition = aMainCamera->GameObject().myTransform->Transform().Translation();
	std::vector<CGameObject*> culledGameObjects;
	for (auto& gameObject : myGameObjects)
	{
		if (!gameObject->Active()) {
			continue;
		}

		float distanceToCameraSquared = Vector3::DistanceSquared(gameObject->GetComponent<CTransformComponent>()->Position(), cameraPosition);
		if (distanceToCameraSquared < 10000.0f)
		{
			culledGameObjects.emplace_back(gameObject);
		}
	}
	return culledGameObjects;
}

std::pair<unsigned int, std::array<CPointLight*, 8>> CScene::CullLights(CGameObject* aGameObject) {
	std::pair<unsigned int, std::array<CPointLight*, 8>> pointLightPair;
	UINT counter = 0;

	for (UINT i = 0; i < myPointLights.size() && counter < 8; ++i) {
		float distanceSquared = DirectX::SimpleMath::Vector3::DistanceSquared(myPointLights[i]->GetPosition(), aGameObject->GetComponent<CTransformComponent>()->Transform().Translation());
		float range = myPointLights[i]->GetRange();
		if (distanceSquared < (range * range)) {
			pointLightPair.second[counter] = myPointLights[i];
			++counter;
		}
	}

	pointLightPair.first = counter;
	return pointLightPair;
}

std::vector<CParticleInstance*> CScene::CullParticles(CCameraComponent* aMainCamera)
{
	for (unsigned int i = 0; i < myParticles.size(); ++i)
	{
		myParticles[i]->Update(CTimer::Dt(), aMainCamera->GameObject().myTransform->Position());
	}
	return myParticles;
}

std::vector<CVFXInstance*> CScene::CullVFX(CCameraComponent* /*aMainCamera*/) {
	
	for (unsigned int i = 0; i < myVFXInstances.size(); ++i) {
		
		myVFXInstances[i]->Scroll({0.15f * CTimer::Dt(), 0.15f * CTimer::Dt() }, { 0.15f * CTimer::Dt() , 0.15f * CTimer::Dt() });
	}
	return myVFXInstances;
}

const std::vector<SLineTime>& CScene::CullLines() const
{
	return CDebug::GetInstance()->GetLinesTime();
	//return CDebug::GetInstance()->GetLines();
}

const std::vector<CLineInstance*>& CScene::CullLineInstances() const
{
	return myLineInstances;
}

std::vector<CSpriteInstance*> CScene::CullSprites()
{
	std::vector<CSpriteInstance*> spritesToRender;

	for (UINT i = 0; i < mySpriteInstances.size(); ++i)
	{
		for (auto& sprite : mySpriteInstances[static_cast<ERenderOrder>(i)])
		{
			if (sprite->GetShouldRender())
			{
				spritesToRender.emplace_back(sprite);
			}
		}
	}

	return spritesToRender;
}

std::vector<CAnimatedUIElement*> CScene::CullAnimatedUI(std::vector<CSpriteInstance*>& someFramesToReturn)
{
	std::vector<CAnimatedUIElement*> elementsToRender;
	for (auto& element : myAnimatedUIElements) {
		if (element->GetInstance()->GetShouldRender()) {
			elementsToRender.emplace_back(element);
			someFramesToReturn.emplace_back(element->GetInstance());
		}
	}
	return elementsToRender;
}

std::vector<CTextInstance*> CScene::GetTexts()
{
	return myTexts;
}

bool CScene::AddInstances(std::vector<CGameObject*>& someGameObjects)
{
	if (someGameObjects.size() == 0)
		return false;

	for (unsigned int i = 0; i < someGameObjects.size(); ++i)
	{
		myGameObjects.emplace_back(someGameObjects[i]);
	}


	//myGameObjects.insert(myGameObjects.end(), someGameObjects.begin(), someGameObjects.end());
	return true;
}

bool CScene::SetEnvironmentLight(CEnvironmentLight* anEnvironmentLight)
{
	myEnvironmentLight = anEnvironmentLight;
	return true;
}

bool CScene::AddInstance(CPointLight* aPointLight) {
	myPointLights.emplace_back(aPointLight);
	return true;
}

bool CScene::AddInstance(CGameObject* aGameObject)
{
	myGameObjects.emplace_back(aGameObject);
	return true;
}

bool CScene::AddInstance(CParticleInstance* aParticleInstance)
{
	myParticles.emplace_back(aParticleInstance);
	return true;
}

bool CScene::AddInstance(CVFXInstance* aVFXInstance) {
	myVFXInstances.emplace_back(aVFXInstance);
	return true;
}

bool CScene::AddInstance(CLineInstance* aLineInstance) {
	myLineInstances.emplace_back(aLineInstance);
	return true;
}

bool CScene::AddInstance(CSpriteInstance* aSprite) {
	if (!aSprite) {
		return false;
	}

	mySpriteInstances[aSprite->GetRenderOrder()].emplace_back(aSprite);

	return true;
}

bool CScene::AddInstance(CAnimatedUIElement* anAnimatedUIElement)
{
	if (!anAnimatedUIElement) {
		return false;
	}
	myAnimatedUIElements.emplace_back(anAnimatedUIElement);
	return true;
}

bool CScene::AddInstance(CTextInstance* aText)
{
	if (!aText) {
		return false;
	}
	myTexts.emplace_back(aText);
	return true;
}

bool CScene::RemoveInstance(CGameObject* aGameObject) {
	for (int i = 0; i < myGameObjects.size(); ++i) {
		if (aGameObject == myGameObjects[i]) {
			//std::swap(myGameObjects[i], myGameObjects[myGameObjects.size() - 1]);
			//myGameObjects.pop_back();
			myGameObjects.erase(myGameObjects.begin() + i);
		}
	}
	return true;
}

bool CScene::RemoveInstance(CPointLight* aPointLight)
{
	for (int i = 0; i < myPointLights.size(); ++i) {
		if (aPointLight == myPointLights[i]) {
			//std::swap(myGameObjects[i], myGameObjects[myGameObjects.size() - 1]);
			//myGameObjects.pop_back();
			myPointLights.erase(myPointLights.begin() + i);
		}
	}
	return true;
}

bool CScene::ClearScene() {

	for (auto gameObject : myGameObjects) {
		delete gameObject;
		gameObject = nullptr;
	}
	myGameObjects.clear();
	return true;
}

bool CScene::ClearSprites() {

	for (UINT i = 0; i < mySpriteInstances.size(); ++i)
	{
		for (auto& sprite : mySpriteInstances[static_cast<ERenderOrder>(i)])
		{
			delete sprite;
			sprite = nullptr;
		}
	}
	mySpriteInstances.clear();

	return true;
}

void CScene::SetModelToOutline(CGameObject* aGameObject)
{
	if (myModelToOutline) {
		myGameObjects.emplace_back(std::move(myModelToOutline));
	}
	auto it = std::find(myGameObjects.begin(), myGameObjects.end(), aGameObject);
	if (it != myGameObjects.end()) {
		std::swap(*it, myGameObjects.back());
		myModelToOutline = myGameObjects.back();
		myGameObjects.pop_back();
	}
	else {
		myModelToOutline = aGameObject;
	}
}
