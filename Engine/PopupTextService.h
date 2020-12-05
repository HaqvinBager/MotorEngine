#pragma once
#include <queue>

class CSpriteInstance;
class CTextInstance;

enum class EPopupType
{
	Damage,
	Info,
	Tutorial
};

struct SDamagePopupData
{
	float myDamage;
	int myHitType;
};

struct STextAnimationData
{
	DirectX::SimpleMath::Vector4 myStartColor;
	DirectX::SimpleMath::Vector4 myEndColor;
	DirectX::SimpleMath::Vector2 myMinScale;
	DirectX::SimpleMath::Vector2 myMaxScale;
	DirectX::SimpleMath::Vector2 myStartSpeed;
	DirectX::SimpleMath::Vector2 mySpeed;
	float myLifespan;
	float myTimer;
};

class CPopupTextService
{
	friend class CMainSingleton;

public:
	bool Init();

	void SpawnPopup(EPopupType aType, void* someData);

public:
	const std::vector<CTextInstance*>& GetTexts();
	const std::vector<CSpriteInstance*>& GetSprites();

private:
	std::vector<CTextInstance*> myActiveTexts;
	std::vector<CSpriteInstance*> myActiveSprites;
	std::vector<STextAnimationData*> myTextAnimationData;

	std::queue<CTextInstance*> myTextPool;
	std::queue<STextAnimationData*> myAnimatedDataPool;

private:
	CPopupTextService();
	~CPopupTextService();

	void UpdateTexts();
	void UpdateSprites();

	float SizeCurve(float x);
};
