#include "stdafx.h"
#include "Debug.h"
#include "LineFactory.h"
#include "LineInstance.h"

CDebug* CDebug::ourInstance = nullptr;
CDebug::CDebug()
{
	ourInstance = this;
}

CDebug::~CDebug()
{
	ourInstance = nullptr;
}

CDebug* CDebug::GetInstance()
{
	return ourInstance;
}

void CDebug::DrawLine(DirectX::SimpleMath::Vector3 aPositionA, DirectX::SimpleMath::Vector3 aPositionB)
{	
	CLineInstance line = { };
	line.Init(CLineFactory::GetInstance()->CreateLine(aPositionA, aPositionB, { 0.1f, 255.0f, 0.1f, 1.0f }));
	myLineInstances.emplace_back(std::move(line));
}

const std::vector<CLineInstance>& CDebug::GetLines() const
{
	return myLineInstances;
}
