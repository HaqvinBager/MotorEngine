#include "stdafx.h"
#include "State.h"

CState::CState(CStateStack& aStateStack, const CStateStack::EState aState): myStateStack(aStateStack), myState(aState) {

}

CState::~CState() {
}
