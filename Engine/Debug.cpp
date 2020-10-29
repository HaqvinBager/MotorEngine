#include "stdafx.h"
#include "Debug.h"
#include "LineFactory.h"
#include "LineInstance.h"
#include "Timer.h"

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
	SLineTime lineData = SLineTime { };
	lineData.myLine = std::move(CLineInstance().Init(CLineFactory::GetInstance()->CreateLine(aPositionA, aPositionB, { 0.1f, 255.0f, 0.1f, 1.0f })));
	lineData.myTime = CTimer::Time() + CTimer::Dt() * 0.1f;
	myLines.emplace_back(std::move(lineData));
}

const std::vector<CLineInstance>& CDebug::GetLines() const
{
	return myLineInstances;
}

const std::vector<SLineTime>& CDebug::GetLinesTime() const
{
	return myLines;
}

void CDebug::Update()
{	
	if(myLines.size() != 0){

		while (myLines[0].myTime < CTimer::Time())
		{
			myLines.erase(myLines.begin());
			if (myLines.size() == 0)
				break;
		}
	}
}
