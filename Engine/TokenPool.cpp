#include "stdafx.h"
#include "TokenPool.h"

CTokenPool* CTokenPool::ourInstance = nullptr;

CTokenPool::CTokenPool(int aMaxTokens, float aTokenCooldown)
{
	ourInstance = this;

	for (int i = 0; i < aMaxTokens; ++i) {
		CToken* token = new CToken();
		token->Init(aTokenCooldown);
		myTokens.emplace_back(token);
	}
}

CTokenPool::~CTokenPool()
{
	ourInstance = nullptr;
	for (auto& token : myTokens)
	{
		delete token;
		token = nullptr;
	}
}

void CTokenPool::Update()
{
	for (auto& tokens : myTokens) {
		tokens->Update();
	}
}

CToken* CTokenPool::Request() const
{
	for (int i = 0; i < myTokens.size(); ++i) {
		if (myTokens[i]->Use() == false) {
			if (myTokens[i]->Cooldown() == false) {
				myTokens[i]->Use(true);
				return myTokens[i];
			}
		}
	}
	return nullptr;
}

void CTokenPool::GiveBack(CToken& aToken, bool aCooldown)
{
	for (int i = 0; i < myTokens.size(); ++i) {
		if (&aToken == myTokens[i]) {
			myTokens[i]->Use(false);
			myTokens[i]->Cooldown(aCooldown);
		}
	}
}
