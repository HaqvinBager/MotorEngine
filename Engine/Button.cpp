#include "Button.h"
#include "stdafx.h"
#include "MainSingleton.h"
#include "SpriteFactory.h"
#include "SpriteInstance.h"

void CButton::OnHover()
{
	mySprites.at(static_cast<size_t>(EButtonState::Idle))->SetShouldRender(false);
	mySprites.at(static_cast<size_t>(EButtonState::Hover))->SetShouldRender(true);
}

void CButton::OnClickDown()
{
	mySprites.at(static_cast<size_t>(EButtonState::Hover))->SetShouldRender(false);
	mySprites.at(static_cast<size_t>(EButtonState::Click))->SetShouldRender(true);
}

void CButton::OnClickUp(void* someData)
{
	mySprites.at(static_cast<size_t>(EButtonState::Click))->SetShouldRender(false);
	mySprites.at(static_cast<size_t>(EButtonState::Hover))->SetShouldRender(true);

	for (unsigned int i = 0; i < myMessagesToSend.size(); ++i)
	{
		CMainSingleton::PostMaster().Send({ myMessagesToSend[i],  someData });
	}
}

void CButton::OnLeave()
{
}

CButton::CButton(SButtonData& /*someData*/)
	/*: myMessagesToSend(someMessageTypes)*/
{
	//Load Sprites

	mySprites.at(static_cast<size_t>(EButtonState::Hover))->SetShouldRender(false);
	mySprites.at(static_cast<size_t>(EButtonState::Click))->SetShouldRender(false);
}

CButton::~CButton()
{
	delete mySprites.at(static_cast<size_t>(EButtonState::Idle));
	delete mySprites.at(static_cast<size_t>(EButtonState::Hover));
	delete mySprites.at(static_cast<size_t>(EButtonState::Click));
}
