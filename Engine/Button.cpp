#include "Button.h"
#include "stdafx.h"

void CButton::OnHover()
{
}

void CButton::OnLeave()
{
}

CButton::CButton(std::function<void()> aFunction) :
	myFunction(aFunction)
{
}

CButton::~CButton()
{
}
