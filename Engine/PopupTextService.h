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
	void SpawnPopup(EPopupType aType, std::string aNameOrText);

public:
	const std::vector<CTextInstance*> GetTexts();
	const std::vector<CSpriteInstance*>& GetSprites();

private:
	std::vector<CTextInstance*> myActiveDamageNumbers;
	std::vector<STextAnimationData*> myDamageAnimationData;
	std::vector<CSpriteInstance*> myActiveSprites;

	std::queue<CTextInstance*> myTextPool;
	std::queue<STextAnimationData*> myAnimatedDataPool;

	DirectX::SimpleMath::Vector4 myDamageColors[5];
	
	CTextInstance* myActiveTutorialText;
	CTextInstance* myTutorialText;
	STextAnimationData* myTutorialAnimationData;

private:
	CPopupTextService();
	~CPopupTextService();

	void SpawnDamageNumber(void* someData);
	void SpawnInfoBox(void* someData);
	void SpawnTutorialText(std::string aText);

	void UpdateTexts();
	void UpdateSprites();

	float DamageSizeCurve(float x);
};
