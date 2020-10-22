#include "stdafx.h"
#include "VFXBase.h"

CVFXBase::CVFXBase() : myVFXBaseData() {
}

CVFXBase::~CVFXBase() {
}

bool CVFXBase::Init(SVFXBaseData someData) {
	myVFXBaseData = someData;
	return true;
}

CVFXBase::SVFXBaseData& CVFXBase::GetVFXBaseData() {
	return myVFXBaseData;
}
