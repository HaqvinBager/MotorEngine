#pragma once
#include <map>
#include "Text.h"

class CDirectXFramework;

namespace DirectX {
	class SpriteFont;
}

class CTextFactory
{
	friend class CEngine;
public:
	bool Init(CDirectXFramework* aFramework);
	
	CText* LoadText(std::string aFontAndSize);
	CText* GetText(std::string aFontAndSize);

	static CTextFactory* GetInstance();

private:
	CTextFactory();
	~CTextFactory();

private:
	static CTextFactory* ourInstance;
	std::map<std::string, CText*> myTextMap;
	CDirectXFramework* myFramework;
};

