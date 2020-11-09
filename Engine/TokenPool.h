#pragma once

class CToken {
public:
	CToken() 
		: defaultTokenCooldown(0.f)
		, tokenCooldown(0.f)
		, onCooldown(false)
		, inUse(false) 
	{
	}
	~CToken() {};

	void Init(float aTokenCooldown) { tokenCooldown = aTokenCooldown; defaultTokenCooldown = aTokenCooldown; }

	void Update() { 
		if (onCooldown == true) {
			tokenCooldown -= CTimer::Dt();
			if (tokenCooldown <= 0.f) {
				onCooldown = false;
				tokenCooldown = defaultTokenCooldown;
			}
		} 
	}

	void Use(bool aUse) { inUse = aUse; }
	bool Use() const { return inUse; }

	void Cooldown(bool aCooldown) { onCooldown = aCooldown; }
	bool Cooldown() const { return onCooldown; }
private:
	float defaultTokenCooldown;
	float tokenCooldown;
	bool onCooldown;
	bool inUse;
};


class CTokenPool
{
public:
	static CTokenPool* GetInstance() { return ourInstance; };
public:

	CTokenPool(int aMaxTokens = 0, float aTokenCooldown = 0.f);
	~CTokenPool();

	void Update();

	CToken* Request() const;

	void GiveBack(CToken& aToken, bool aCooldown);


private:
	static CTokenPool* ourInstance;
private:
	std::vector<CToken*> myTokens;
};

