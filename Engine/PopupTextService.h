#pragma once
#include <queue>

class CSpriteInstance;
class CTextInstance;
class CGameObject;

enum class EPopupType
{
	Damage,
	Info,
	Tutorial,
	Warning
};

struct SDamagePopupData
{
	float myDamage;
	int myHitType;
	CGameObject* myGameObject;
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
	float myFadeOutThreshold;
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
	const std::vector<CSpriteInstance*> GetSprites();

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

	CTextInstance* myActiveWarningText;
	CTextInstance* myWarningText;
	STextAnimationData* myWarningAnimationData;

	std::vector<CSpriteInstance*> mySkillIcons;
	std::vector<std::string> myStoredSkillInfoStrings;
	CSpriteInstance* myActiveSkillSprite;
	CSpriteInstance* myInfoBoxBackground;
	CTextInstance* myActiveInfoBoxText;
	CTextInstance* myInfoBoxText;
	STextAnimationData* myInfoAnimationData;

	std::unordered_map<std::string, CGameObject*> myActiveGameObject;

private:
	CPopupTextService();
	~CPopupTextService();

	void SpawnDamageNumber(void* someData);
	void SpawnInfoBox(std::string someInfoIdentifier);
	void SpawnTutorialText(std::string aText);
	void SpawnWarningText(std::string aText);

	void UpdateResources();

	float DamageSizeCurve(float x);
};
