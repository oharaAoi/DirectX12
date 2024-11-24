#include "Panel.h"

Panel::Panel() {
}

Panel::~Panel() {
}

void Panel::Init() {
	time_ = 0.0f;
	timeLimit_ = 1.0f;
}

void Panel::Update() {
}

void Panel::Draw() const {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　
//////////////////////////////////////////////////////////////////////////////////////////////////

void Panel::SetBlackOut() {

}

void Panel::SetBlackOutOpen() {

}


#ifdef _DEBUG
void Panel::Debug_Gui() {

}
#endif